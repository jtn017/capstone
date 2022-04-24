%% Script
% Setup
close all
set(0, 'DefaultFigureWindowStyle', 'docked')
pll_input = load('pll_input.mat'); % From: save('pll_input.mat', 'rx_tec')
rx_tec = pll_input.rx_tec;

% Run
[rx_pll, phs_err] = pll(rx_tec);

% Save IO into VITIS testbench format
pll_test_vec(rx_tec, rx_pll);

% Plot
pll_plot(phs_err, rx_tec, rx_pll)

% Cleanup
set(0, 'DefaultFigureWindowStyle', 'normal')

%% PLL code
% Coefficients
function [Ki, Kp] = pll_coeff()
    % PLL coefficients
    theta_0= 2*pi/500;
    eta = sqrt(2)/2; 
    Ki = (4*theta_0*theta_0)/(1+2*eta*theta_0+theta_0*theta_0); % integral gain
    Kp = (4*eta*theta_0)/(1+2*eta*theta_0+theta_0*theta_0); % proportional gain
    fprintf("Ki = %f, Kp = %f\n", Ki, Kp);
end

% PLL
function [rx_pll, phs_err] = pll(rx_tec)
    % PLL coefficients
    [Ki, Kp] = pll_coeff();
    
    % PLL registers
    phs_int  = 0;
    acc_ph = 0;

    % PLL arrays
    rx_pll  = zeros(size(rx_tec));
    phs_err = zeros(size(rx_tec));

    % PLL
    for n = 1:length(rx_tec)
        % Signal mix
        prod      = rx_tec(n)*exp(1i*2*pi*acc_ph);
        rx_pll(n) = prod;
        
        % Phase error
        det_ph     = sign(real(prod))+1i*sign(imag(prod));
        phs_err(n) = angle(det_ph*conj(prod))/(2*pi);
    
        % Loop filter
        phs_prop  = Kp*phs_err(n);
        phs_int   = phs_int + Ki*phs_err(n);
        loop_filt = phs_prop + phs_int;
    
        % Phase accumulator
        acc_ph = acc_ph + loop_filt;
    end
end

% Plot
function [] = pll_plot(phs_err, rx_tec, rx_pll)
    % Time plot of phase error
    figure()
    subplot(2, 1, 1)
    plot(phs_err(1:300))
    grid on
    title('Phase Lock Loop Phase Error')
    
    % Constellation plot of PLL IO
    subplot(2, 1, 2)
    plot(rx_tec, '.')
    hold on
    plot(rx_pll(64:1:end), '.r') % Ingoring some of the preamble
    hold off
    grid on
    axis('square')
    axis([-1.5 1.5 -1.5 1.5])
    title('Constellation Diagram')
    legend('Before PLL', 'After PLL')
end

% VITIS testbench file gen
% Output file format:
% $(real) $(imag)
% $(real) $(imag)
% ...
% $(real) $(imag)
function [] = pll_test_vec(rx_tec, rx_pll)
    % Setup
    formatSpec = '% 8.6f % 8.6f\n';

    % Input file
    fileID = fopen('../input.gold.dat', 'w');
    temp_vec = zeros(1, 2*length(rx_tec));
    temp_vec(1:2:end) = real(rx_tec);
    temp_vec(2:2:end) = imag(rx_tec);
    fprintf(fileID, formatSpec, temp_vec);
    fclose(fileID);

    % Output file
    fileID = fopen('../output.gold.dat', 'w');
    temp_vec = zeros(1, 2*length(rx_pll));
    temp_vec(1:2:end) = real(rx_pll);
    temp_vec(2:2:end) = imag(rx_pll);
    fprintf(fileID, formatSpec, temp_vec);
    fclose(fileID);
end
