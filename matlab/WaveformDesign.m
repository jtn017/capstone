clc
% Capstone 
% Ryan Hiser, Jorge Pacheco, Jacob Nguyen
%

MAX_RANGE = 0.4; % Maximum radar distance: 0.4km (0.25mi)
MAX_SPEED = 200; % Maximum vehicle speed: 200km/h (124MPH)

fprintf('---------- Chosen Requirements ----------\n');
fprintf('Maximum Range: %f km   \n',MAX_RANGE);
fprintf('Maximum Speed: %f km/h \n',MAX_SPEED);

c = 3e8; % Speed of light (m/s)

%% Variable to select

tau = 4e-9; % length of transmitted pulse (smallest pulse i.e. chip)

fprintf('\n---------- Chosen Variable Values -------\n');
fprintf('Pulse Width (tau): %d ns\n',tau*10^9);
fprintf('\n---------- Calculated Values ------------\n');

%% Maximum unambiguous range
% https://www.radartutorial.eu/01.basics/Maximum%20Unambiguous%20Range.en.html
% MAX_RANGE = c_(PRT-tau)/2
 
PRT = tau + ((2*MAX_RANGE*100)/c); % Pulse Repetition time
PRF = 1/PRT; % Pulse Repetition Frequency
fprintf('Pulse Repetition Time (PRT): %f us\n',PRT*10^6);
fprintf('Pulse Repetition Frequency (PRF): %f kHz\n',PRF/1000);

%% Range Resolution
delta_R = 0.5*tau*c; 
fprintf('Range Resolution: %f m\n',delta_R);

B_HF = 1/tau; % Bandwith increases with decreasing pulse width

%% Slant Range
% https://www.radartutorial.eu/01.basics/Distance-determination.en.html
t = 0; % measured runtime
R = 0; % slant range

R = (c*t)/2;


%% Notes
% Still need to consider:
    % RCS (final frequency dependent)
    % Maximum Doppler
    % Minimum Range (skin delay) (believe this is related to tau plus some x.
    % Dwell time (hits per scan) we won't be scanning