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
NO_RX_SRRC = 1; % 0 - Use Rx SRRC, 1 - RX SRRC bypassed
AGC = 0; % 1 - AGC on, 0 - AGC off

% Channel Settings
NO_DOPP = 0; % 1 - Don't Add Doppler, 0 - Add Doppler
LARGE_DOPPLER = 0; % set this to select amount of doppler
phase_offset = pi/16;
SNRdB = 40;
adc_sps = 4; % Down Sample at Receiver?

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



rolloff = 0.5;
tx_shaping_filter=rcosine(1,sps,'sqrt',rolloff);
tx_shaping_filter=tx_shaping_filter/sum(tx_shaping_filter);

tx_qpsk = conv(sps*tx_shaping_filter,symbols_x8);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Channel
if LARGE_DOPPLER == 1
    frequency_offset = 800;
else
    frequency_offset = 400;
end

if (NO_DOPP == 1)
    tx_doppler = tx_qpsk;
    frequency_offset = 0;
else
    tx_doppler = tx_qpsk.*exp(j*2*pi.*frequency_offset/fs.*(0:length(tx_qpsk)-1)+j*phase_offset);
end

% Noise
SNR = 10^(SNRdB/10);
signal_power = tx_doppler * tx_doppler'/length(tx_doppler);
noise_power = signal_power/ SNR *sps;
noise_voltage = sqrt(noise_power/2);
noise = noise_voltage*[1 j]*randn(2,length(tx_doppler));
tx_doppler_noise = tx_doppler+noise;


subplot(221)
plot(real(symbols),imag(symbols),'bx','LineWidth',2)
hold on;
plot(real(symbols_x8),imag(symbols_x8),'ro','LineWidth',2)
axis([-1.5 1.5 -1.5 1.5])
grid on;
legend('Symbols','Upsampled')
title('Name','QAM 8-SPS')


subplot(222)
plot(real(tx_doppler_noise),imag(tx_doppler_noise),'bx','LineWidth',2)
axis([-3 3 -3 3])
grid on;
title('After Channel')

subplot(2,2,[3,4])
NFFT = 16384;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_qpsk,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(tx_doppler_noise,NFFT)))),'LineWidth',2);
legend('tx sent','tx after channel')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% ADC ( Down sample at receiver )
if(adc_sps ~= sps)
    tx_doppler_noise = downsample(tx_doppler_noise,sps/adc_sps);
    sps = adc_sps;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Save Signal for HDL Test Bench (verilog)

bits = 10; % want to see AGC gain it up
scale = (2^(bits-1)-1);
to_tb = floor((tx_doppler_noise./max([max(abs(real(tx_doppler_noise))) max(abs(imag(tx_doppler_noise)))])).*scale);
fid=fopen('tb_rx_input.dat','w');
for i = 1:length(to_tb)
    fwrite(fid,[real(to_tb(i)) imag(to_tb(i))],'short','ieee-be');
end
fclose(fid);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Matching SRRC Filter
rx_srrc_filter=rcosine(1,sps,'sqrt',rolloff);

if (NO_RX_SRRC)
    rx_signal = tx_doppler_noise;
else
    rx_signal = conv(sps*rx_srrc_filter,tx_doppler_noise);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Generate matched filter
% Notes: I see three options: 

mf = upsample(symbols(1:64),sps); % (1)
b = flipud(mf');

y = filter(b,1,symbols_x8);

y2 = filter(b,1,rx_signal);
save 'mf.mat' b -mat

figure('Name', 'Preamble Convolution')
subplot(211)
plot(abs(y))
subplot(212)
plot(abs(y2))

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Do Frequency Correction
[amp,loc] = max(abs(real(y2))+abs(imag(y2))); %% Do we need to do magnitude? 

N = 32*sps; % length preamble * SPS (256)
N1 = loc-(2*N);
N2 = N1 + N;

Z2 = mean(conj(rx_signal(N1+(0:N-1))) .* rx_signal(N2+(0:N-1)));
fo = atan2(imag(Z2),real(Z2))./(2*pi*N);

fprintf("expected frequency correction value: %d\n",frequency_offset/fs)
fprintf("calulated frequency correction value: %d\n",fo)

f_expect = exp(-1i*2*pi*frequency_offset/fs*(0:length(rx_signal)-1));
f_correct = exp(-1i*2*pi*fo*(0:length(rx_signal)-1));

rx_f_expected = rx_signal .* f_expect;
rx_f_corrected = rx_signal .* f_correct;

figure('Name','Coarse Frequency Correction')
NFFT = 16384;
subplot(2,2,[1 2])
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_signal,NFFT)))),'LineWidth',2);
hold on
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(rx_f_corrected,NFFT)))),'r','LineWidth',2);
legend('rx','rx corrected')
ylim([0 100])

