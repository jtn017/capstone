%% Main function
function ini = v2x_modem_init()
    % Pulse shaping params
    ini.srrc_params = get_srrc_params();

    % Up/Down conversion params
    ini.if_params = get_if_params();
end

%% Helper Function
function if_params = get_if_params()
    % Default MATLAB params for Digital Up-Converter
    if_params.interp_factor = 20;
    if_params.two_sided_bw = 200e3;
    if_params.passband_dB = 0.1;
    if_params.stopband_dB = 60;
    if_params.center_freq = 4e6;
end

function srrc_params = get_srrc_params()
    % Default MATLAB params for SRRC
    srrc_params.rolloff = 0.2;
    srrc_params.filt_span = 10;
    srrc_params.interp = 8;
    srrc_params.decim = 1;
    srrc_params.lin_amp = 1;
end