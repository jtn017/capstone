%% Waveform Description
close all
clearvars

% Good example:
% openExample('comm/QPSKTransmitterAndReceiverSimulinkExample')

%% Baseband params
bb_ini = v2x_baseband_init();

%% Simulation
% Parameters
sim_name = 'v2x_baseband_tb';
seed = 123;
Ts = 1;
Tsim = Ts * 5;

% Run sim
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim(sim_name, Tsim);