subplot(223)
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(f_correct,NFFT)))),'k--','LineWidth',2);
hold on;
plot((-0.5:1/NFFT:0.5-1/NFFT)*sps,20*log10(abs(fftshift(fft(f_expect,NFFT)))),'r','LineWidth',2);
legend('f correction','f expected')
ylim([0 100])
xlim([-0.025 0.025])

subplot(224)
title('FLL OUT')
plot(real(rx_f_corrected),imag(rx_f_corrected),'bx','LineWidth',2)
axis('square')

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
eta=10*eta;
k_i_t= (4*theta_0*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);
k_p_t= (4*eta*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);
int_t=0.0;
accum_t=1;

rx_idx = 1;
idx = 1;
odx = 1;
ssel= 0;
OPTION = 0;
while idx < (length(rx_f_corrected))-sps 
    fbsel=floor(accum_t);            % point to a coefficient set

    for s=0:sps-1
        % select correct samples to operate on
        reg_t=[rx_f_corrected(idx+s) reg_t(1:reg_len-1)];
    
        % Now lets filter
        y_t(s+1) =reg_t*h_rx_poly(fbsel,:)'; % MF output time sample
        dy_t(s+1)=reg_t*dh_rx_poly(fbsel,:)';% dMF output time sample 
        rx_tec(rx_idx)=y_t(s+1);            % save MF output sample
        %rx_idx = rx_idx + 1;
    end

    if(OPTION == 0)% Option 0
        det_re_t=real(y_t(1))*real(dy_t(1));   % y*y_dot product (timing error)
        det_im_t=imag(y_t(1))*imag(dy_t(1));   % y*y_dot product (timing error)
        det_t = (det_re_t + det_im_t)/2;
    else% option 1
        det_t=real(y_t(1))*real(dy_t(1));   % y*y_dot product (timing error)
    end

    % Loop Filter
    int_t=int_t+k_i_t*det_t; 
    sm_t =int_t+k_p_t*det_t; 
    accum_t_sv(odx)=accum_t;                 
    accum_t=accum_t+sm_t; 

    % Run beyond last filter so wrap around and move to next sample
    if accum_t > nfilts+1        
       accum_t = accum_t-nfilts;
    end

    % Gone below 1 wrap around to previous sample
    if accum_t < 1                   
        accum_t = accum_t+nfilts;
    end

    rx_idx = rx_idx + 1;
    idx = idx + sps;
    odx = odx + 1; % increment symbol clock
end

figure('Name','Timing Error Correction')
subplot(211)
plot(accum_t_sv)
hold on
plot(floor(accum_t_sv),'r')
hold off
grid on
%axis([0 2000 0 32])
title('Timing Loop Phase Accumulator and Pointer')

subplot(223)
hist(real(rx_tec))

subplot(224)
offset = 32;
plot(real(rx_tec(offset:end)),imag(rx_tec(offset:end)),'bo')
hold off
grid on
axis('square')
title('Constellation Diagram')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Phase Lock Loop

theta_0= 2*pi/500;
eta=sqrt(2)/2;
eta=0.15*eta;

k_i_ph= (4*theta_0*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);
k_p_ph= (4*eta*theta_0)/(1+2*eta*theta_0+theta_0*theta_0);

int_ph=0.0;
accum_ph=0;
lp_flt=0;

odx = 1;
for idx=1:length(rx_tec)
    prod=rx_tec(idx)*exp(j*2*pi*accum_ph);
    rx_pll(idx)=prod;
    
    %if rem(idx,sps) == 1
        % Calculate error
        det_ph=sign(real(prod))+j*sign(imag(prod));
        phs_err=angle(det_ph*conj(prod))/(2*pi);
        phs_err_sv(odx)=phs_err;
    
        %Loop filter
        int_ph=int_ph+k_i_ph*phs_err;
        lp_flt=int_ph+k_p_ph*phs_err;
        odx = odx + 1;
    %end
    accum_ph_sv(idx)=accum_ph;
    accum_ph=accum_ph+lp_flt;
end

figure('Name','Phase Lock Loop')
subplot(211)
plot(phs_err_sv(1:500))
grid on
title('Phase Lock Loop Phase Error')
   
subplot(223)
hist(real(rx_pll))

subplot(224)
plot(real(rx_tec(22:end)),imag(rx_tec(22:end)),'bo')
hold on
plot(real(rx_pll(1:sps:end)),imag(rx_pll(1:sps:end)),'ro')
grid on
axis([-1.5 1.5 -1.5 1.5])
title('Constellation Diagram')
legend('Before PLL','After PLL')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Decode Data

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')
%