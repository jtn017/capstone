%% Capstone V2X Motorcycle HUD
% WaveformDescriptions.m
close all
clearvars
set(0, 'DefaultFigureWindowStyle', 'docked')

%% Run
% Set seed
rng(123)

% Run
% barker_acs();
saveplots = 1;
use_barker = 0;
rfsoc_barker_msg(saveplots, 'Radar TX Data Packet (Barker)', use_barker);
use_barker = 1;
rfsoc_barker_msg(saveplots, 'Radar TX Data Packet (PN)', use_barker);

%% Cleanup
set(0, 'DefaultFigureWindowStyle', 'normal')

%% Barker Code Autocorrelation Example
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

function [] = barker_acs()
    % Barker code
    barker_13 = get_barker(13);

    % Autocorrelation
    ac = xcorr(barker_13, barker_13);
    ac_db = 20*log10(abs(ac));

    % Plot
    figure()
    plot(ac_db)
    ylim([-10 40])
    ylabel('Auorrelation (dB)');
    xlabel('n shift');
    title('Barker Autocorrelation Example');
end

%% RFSOC TX Message (Barker)
% https://www.commsp.ee.ic.ac.uk/~mrt102/projects/mls/MLS%20Theory.pdf

%% RFSOC TX Message (Barker)
function [] = rfsoc_barker_msg(saveplots, filename, use_barker)
    % Scrambling sequence
    if use_barker == 1
        % Barker code
        barker_13 = get_barker(13);
        scramb_seq = repelem(barker_13, 12);
    else
        % PN sequences from pngen are MLS
        % https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Example_polynomials_for_maximal_LFSRs
        scramb_seq = pngen(8, 13*12); % Order 8 -> period of 255
    end

    % Preamble
    preamble = zeros(1, 13);

    % Data
    name = reshape(dec2bin(uint8('TEST'), 8), 1, 32); % Identifier (name): 32 bits (4 ASCII Characters)
    lat = dec2bin(123456789, 32); % Latitude: 32bit float
    long = dec2bin(123456789*2, 32); % Longitude: 32 bit float
    speed = dec2bin(75, 7); % Speed (mph): 7 bits (127 max) mph
    direction = dec2bin(4, 8); % Directions: 8 bits (left, right, straight, u-turn, arrived) 
    distance = dec2bin(3000, 32); % Distance to next step: 32 bits (float)
    data_packet = [name, lat, long, speed, direction, distance];
    data_packet = data_packet - '0'; % get 0's and 1's
%     data_packet = randi([0 1], 1, 143);

    % Concat message
    msg = [preamble data_packet]; % add preamble

    % Scramble and modulate
    scramdata = xor(msg, scramb_seq);
    tx_data = scramdata*2-1;
    n = 1:length(tx_data);

    % Autocorrelation
    ac = xcorr(tx_data,tx_data);
    ac_db = 20*log10(abs(ac));

    % Plot
    plt1 = figure();
    subplot(2,1,1)
    plot(n,tx_data, 'linewidth', 1)
    ylim([-1.2 1.2])
    ylabel('Amplitude')
    xlabel('n')
    title('Radar Tx Msg Time Domain Plot');
    subplot(2, 1, 2)
    plot(ac_db)
    ylim([20 45])
    ylabel('Autoorrelation (dB)');
    xlabel('n shift');
    title('Radar Tx Msg Autocorrelation');
    save_plot(plt1, filename, saveplots);
end
