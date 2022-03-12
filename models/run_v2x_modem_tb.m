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

SNR_dB = 10;
% M = 2; % Bits per symbol
% EbN0_dB = 10; % Eb/N0 in dB
% sym_per = 1/mod_ini.Fc;

%% Simulation
Tsim = Ts * 3;
sim_name = 'v2x_modem_tb';
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim_out = sim(sim_name, Tsim);

%% Save to CSV
save_to_csv = 0;

% V2X TX Baseband
if save_to_csv
    v2x_tx_bb_in  = squeeze(sim_out.logsout.getElement('v2x_tx_bb_in').Values.Data);
    v2x_tx_bb_out = squeeze(sim_out.logsout.getElement('v2x_tx_bb_out').Values.Data);
    writematrix(v2x_tx_bb_in,  'main/data/v2x_tx_bb_in.csv')
    writematrix(real(v2x_tx_bb_out), 'main/data/v2x_tx_bb_out_real.csv')
    writematrix(imag(v2x_tx_bb_out), 'main/data/v2x_tx_bb_out_imag.csv')
end

%% Build script
if 0
    % Models
    v2x_tx_bb_fp  = 'v2x_modem_tb/V2X_TX_Baseband';
    v2x_tx_mod_fp = 'v2x_modem_tb/V2X_TX_Modulator';
    v2x_rx_bb_fp  = 'v2x_modem_tb/V2X_RX_Baseband';

    % Generated code folder
    src_fp = 'src';
    if isfolder(src_fp)
        rmdir(src_fp, 's');
    end
    if ~isfolder(src_fp)
       mkdir(src_fp)
    end
    set_param(0, 'CodegenFolder', src_fp)

    % V2X TX Baseband
    slbuild(v2x_tx_bb_fp)
    slbuild(v2x_tx_mod_fp)
    slbuild(v2x_rx_bb_fp)
end