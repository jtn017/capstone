%% Waveform Description
close all
clearvars

% Good example:
% openExample('comm/QPSKTransmitterAndReceiverSimulinkExample')

%% Baseband params
bb_ini = baseband_init();

%% Simulation
% Parameters
sim_name = 'baseband_tb';
seed = 123;
Ts = 1;
Tsim = Ts * 5;

% Run sim
open(sim_name);
sim(sim_name, Tsim);
