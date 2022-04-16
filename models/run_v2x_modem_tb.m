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
mod_dt = mod_ini.intfc_dt;
info_pkt_v1 = get_info_pkt_v1();
info_pkt_v2 = get_info_pkt_v2();

%% Channel params
phi = 20; % Phase offset (deg)
f0 = 500; % Frequency offset (Hz), none in this sim because no carrier freq
SNR_dB = 20;

%% Simulation
Tsim = Ts * 3;
sim_name = 'v2x_modem_tb';
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim_out = sim(sim_name, Tsim);

%% Save to CSV
save_to_bin = 1;

% V2X TX Baseband
if save_to_bin
    % TX Baseband
    v2x_tx_bb_in_fileID = fopen('main/data/v2x_tx_bb_in.bin', 'w');
    v2x_tx_bb_out_fileID = fopen('main/data/v2x_tx_bb_out.bin', 'w');
    v2x_tx_bb_in  = squeeze(sim_out.logsout.getElement('v2x_tx_bb_in').Values.Data);
    v2x_tx_bb_out = squeeze(sim_out.logsout.getElement('v2x_tx_bb_out').Values.Data);
    fwrite(v2x_tx_bb_in_fileID, v2x_tx_bb_in);
    fwrite(v2x_tx_bb_out_fileID, v2x_tx_bb_out);
    fclose(v2x_tx_bb_in_fileID);
    fclose(v2x_tx_bb_out_fileID);

    % TX Modulator
    v2x_tx_mod_out_real_fileID = fopen('main/data/v2x_tx_mod_out_real.bin', 'w');
    v2x_tx_mod_out_imag_fileID = fopen('main/data/v2x_tx_mod_out_imag.bin', 'w');
    v2x_tx_mod_out = squeeze(sim_out.logsout.getElement('v2x_tx_mod_out').Values.Data);
    fwrite(v2x_tx_mod_out_real_fileID, real(single(v2x_tx_mod_out)), 'single');
    fwrite(v2x_tx_mod_out_imag_fileID, imag(single(v2x_tx_mod_out)), 'single');
    fclose(v2x_tx_mod_out_real_fileID);
    fclose(v2x_tx_mod_out_imag_fileID);

    % RX Baseband
    v2x_rx_bb_in_fileID = fopen('main/data/v2x_rx_bb_in.bin', 'w');
    v2x_rx_bb_out_fileID = fopen('main/data/v2x_rx_bb_out.bin', 'w');
    v2x_rx_bb_in  = squeeze(sim_out.logsout.getElement('v2x_rx_bb_in').Values.Data);
    v2x_rx_bb_out = squeeze(sim_out.logsout.getElement('v2x_rx_bb_out').Values.Data);
    fwrite(v2x_rx_bb_in_fileID, v2x_rx_bb_in);
    fwrite(v2x_rx_bb_out_fileID, v2x_rx_bb_out);
    fclose(v2x_rx_bb_in_fileID);
    fclose(v2x_rx_bb_out_fileID);
end

%% Build script
build_src = 1;

if build_src
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

%% Helper functions
function info_pkt = get_info_pkt_v1()
    % Name: V2X!
    name = 'V2X!';
    name_bin_mat = dec2bin(name, 8) - '0';
    name_bin_mat2 = name_bin_mat.';
    name_bin = name_bin_mat2(:);

    % Location: UCSD
    pos.lat = single(32.880100);
    pos.lon = single(-117.234000);
    pos.lat_bin = (float_2_bin(pos.lat) - '0')';
    pos.lon_bin = (float_2_bin(pos.lon) - '0')';

    % Speed: 60
    speed = uint8(60);
    speed_bin = (dec2bin(speed, 8) - '0')';

    % Navigation: Directions = 3, Distance to next step = 5.3
    nav.dir = uint8(3);
    nav.dtns = single(5.3);
    nav.dir_bin = (dec2bin(nav.dir, 8) - '0')';
    nav.dtns_bin = (float_2_bin(nav.dtns) - '0')';

    % Concatenate array
    info_pkt = [name_bin; pos.lat_bin; pos.lon_bin; speed_bin; ...
                nav.dir_bin; nav.dtns_bin];
end

function info_pkt = get_info_pkt_v2()
    % Name: Home
    name = 'Home';
    name_bin_mat = dec2bin(name, 8) - '0';
    name_bin_mat2 = name_bin_mat.';
    name_bin = name_bin_mat2(:);

    % Location: San Jose, CA
    pos.lat = single(37.3382);
    pos.lon = single(-121.8863);
    pos.lat_bin = (float_2_bin(pos.lat) - '0')';
    pos.lon_bin = (float_2_bin(pos.lon) - '0')';

    % Speed: 45
    speed = uint8(45);
    speed_bin = (dec2bin(speed, 8) - '0')';

    % Navigation: Directions = 1, Distance to next step = 0.6789
    nav.dir = uint8(1);
    nav.dtns = single(0.6789);
    nav.dir_bin = (dec2bin(nav.dir, 8) - '0')';
    nav.dtns_bin = (float_2_bin(nav.dtns) - '0')';

    % Concatenate array
    info_pkt = [name_bin; pos.lat_bin; pos.lon_bin; speed_bin; ...
                nav.dir_bin; nav.dtns_bin];
end

function [bitstr] = float_2_bin(x)

    hex = num2hex(x);        % string of 16 hex digits for x
    dec = hex2dec(hex');     % decimal for each digit (1 per row)
    dec2 = [dec(7:8);dec(5:6); dec(3:4);dec(1:2)];
    bin = dec2bin(dec2,2);    % 4 binary digits per row
    bitstr = reshape(bin',[1 32]);  % string of 32 bits in order

end
