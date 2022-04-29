%clear all
%close all
%clc

set(0,'DefaultFigureWindowStyle','docked')

%% Open and read file
test = csvread('../xcorr/solution1/csim/build/xcorr_output.csv');
org_I = test(:,1);
org_Q = test(:,2);
hls_I = test(:,3);
hls_Q = test(:,4);
corr = test(:,5);
maxv = test(:,6);
atan_i = test(:,7);
atan_q = test(:,8);
fo = atan2(atan_q,atan_i)./(2*pi*256); % 5.956497e-05

h1 = figure('Name','HLS Test Bench Results');
ax1 = subplot(311);
plot(org_I,'b','linewidth',2);
hold on;
plot(org_Q,'r','linewidth',2);

legend('I','Q','Location','southwest')
title('HLS INPUT','FontSize',16)
xlabel('n (samples)','FontSize',14)
ylabel('Amplitude','FontSize',14)

ax2 = subplot(312);
plot(hls_I,'b','linewidth',2);
hold on;
plot(hls_Q,'r','linewidth',2);
title('HLS OUTPUT (Delayed IQ)','FontSize',16)
xlabel('n (samples)','FontSize',14)
ylabel('Amplitude','FontSize',14)

% Plot Frequency Correction
f = exp(-1i*2*pi*fo(end)*(0:length(fo)-1))*1.5e4; % scale for easier to see plot
plot(real(f),'k-','linewidth',2)

legend('I','Q','I of estimated phasor','Location','southwest')

ax3 = subplot(313);
plot(corr)
hold on;
plot(maxv*8e5,'LineWidth',2)
linkaxes([ax1 ax2 ax3],'x');
title('HLS Output Cross Correlation','FontSize',16)
xlabel('n (samples)','FontSize',14)
ylabel('Amplitude','FontSize',14)

legend('INPUT into HLS','OUT','Location','southwest')

linkaxes([ax1,ax2,ax3],'x')

%% Expected Value
load 'mf.mat'
temp = org_I+1i*org_Q;
y2 = filter(b,1,temp);
[amp,loc] = max(abs(y2));
N = 32*8; % length preamble * SPS (256)
N1 = loc-(2*N);
N2 = N1 + N;
Z2 = mean(conj(temp(N1+(0:N-1))) .* temp(N2+(0:N-1)));
expected = atan2(imag(Z2),real(Z2))./(2*pi*N);

%% Do Frequency Correction

fprintf("expected frequency correction value: %d\n", expected);
fprintf("calulated frequency correction value: %d\n",fo(end));

%% Save Plot
saveplot(h1,'Hls_xcorr_results', 1);

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')