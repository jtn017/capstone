%% AGC LOG BASED
%  Author: Ryan Hiser
%  Project: Capstone V2X Motorcycle HUD
%  Filename: agc.m
%
%  Description: Log Based AGC Script for HLS Verification

close all;
clear all;

set(0,'DefaultFigureWindowStyle','docked')

%% Initial Version of AGC Code (based on fred harris description)
y = cos(2*pi*(0:399)*0.25);
y1 = [y 0.1*y y 2*y y]; % vary amplitude

IQ = hilbert(y1);
InPhase = real(IQ);
Quadrature = imag(IQ);

loggain = zeros(1,2001);
loggain(1) = 0;
mu = 0.125;
for nn = 1:2000
    scl(nn)= IQ(nn)*exp(loggain(nn));
    loggain(nn+1) = loggain(nn)- mu*log(abs(scl(nn)+0.1));
end

h1 = figure('Name', 'Initial Non-optimized AGC');
subplot(311)
plot(InPhase, 'b','linewidth',2);
hold on;
plot(Quadrature, 'r','linewidth',2);
ylabel('Amplitude');xlabel('Samples');
title('INPUT - Matlab Log Based AGC')
subplot(312)
plot(real(scl), 'b','linewidth',2);
hold on;
plot(imag(scl), 'r','linewidth',2);
ylabel('Amplitude');xlabel('Samples');
title('OUTPUT - Matlab Log Based AGC')

subplot(313)
plot(loggain(1:2000), 'b','linewidth',2);
ylabel('Gain');xlabel('Samples');
title('GAIN - Matlab Log Based AGC')

%% Save input data for HLS Testbench
res = saveIQdat(IQ,12,'agc_input.dat');

%% Open and read file
test = csvread('../agc/solution1/csim/build/agc_output.csv');
org_I = test(:,1);
org_Q = test(:,2);
hls_I = test(:,3);
hls_Q = test(:,4);
gain = test(:,5);

h2 = figure('Name','HLS Test Bench Results');
ax1 = subplot(311);
plot(org_I,'b','linewidth',2);
hold on;
plot(org_Q,'r','linewidth',2);
ylabel('Amplitude');xlabel('Samples');
title('HLS Results for Log Based AGC')
title('INPUT - HLS Log Based AGC')

ax2 = subplot(312);
plot(hls_I,'b','linewidth',2);
hold on;
plot(hls_Q,'r','linewidth',2);
ylabel('Amplitude');xlabel('Samples');
title('OUTPUT - HLS Log Based AGC')

ax3 = subplot(313);
plot(gain)
linkaxes([ax1 ax2 ax3],'x');
ylabel('Gain');xlabel('Samples');
title('GAIN - HLS Log Based AGC')

%% Saveplots
saveplot(h1,'MAT_Verficiation',1);
saveplot(h2,'HLS_Verification',1);

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')