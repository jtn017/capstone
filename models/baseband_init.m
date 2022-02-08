%% Main function
function ini = baseband_init()
    % Preamble
    ini.preamble = repmat(get_barker(13), 1, 3)';
    ini.preamble_len = length(ini.preamble);
    ini.detect_thresh = 0.7 * length(ini.preamble);
    
    % Data packet length (each var in terms of # bits)
    ini.info_len = get_info_len();
    ini.audio_len = get_audio_len();
    ini.data_len = ini.info_len + ini.audio_len;

    % Encoder params
    [ini.encoder_n, ini.encoder_k] = get_encoder_params();
    ini.encoder_out_len = (ini.encoder_n/ini.encoder_k) * ini.data_len;
    
    % Mapper params
    ini.demapper_out_len = ini.encoder_out_len/2; % 2 bits per symbol

    % TX output/RX input frame length
    ini.tx_frame_len = ini.preamble_len + ini.demapper_out_len;
    ini.rx_frame_len = ini.tx_frame_len;

    % Scrambler params
    [ini.scramb_seed, ini.scramb_tap] = get_scramb_params();
end

%% Helper Function
function barker = get_barker(n)
    if n == 2
        barker = [1 -1]; % [1 1]
    elseif n == 3
        barker = [+1 +1 -1];
    elseif n == 4
        barker = [+1 +1 -1 +1]; % [+1 +1 +1 -1]
    elseif n == 5
        barker = [+1 +1 +1 -1 +1];
    elseif n == 7
        barker = [+1 +1 +1 -1 -1 +1 -1];
    elseif n == 11
        barker = [+1 +1 +1 -1 -1 -1 +1 -1 -1 +1 -1];
    else % n == 13
        barker = [+1 +1 +1 +1 +1 -1 -1 +1 +1 -1 +1 -1 +1];
    end
end

function info_len = get_info_len()
    name_len  = 32;
    lat_len   = 32;
    long_len  = 32;
    speed_len = 8;
    dir_len   = 8;
    dist_len  = 32;
    info_len  = name_len + lat_len + long_len + speed_len + ...
               dir_len + dist_len;
end

function audio_len = get_audio_len()
    f_audio   = 44.1e3;
    t_audio   = 1; % 1 second of captured data
    audio_len = f_audio*t_audio;
end

function [scramb_seed, scramb_tap] = get_scramb_params()
    % Choosing order 16, (2^16 = 65535) > (144 + 44100)
    % https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Example_polynomials_for_maximal_LFSRs
    scramb_seed = 0; % zeros(1, 16);
    scramb_tap = 'z^16 + z^15 + z^13 + z^4 + 1';
end

function [n, k] = get_encoder_params()
    % Default MATLAB N, K for RS Encoder/Decoder
    n = 7;
    k = 3;
end