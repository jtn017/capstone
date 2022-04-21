%% Waveform Description
close all
clearvars

%% General params
ini = v2x_baseband_init();
seed = 123;
Ts = ini.T;

%% Frame params
num_frames = 10;
audio_pkts = get_audio_pkts();
audio_pkts = audio_pkts(:, 1:num_frames);
info_pkts = get_info_pkts();
info_pkts = info_pkts(:, 1:num_frames);

%% Simulation
Tsim = Ts * (num_frames - 0.5);
sim_name = 'v2x_data_tb';
open(sim_name);
set_param(sim_name, 'StopTime', num2str(Tsim))
sim_out = sim(sim_name, Tsim);

%% Save to CSV
v2x_data_fileID = fopen('v2x_data.bin', 'w');
v2x_data  = squeeze(sim_out.logsout.getElement('v2x_data').Values.Data);
fwrite(v2x_data_fileID, v2x_data);
fclose(v2x_data_fileID);

%% Final sample funcions
% Sample audio has 6000 frames, AKA 60 seconds of audio
% Mimic sample audio and create 6000 frames of data
function audio_pkts = get_audio_pkts()
    % Read file
    fileID = fopen('audio_gen/sample_audio.bin', 'r');
    audio_data = int16(fread(fileID, 'int16'));
    fclose(fileID);
    
    % Convert to binary matrix
    ini = v2x_baseband_init();
    audio_bin = dec2bin(audio_data, 16) - '0';
    audio_bin_vec = audio_bin';
    audio_bin_vec = audio_bin_vec(:);
    audio_pkts = reshape(audio_bin_vec, ini.pkt.audio_len, []);
end

function info_pkts = get_info_pkts()
    info_pkts = [];
    for n = 1:6000
        % Name: V2X!
        name = 'V2X!';
        name_bin_mat = dec2bin(name, 8) - '0';
        name_bin_mat2 = name_bin_mat.';
        name_bin = name_bin_mat2(:);
    
        % Location: UCSD, changing 0.6 over duration
        pos.lat = single(32.880100 - n*0.0001);
        pos.lon = single(-117.234000 + n*0.0001);
        pos.lat_bin = (matlab_f2b(pos.lat) - '0')';
        pos.lon_bin = (matlab_f2b(pos.lon) - '0')';
    
        % Speed: 60, changing 10 over duration
        speed = uint8(60 + floor(n/600));
        speed_bin = (dec2bin(speed, 8) - '0')';
    
        % Navigation: Directions = 3, Distance to next step = 5.3
        % Dir changes 1 over duration, Dist changes 3 over duration
        nav.dir = uint8(3 + floor(n/4000));
        nav.dtns = single(5.3 - floor(n/1800));
        nav.dir_bin = (dec2bin(nav.dir, 8) - '0')';
        nav.dtns_bin = (matlab_f2b(nav.dtns) - '0')';

        % Concatenate array
        info_pkt = [name_bin; pos.lat_bin; pos.lon_bin; speed_bin; ...
                    nav.dir_bin; nav.dtns_bin];
        info_pkts = [info_pkts, info_pkt];
    end
end

% Stores float into binary (technically correct, but gets saved in wrong
% order when running matlab's fwrite)
function [bitstr] = matlab_f2b(x)
    bitstr = dec2bin(typecast(x, 'uint32'), 32);
end

% Stores float into binary that "pre-corrects" matlab's behavior of writing
% floats in reverse short order. Currently breaks on some hex conversions
function [bitstr] = pre_correct_f2b(x)
    hex = num2hex(x);        % string of 16 hex digits for x
    dec = hex2dec(hex');     % decimal for each digit (1 per row)
    dec2 = [dec(7:8); dec(5:6); dec(3:4); dec(1:2)];
    bin = dec2bin(dec2,2);    % 4 binary digits per row
    bitstr = reshape(bin',[1 32]);  % string of 32 bits in order
end

