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
NO_RX_SRRC = 1; % 1 - Use Rx SRRC, 0 - RX SRRC bypassed
AGC = 0; % 1 - AGC on, 0 - AGC off

% Channel Settings
NO_DOPP = 0; % 1 - Don't Add Doppler, 0 - Add Doppler
LARGE_DOPPLER = 0; % set this to select amount of doppler
atten = 1;
SNRdB = 50;

% Waveform Settings
NSAMP= 10000;
sps = 8;
fs = 13.44e6; % 8MHZ
fsym = fs/sps; % 1MHz

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Channel
if LARGE_DOPPLER == 1
    frequency_offset = 25e3;
else
    frequency_offset = 800;
end

if (NO_DOPP == 1)
    tx_doppler = tx_qpsk;
else
    tx_doppler = tx_qpsk.*exp(j*2*pi.*frequency_offset/fs.*(0:length(tx_qpsk)-1));
end
tx_doppler = atten*tx_doppler;

% Noise
SNR = 10^(SNRdB/10);
signal_power = tx_doppler * tx_doppler'/length(tx_doppler);
noise_power = signal_power/ SNR *sps;
noise_voltage = sqrt(noise_power/2);
noise = noise_voltage*[1 j]*randn(2,length(tx_doppler));
tx_doppler_noise = tx_doppler+noise;

figure('Name','After Channel')
subplot(211)
plot(real(tx_doppler_noise),imag(tx_doppler_noise),'bx','LineWidth',2)
axis([-2.2 2.2 -2.2 2.2])
grid on;

subplot(212)
NFFT = 16384;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_qpsk,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_doppler_noise,NFFT)))),'LineWidth',2);
legend('tx sent','tx after channel')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Matching SRRC Filter
rx_srrc_filter=rcosine(1,8,'sqrt',rolloff);

if (NO_RX_SRRC)
    rx_signal = tx_doppler_noise;
else
    rx_signal = conv(sps*rx_srrc_filter,tx_doppler_noise);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% AGC
agc = comm.AGC('AveragingLength',50);

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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Correlation With Matched Filter

figure('Name', 'Preamble Convolution')
subplot(211)
plot(abs(y))
subplot(212)
plot(abs(y2))

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
subplot(211)
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_signal,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_f_corrected,NFFT)))),'r','LineWidth',2);
legend('Rx Signal', 'Rx Corrected')
ylim([0 100])

subplot(212)
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(f_correct,NFFT)))),'k','LineWidth',2);
hold on;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(f_expect,NFFT)))),'R','LineWidth',2);
legend('f correction','f expected')
ylim([0 100])

figure('Name','FLL OUT')
plot(real(rx_f_corrected),imag(rx_f_corrected),'bx','LineWidth',2)
%axis([-2.2 2.2 -2.2 2.2])
grid on;
legend('Symbols','Upsampled')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 32-path Polyphase Matched filter, 8-samples per symbol 
nfilts = 32;
h_rx=rcosine(1,nfilts*sps,'sqrt',rolloff);                % Matched filter
h_rx=[h_rx zeros(1,nfilts-mod(length(h_rx),nfilts))];                        % zeros extending
dh_rx=conv([1 0 -1],h_rx(:)');                   % derivative matched filter
dh_rx=dh_rx(2:end-1);

h_rx_poly=reshape(h_rx,nfilts,numel(h_rx)/nfilts);   % 32 path polyphase MF
dh_rx_poly=reshape(dh_rx,nfilts,numel(dh_rx)/nfilts); % 32 path polyphase dMF 

reg_len = size(h_rx_poly,2);
reg_t = zeros(1,reg_len);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Do Timing Error Correction

% Timing Recovery Loop
theta_0= 2*pi/200;
eta=sqrt(2)/2;
eta=6*eta;

k_i_t= (4*theta_0*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);
k_p_t= (4*eta*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);

int_t=0.0;
accum_t=1;
accum_t_sv=zeros(1,length(rx_f_corrected));

mm=1;                           % output clock at 1-sample per symbol
ssel = 0;
n1 = 1;

%for ssel=1:1:length(rx_f_corrected)-reg_len
nout_items = (length(rx_f_corrected)-reg_len-8)/8
idx = 0;
odx = 1;
ssel=1;

while idx < nout_items      
    fbsel=floor(accum_t);            % point to a coefficient set

    % select correct samples to operate on
    start = ssel;% + odx-1;
    reg_t = rx_f_corrected(start:start+reg_len-1);

    % Now lets filter
    y_t =reg_t*h_rx_poly(fbsel,:)'; % MF output time sample
    dy_t=reg_t*dh_rx_poly(fbsel,:)';% dMF output time sample 
    rx_tec(odx)=y_t;                % save MF output sample
    
    det_t=real(y_t)*real(dy_t);   % y*y_dot product (timing error)
    int_t=int_t+k_i_t*det_t;        % Loop filter integrator 
    sm_t =int_t+k_p_t*det_t;        % loop filter output
    
    accum_t_sv(odx)=accum_t;         % save timing accumulator content                  
    accum_t=accum_t+sm_t;           % update accumulator

    % Run beyond last filter so wrap around and move to next sample
    if accum_t > nfilts         
        accum_t =accum_t-nfilts;
        fbsel = fbsel-nfilts; % ask sal
        ssel = ssel + 1;
    end

    % Gone below 1 wrap around to previous sample
    if accum_t < 1                   
        accum_t = accum_t+nfilts;
        fbsel = fbsel+nfilts; % ask sal
        ssel = ssel - 1;
    end
    
    odx = odx + 1; % increment symbol clock
    idx = idx + 8; % increment input sample
    ssel = ssel + 8; % Sample Select
end

figure()
subplot(211)
plot(accum_t_sv)
hold on
plot(floor(accum_t_sv),'r')
hold off
grid on
%axis([0 2000 0 32])
title('Timing Loop Phase Accumulator and Pointer')

offset = 256;

subplot(224)
plot(real(rx_tec),imag(rx_tec),'bo')

hold off
grid on
axis('square')
title('Constellation Diagram')


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Phase Lock Loop

theta_0= 2*pi/500;
eta=sqrt(2)/2;
eta=1*eta;

k_i_ph= (4*theta_0*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);
k_p_ph= (4*eta*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);

int_ph=0.0;
accum_ph=0;
lp_flt=0;

for idx=1:length(rx_tec)
    prod=rx_tec(idx)*exp(j*2*pi*accum_ph);
    rx_pll(idx)=prod;
    
    % Calculate error
    det_ph=sign(real(prod))+j*sign(imag(prod));
    phs_err=angle(det_ph*conj(prod))/(2*pi);
    phs_err_sv(idx)=phs_err;

    %Loop filter
    int_ph=int_ph+k_i_ph*phs_err;
    lp_flt=int_ph+k_p_ph*phs_err;

    accum_ph_sv(idx)=accum_ph;
    accum_ph=accum_ph+lp_flt;
end

figure()
subplot(211)
plot(phs_err_sv(1:500))
grid on
title('Phase Lock Loop Phase Error')
   

subplot(224)
plot(rx_tec,'.')
hold on
plot(rx_pll(64:1:end),'.r') % Ingoring some of the preamble
hold off
grid on
axis('square')
axis([-1.5 1.5 -1.5 1.5])
title('Constellation Diagram')
legend('Before PLL','After PLL')

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')
%