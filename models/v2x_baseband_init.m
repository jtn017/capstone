%% Main function
function ini = v2x_baseband_init()
    % Packet params
    ini.pkt_params = get_pkt_params();

    % Scrambler params
    ini.scramb_params = get_scramb_params();

    % Encoder params
    ini.enc_params = get_enc_params(ini.pkt_params.data_len);
    
    % Mapper params
    ini.map_params = get_map_params(ini.enc_params.out_len);

    % Preamble params
    ini.preamble_params = get_preamble_params();

    % Guard Interval params
    ini.guard_params = get_guard_params();

    % TX output/RX input frame length
    ini.intfc_params = get_intfc_params(ini.preamble_params.len, ...
                                        ini.map_params.map_out_len, ...
                                        ini.guard_params.len);
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

function pkt_params = get_pkt_params()
    % Data packet length (each var in terms of # bits)
    pkt_params.info_len = get_info_len();
    pkt_params.audio_len = get_audio_len();
    pkt_params.data_len = pkt_params.info_len + pkt_params.audio_len;
end

function scramb_params = get_scramb_params()
    % Choosing order 16, (2^16 = 65535) > (144 + 44100)
    % https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Example_polynomials_for_maximal_LFSRs
    scramb_params.seed = 0; % zeros(1, 16);
    scramb_params.tap = 'z^16 + z^15 + z^13 + z^4 + 1';
end

function enc_params = get_enc_params(data_len)
    % Default MATLAB N, K for RS Encoder/Decoder
    enc_params.n = 7;
    enc_params.k = 3;
    enc_params.out_len = (enc_params.n/enc_params.k) * data_len;
end

function map_params = get_map_params(data_len)
    map_params.map_out_len = data_len/2; % 2 bits per symbol
end

function preamble_params = get_preamble_params()
    preamble_params.seq = repmat(get_barker(13), 1, 3)';
    preamble_params.len = length(preamble_params.seq);
    preamble_params.thresh = 0.7 * preamble_params.len;
end

function guard_params = get_guard_params()
    guard_params.seq = repmat(get_barker(7), 1, 3)';
    guard_params.len = length(guard_params.seq);
end


function intfc_params = get_intfc_params(preamble_len, map_out_len, guard_len)
    intfc_params.tx_frame_len = preamble_len + map_out_len + guard_len;
    intfc_params.rx_frame_len = intfc_params.tx_frame_len;
end
