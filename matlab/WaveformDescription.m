close all;
clear all;
clc;

DO_BARKER_AC = 0;

%% Capstone V2X Motorcycle HUD
% WaveformDescriptions.m
%
%
saveplots = 1
barker_13 =[1 1 1 1 1 -1 -1 1 1 -1 1 -1 1];

%% Barker Code Autocorrelation Example
if(DO_BARKER_AC)
    ac = xcorr(barker_13,barker_13);
    ac_db = 20*log10(abs(ac)); 
    figure()
    plot(ac_db)
    ylim([-10 40])
    ylabel('Auorrelation (dB)');
    xlabel('n shift');
    title('Barker Autocorrelation Example');
end
%% For RFSOC TX Message
barker_13 =[1 1 1 1 1 -1 -1 1 1 -1 1 -1 1];
scrambler = repelem(barker_13,12);

preamble = zeros(1,13);
name = zeros(1,32);
lat = zeros(1,32); 
long = zeros(1,32); 
speed = zeros(1,7);


name = reshape(dec2bin(uint8('TEST'),8),1,32);%Identifier (name): 32 bits (4 ASCII Characters)
lat = dec2bin(123456789,32);% Latitude: 32bit float
long = dec2bin(123456789*2,32);% Longitude: 32 bit float
speed = dec2bin(75,7);%Speed (mph): 7 bits (127 max) mph
direction = dec2bin(4,8);%Directions: 8 bits (left, right, straight, u-turn, arrived) 
distance = dec2bin(3000,32);%Distance to next step: 32 bits (float)
data_packet = [name, lat, long, speed, direction, distance];
data_packet = data_packet - '0'; % get 0's and 1's
msg = [preamble data_packet]; % add preamble

scramdata = xor(msg, scrambler);
tx_data = scramdata*2-1;
n = 1:length(tx_data);

% Autocorrelation
ac = xcorr(tx_data,tx_data);
ac_db = 20*log10(abs(ac)); % 
plt1 = figure();
subplot(2,1,1)
plot(n,tx_data,'linewidth',1)
ylim([-1.2 1.2])
ylabel('Amplitude')
xlabel('n')
title('Radar Tx Msg Time Domain Plot');
subplot(2,1,2)
plot(ac_db)
ylim([20 45])
ylabel('Autoorrelation (dB)');
xlabel('n shift');
title('Radar Tx Msg Autocorrelation');
save_plot(plt1,'Radar TX Data Packet',saveplots);

%% Do Channel

