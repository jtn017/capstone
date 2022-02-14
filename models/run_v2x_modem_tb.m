%% Waveform Description
close all
clearvars

% Good example:
% openExample('comm/QPSKTransmitterAndReceiverSimulinkExample')

%% General params
% Parameters
seed = 123;
Ts = 1;

%% Baseband params
bb_ini = v2x_baseband_init();
mod_ini = v2x_modem_init();

%% Channel params
phi = 90; % Phase offset in degrees
f0 = 0; % Frequency offset

noise_var = 0.01;
% M = 2; % Bits per symbol
% EbN0_dB = 10; % Eb/N0 in dB
% sym_per = 1/mod_ini.Fc;

%% Simulation
Tsim = Ts * 6;
sim_name = 'v2x_modem_tb';
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim(sim_name, Tsim);
