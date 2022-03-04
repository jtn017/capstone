%% Waveform Description
close all
clearvars

% Good example:
% openExample('comm/QPSKTransmitterAndReceiverSimulinkExample')

%% Add frame synchronizer
example_comm_path = fullfile(matlabroot, 'examples', 'comm', 'main');
addpath(example_comm_path)

%% General params
% Parameters
ini = v2x_baseband_init();
seed = 123;
Ts = ini.T;

%% Baseband params
bb_ini = v2x_baseband_init();
mod_ini = v2x_modem_init();

%% Channel params
phi = 20; % Phase offset (deg)
f0 = 500; % Frequency offset (Hz), none in this sim because no carrier freq

SNR_dB = 20;
% M = 2; % Bits per symbol
% EbN0_dB = 10; % Eb/N0 in dB
% sym_per = 1/mod_ini.Fc;

%% Simulation
Tsim = Ts * 3;
sim_name = 'v2x_modem_tb';
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim(sim_name, Tsim);
