%% Main function
function ini = v2x_baseband_init()
    % Data frame period
    ini.T = 0.01;

    % Packet params
    ini.pkt = get_pkt_params();

    % Scrambler params
    ini.scramb = get_scramb_params();

    % Encoder params
    extra_bits = 8;
    ini.enc = get_enc_params(ini.pkt.data_len, extra_bits);
    
    % Preamble params
    ini.preamble = get_preamble_params();

    % TX output/RX input frame length
    ini.intfc = get_intfc_params(ini.preamble.bit.len, ini.enc.len);
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
    f_audio   = 4e3; % 4 KHz audio format
    t_audio   = 0.01; % 0.01 second of captured data
    audio_bits = 16; % 1 channel at 16 bits per audio sample
    audio_len = round(f_audio*t_audio*audio_bits);
end

function params = get_pkt_params()
    % Data packet length (each var in terms of # bits)
    params.info_len = get_info_len();
    params.audio_len = get_audio_len();
    params.data_len = params.info_len + params.audio_len;
end

function params = get_scramb_params()
    % Choosing order 16, (2^16 = 65535) > (144 + 44100)
    % https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Example_polynomials_for_maximal_LFSRs
    params.seed = 0; % zeros(1, 16);
    params.tap = 'z^16 + z^15 + z^13 + z^4 + 1';
end

function params = get_enc_params(data_len, extra_bits)
    % Default MATLAB N, K for RS Encoder/Decoder
    params.extra_bits = 8;
    params.n = 7;
    params.k = 3;
    params.len = round((params.n/params.k) * (data_len + extra_bits));
end

function params = get_preamble_params()
    % Bit version
    params.bit.orig_seq = pngen(6, 32)'; % 32 bit sequence
    params.bit.dbl_seq = repmat(params.bit.orig_seq, 2, 1); % Doubled sequence (32 x 2 bit sequence)
    params.bit.seq = repelem(params.bit.dbl_seq, 2); % Repeated sequence for QPSK IQ
    params.bit.len = length(params.bit.seq);

    % QPSK version
    qpskmod = comm.QPSKModulator('BitInput', true);
    params.qpsk.seq = qpskmod(params.bit.seq);
    params.qpsk.len = length(params.qpsk.seq);

    % Threshold
    params.thresh = 0.7 * params.qpsk.len;
end

function params = get_intfc_params(preamble_len, enc_out_len)
    params.tx_bit_len = preamble_len + enc_out_len;
    params.tx_sym_len = params.tx_bit_len/2;
    params.rx_bit_len = params.tx_bit_len;
end
