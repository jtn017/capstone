%% QPSK Simulation
%  Project: Capstone V2X Motorcycle HUD
%  Filename: QPSK_FrequencyCorrection.m
%
%  Description: FLL - Frequency Correction 
%     Using bandedge filters as described in 
%     fred harris' "Let s Assume the System Is Synchronized"

clear all
close all
clc

set(0,'DefaultFigureWindowStyle','docked')

LARGE_DOPPLER = 0; % set this to select amount of doppler

% Script Settings
GEN_COEFS = 0;

% Waveform Settings
NSAMP= 1000;
sps = 8;
fs = 13.44e6; % 8MHZ
fsym = fs/sps; % 1MHz

%% Generate TX Signal

% Generateion QAM Symbols, 8-samples per symbol
symbols = sign(rand(1,NSAMP)-0.5)+j*sign(rand(1,NSAMP)-0.5);

% Add preamble
preamble = pngen(6,32)*2-1;
preamble = preamble + preamble*1i;

symbols = [preamble preamble symbols];
NSAMP = length(symbols);

% Upsample
symbols_x8 = upsample(symbols,sps);

figure('Name','QAM 8-SPS')
plot(real(symbols),imag(symbols),'bx','LineWidth',2)
hold on;
plot(real(symbols_x8),imag(symbols_x8),'ro','LineWidth',2)
axis([-1.5 1.5 -1.5 1.5])
grid on;
legend('Symbols','Upsampled')

rolloff = 0.5
tx_shaping_filter=rcosine(1,sps,'sqrt',rolloff);
tx_shaping_filter=tx_shaping_filter/sum(tx_shaping_filter);

tx_qpsk = conv(sps*tx_shaping_filter,symbols_x8);

%% Channel
if LARGE_DOPPLER == 1
    frequency_offset = 1e6;
else
    frequency_offset = 800;
end
atten = 0.1;
atten = 1;
tx_doppler = tx_qpsk.*exp(j*2*pi.*frequency_offset/fs.*[0:length(tx_qpsk)-1]);
tx_atten = atten*tx_doppler;

figure('Name','After Channel')
subplot(211)
plot(real(tx_atten),imag(tx_atten),'bx','LineWidth',2)
axis([-1.5 1.5 -1.5 1.5])
grid on;
legend('Symbols','Upsampled')
subplot(212)
NFFT = 16384;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_qpsk,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_atten,NFFT)))),'LineWidth',2);
legend('Original','Doppler shifted signal')
title('Before and After Channel with Doppler')
ylabel('Amplitude (dB)')
xlabel('Normalized Frequency')

%% AGC
%agc = comm.AGC('AveragingLength',1000);
rx_signal = tx_atten; %agc(tx_atten.')';
rx_len = length(rx_signal);

figure('Name','AGC')
plot(real(rx_signal),imag(rx_signal),'bx','LineWidth',2)
%axis([-1.5 1.5 -1.5 1.5])
grid on;
legend('Symbols','Upsampled')

%% Generate Test Data FOR HLS Test Bench
data_bits = 15;
res = saveIQdat(rx_signal,data_bits,'fll_input.dat');

%% Band Edge Filter Creation 
%rx_signal=rx_signal(1:2:end);% offset receiver clock
%rx_len = length(rx_signal);

% Create a new filter for upper and lower using sincs
M = round(length(tx_shaping_filter)/sps); % Get length of shaping filter
hh1 = sinc(rolloff*(-M:2/sps:M)-0.5)+sinc(rolloff*(-M:2/sps:M)+0.5);
hh1 = hh1/sum(hh1);
N = (length(hh1)-1)/2;

% Now We will make Upper and Lower Band Filters
beu = exp( 1i*2*pi*(1+rolloff)*(-N:1:N)/(2*sps)).*hh1; % Shift Up 
bel = exp(-1i*2*pi*(1+rolloff)*(-N:1:N)/(2*sps)).*hh1; % Shift Down

figure()
plot(20*log10(abs(fftshift(fft(beu,8192)))),'LineWidth',2);
hold on;
plot(20*log10(abs(fftshift(fft(bel,8192)))),'LineWidth',2);

filt_len = length(beu);

% Save Filter Coeficients
if GEN_COEFS
    fid = fopen("../coef.h","w");
    fprintf(fid,"#ifndef COEF_H_\n");
    fprintf(fid,"#define COEF_H_\n\n");
    fprintf(fid,"#define NUMTAPS %d\n\n", filt_len);
    
    save_coefs(fid,"be_upper_coefs_re",real(beu));
    save_coefs(fid,"be_upper_coefs_im",imag(beu));
    save_coefs(fid,"be_lower_coefs_re",real(beu));
    save_coefs(fid,"be_lower_coefs_re",imag(beu));
    
    fprintf(fid,"\n#endif // COEF_H_\n");
    fclose(fid);
end

%% Frequency Correction
loop_len = rx_len-filt_len;
loop_filt = zeros(1,loop_len);

if LARGE_DOPPLER == 1
    theta= 2*pi/400; % Shared from coworkder
    zeta = sqrt(2)/2;
    ki = (4*theta^2)/(1+2*zeta*theta+theta^2);
    kp = (4*zeta*theta)/(1+2*zeta*theta+theta^2);
else
    theta= 2*pi/200; % Expeirmenatin ??
    zeta = sqrt(2)/2;
    ki = (2*theta^2)/(1+2*zeta*theta+theta^2);
    kp = (2*zeta*theta)/(1+2*zeta*theta+theta^2);
end


accum1 = 0;
accum2 = 0;
r_fll_out = zeros(1,filt_len);

for k=1:loop_len
    fll_out(k) = rx_signal(k)*exp(-1i*accum2);

    % Perform BE Filters
    r_fll_out = [fll_out(k) r_fll_out(1:end-1)]; % tap delay line
    beu_out(k) = r_fll_out*beu.';
    bel_out(k) = r_fll_out*bel.';

    % use three mult structure
    a = beu_out(k) + bel_out(k);
    b = beu_out(k) - bel_out(k);

    err = real(a*conj(b));

    if(err >= 1.5)
        err = 1.5;
    elseif(err < -1.5)
        err = -1.5;
    end

    % Loop Filer
    kp_err = kp*err;
    ki_err = ki*err;
    accum1 = accum1 + ki_err;
    loop_filt(k) = accum1 + kp_err;

    % Phase Accumulator
    accum2 = accum2 + loop_filt(k);

    if(accum2 >= pi)
        accum2 = accum2 - 2*pi;
    elseif(accum2 < -pi)
        accum2 = accum2 + 2*pi;
    end
end

% Plot Frequency Estimation
figure()
subplot(211)
plot(loop_filt)
title('FLL Freqency Estimation')
ylabel('Phase Error')
xlabel('Samples')

subplot(212)
plot(real(rx_signal(1:1000)))
hold on;
plot(real(fll_out(1:1000)))
legend('rx','fll out')

figure()
NFFT = 16384;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_signal,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(fll_out,NFFT)))),'LineWidth',2);
legend('tx', 'fll out')
legend('Received','Frequency Corrected')
title('Before and After FLL')
ylabel('Amplitude (dB)')
xlabel('Normalized Frequency')

figure('Name','FLL OUT')
plot(real(fll_out),imag(fll_out),'bx','LineWidth',2)
%axis([-1.5 1.5 -1.5 1.5])
grid on;
legend('Symbols','Upsampled')

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')