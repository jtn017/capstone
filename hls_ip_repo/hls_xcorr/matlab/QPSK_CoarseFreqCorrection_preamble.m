%% QPSK Simulation
%  Project: Capstone V2X Motorcycle HUD
%  Filename: QPSK_FrequencyCorrection.m
%
%  Description: Coarse Frequency Correction using preamble

clear all
close all
clc

set(0,'DefaultFigureWindowStyle','docked')

rng(1)
% Script Settings
NO_RX_SRRC = 0; % 1 - Use Rx SRRC, 0 - RX SRRC bypassed
AGC = 0; % 1 - AGC on, 2 - AGC off
LARGE_DOPPLER = 0; % set this to select amount of doppler

% Waveform Settings
NSAMP= 10000;
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

rolloff = 0.5;
tx_shaping_filter=rcosine(1,sps,'sqrt',rolloff);
tx_shaping_filter=tx_shaping_filter/sum(tx_shaping_filter);

tx_qpsk = conv(sps*tx_shaping_filter,symbols_x8);

%% Channel
if LARGE_DOPPLER == 1
    frequency_offset = 25e3;
else
    frequency_offset = 800;
end

atten = 1;
tx_doppler = tx_qpsk.*exp(j*2*pi.*frequency_offset/fs.*(0:length(tx_qpsk)-1));
tx_atten = atten*tx_doppler;

figure('Name','After Channel')
subplot(211)
plot(real(tx_atten),imag(tx_atten),'bx','LineWidth',2)
axis([-2.2 2.2 -2.2 2.2])
grid on;

subplot(212)
NFFT = 16384;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_qpsk,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_atten,NFFT)))),'LineWidth',2);
legend('tx sent','tx after channel')

%% Matching SRRC Filter
rx_srrc_filter=rcosine(1,8,'sqrt',rolloff);

if (NO_RX_SRRC)
    rx_signal = tx_atten;
else
    rx_signal = conv(sps*rx_srrc_filter,tx_atten);
end

%% AGC
agc = comm.AGC('AveragingLength',10);

if (AGC)
    rx_signal = agc(rx_signal.')';
end

rx_len = length(rx_signal);

figure('Name','AGC')
plot(real(rx_signal),imag(rx_signal),'bx','LineWidth',2)
%axis([-1.5 1.5 -1.5 1.5])
grid on;
legend('Symbols','Upsampled')

%% Generate Test Data FOR HLS Test Bench
data_bits = 15;
res = saveIQdat(rx_signal,data_bits,'xcorr_input.dat');

%% Generate matched filter
% Notes: I see three options: 
%     (1) use upsampled version w/ zeros (favored bc of resources)
%     (2) use a repeated version
%     (3) use the upsampled version passed through the SRRC

mf = upsample(symbols(1:64),sps); % (1)
%mf = reshape(repmat(symbols(1:64),sps,1),1,[]) % (2)
b = flipud(mf');

y = filter(b,1,symbols_x8);

y2 = filter(b,1,tx_qpsk);
save 'mf.mat' b -mat
%% Correlation With Matched Filter

figure('Name', 'Preamble Convolution')
subplot(211)
plot(abs(y))
subplot(212)
plot(abs(y2))

%% Do Frequency Correction
[amp,loc] = max(abs(real(y2))+abs(imag(y2))); %% Do we need to do magnitude? 
%[amp,loc] = max(abs(y2)); %% Do we need to do magnitude? 

N = 32*8; % length preamble * SPS (256)
N1 = loc-(2*N);
N2 = N1 + N;

Z2 = mean(conj(rx_signal(N1+(0:1-1))) .* rx_signal(N2+(0:1-1)));
fo = atan2(imag(Z2),real(Z2))./(2*pi*N);

fprintf("expected frequency correction value: %d\n",frequency_offset/fs)
fprintf("calulated frequency correction value: %d\n",fo)

f_expect = exp(-1i*2*pi*frequency_offset/fs*(0:length(rx_signal)-1));
f_correct = exp(-1i*2*pi*fo*(0:length(rx_signal)-1));

rx_f_expected = rx_signal .* f_expect;
rx_f_corrected = rx_signal .* f_correct;

%% Plot Resulting Signal

figure()
NFFT = 16384;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_signal,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_f_corrected,NFFT)))),'r','LineWidth',2);
%plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_f_expected,NFFT)))),'g-.','LineWidth',2);
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(f_correct,NFFT)))),'k','LineWidth',2);
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(f_expect,NFFT)))),'R','LineWidth',2);
legend('tx', 'rx corrected tru','rx corrected','f correction','f expected')
ylim([0 100])
figure('Name','FLL OUT')
plot(real(rx_f_corrected),imag(rx_f_corrected),'bx','LineWidth',2)
%axis([-2.2 2.2 -2.2 2.2])
grid on;
legend('Symbols','Upsampled')


%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')
%