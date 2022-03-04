%% Waveform Description
close all
clearvars

% Good example:
% openExample('comm/QPSKTransmitterAndReceiverSimulinkExample')

%% General params
ini = v2x_baseband_init();
seed = 123;
Ts = ini.T;

%% Baseband params
bb_ini = v2x_baseband_init();

%% Simulation
sim_name = 'v2x_baseband_tb';
Tsim = Ts * 5;
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim(sim_name, Tsim);
