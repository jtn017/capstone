%% AGC Cordic Approach
%  Author: Ryan Hiser
%  Project: Capstone V2X Motorcycle HUD
%  Filename: phnorm.m
%
%  Description: 
close all;
clear all;
clc;
%% Generate Test Data
y = cos(2*pi*(0:399)*0.25)*.4;
y1 = [y 0.1*y y 2*y 0.4*y]; % vary amplitude

IQ = hilbert(y1);
InPhase = real(IQ);
Quadrature = imag(IQ);

%% Do matlab implementation
phase = atan2(Quadrature,InPhase);
y1norm = exp(1i*phase);

%% Plot Results
h1 = figure();
subplot(211)
plot(InPhase)
hold on;
plot(Quadrature)
title('INPUT - MATLAB CORDIC AGC')
ylabel('Amplitude');xlabel('Samples');

subplot(212)
plot(real(y1norm))
hold on;
plot(imag(y1norm))
ylim([-1.2 1.2])
title('OUTPUT - HLS CORDIC AGC')
ylabel('Amplitude');xlabel('Samples');

%% Save Test Data for HLS testbench
res = saveIQdat_f(IQ,'norm_input.dat');

%% Read Results of HLS testbench
test = csvread('../normalizer/solution1/csim/build/norm_output.csv');
org_I = test(:,1);
org_Q = test(:,2);
hls_I = test(:,3);
hls_Q = test(:,4);

h2 = figure('Name','HLS Test Bench Results');
ax1 = subplot(211);
plot(org_I,'b','linewidth',2);
hold on;
plot(org_Q,'r','linewidth',2);
title('INPUT - HLS CORDIC AGC')
ylabel('Amplitude');xlabel('Samples');

ax2 = subplot(212);
plot(hls_I,'b','linewidth',2);
hold on;
plot(hls_Q,'r','linewidth',2);
title('OUTPUT - HLS CORDIC AGC')
ylabel('Amplitude');xlabel('Samples');
ylim([-1.2 1.2])
linkaxes([ax1 ax2],'x');

%% Saveplots
saveplot(h1,'MAT_Verficiation',1);
saveplot(h2,'HLS_Verification',1);

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')