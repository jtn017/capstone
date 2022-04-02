%% Main function
function ini = v2x_modem_init()
    % Baseband parameters
    ini.bb = v2x_baseband_init();

    % AGC params
    ini.agc = get_agc_params();

    % Pulse shaping params
    ini.srrc = get_srrc_params();

    % DAC/ADC data type (12 bitwidth, interface in SW is 16 bits where last
    % 4 LSBs get truncated)
    ini.intfc_dt = fixdt(1, 16, 14);

    % Symbol timing recovery params
    ini.str = get_sym_timing_recov_params();

    % Frequency params
    ini.freq = get_freq_params();
end

%% Helper Function
function agc_params = get_agc_params()
    agc_params.step_size = 0.01;
    agc_params.desired_out_pwr = 1;
    agc_params.avg_len = 100;
    agc_params.max_gain_dB = 0;
end

function srrc_params = get_srrc_params()
    srrc_params.rolloff = 0.5;
    srrc_params.filt_span = 10;
    srrc_params.sps = 8;
    srrc_params.decim = 1;
    srrc_params.lin_amp = 1;
    srrc_params.shift = -10;
end

function str_params = get_sym_timing_recov_params()
    str_params.damping_fact = 1;
    str_params.norm_loop_bw = 0.01;
    str_params.detector_gain = 2.7;
end

function freq_params = get_freq_params()
    % Doppler calc
    fc = 2.4e9; % Hz
    max_v = 89.408; % m/s
    c = 3e8; % m/s
    fd = (1 + max_v/c) * fc;
    max_fo = fd - fc;

    % Return output
    freq_params.fc = fc;
    freq_params.fd = fd;
    freq_params.max_fo = max_fo;
end