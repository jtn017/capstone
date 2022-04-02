%% Main function
function ini = v2x_baseband_init()
    % Data frame period
    ini.T = 0.01;

    % Packet params
    ini.pkt = get_pkt_params();

    % Scrambler params
    ini.scramb = get_scramb_params();

    % Encoder params
    ini.enc = get_enc_params(ini.pkt.data_len);
    
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
    f_audio   = 44.1e3;
    t_audio   = 0.01; % 0.01 second of captured data
    audio_bits = 16; % 16 bits -> 1 channel, 2 channels for stereo
    audio_len = f_audio*t_audio*audio_bits;
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
    enc_params.len = (enc_params.n/enc_params.k) * data_len;
end

function preamble_params = get_preamble_params()
    % Bit version
    preamble_params.bit.seq = repmat(pngen(6, 32), 1, 2)'; % 64 bit sequence
    preamble_params.bit.seq = repelem(preamble_params.bit.seq, 2); % Repeat bits for QPSK IQ
    preamble_params.bit.len = length(preamble_params.bit.seq);

    % QPSK version
    qpskmod = comm.QPSKModulator('BitInput', true);
    preamble_params.qpsk.seq = qpskmod(preamble_params.bit.seq);
    preamble_params.qpsk.len = length(preamble_params.qpsk.seq);

    % Threshold
    preamble_params.thresh = 0.7 * preamble_params.qpsk.len;
end

function intfc_params = get_intfc_params(preamble_len, enc_out_len)
    intfc_params.tx_bit_len = preamble_len + enc_out_len;
    intfc_params.tx_sym_len = intfc_params.tx_bit_len/2;
    intfc_params.rx_bit_len = intfc_params.tx_bit_len;
end
