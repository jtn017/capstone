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
function params = get_agc_params()
    params.step_size = 0.01;
    params.desired_out_pwr = 1;
    params.avg_len = 100;
    params.max_gain_dB = 0;
end

function params = get_srrc_params()
    params.rolloff = 0.5;
    params.filt_span = 6;
    params.sps = 8;
    params.decim = 1;
    params.lin_amp = 1;
    params.shift = -10;
end

function params = get_sym_timing_recov_params()
    params.damping_fact = 1;
    params.norm_loop_bw = 0.01;
    params.detector_gain = 2.7;
end

function params = get_freq_params()
    % Doppler calc
    fc = 2.4e9; % Hz
    max_v = 89.408; % m/s
    c = 3e8; % m/s
    fd = (1 + max_v/c) * fc;
    max_fo = fd - fc;

    % Return output
    params.fc = fc;
    params.fd = fd;
    params.max_fo = max_fo;
end