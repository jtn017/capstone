clear all;
close all;

set(0,'DefaultFigureWindowStyle','docked')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Original Simulink filter

% ------------------------------- TX --------------------------------------
params = get_srrc_params()
tx_shaping_filter = rcosdesign(params.rolloff, params.filt_span, params.sps, 'sqrt');
fvtool(tx_shaping_filter,'Analysis','impulse')

% ------------------------------- RX --------------------------------------
nfilts = 32;
h_rx = rcosdesign(params.rolloff, params.filt_span, nfilts*params.sps, 'sqrt');
h_rx=[h_rx zeros(1,nfilts-mod(length(h_rx),nfilts))];  % zeros extending
dh_rx=conv([1 0 -1],h_rx(:)');                  % derivative matched filter
dh_rx=dh_rx(2:end-1);

h_rx_poly=reshape(h_rx,nfilts,numel(h_rx)/nfilts);   % 32 path polyphase MF
dh_rx_poly=reshape(dh_rx,nfilts,numel(dh_rx)/nfilts); % 32 path polyphase dMF 

reg_len = size(h_rx_poly,2);
reg_t = zeros(1,reg_len);

fvtool(dh_rx,'Analysis','impulse')
fvtool(h_rx,'Analysis','impulse')

save 'coefs.mat' h_rx_poly dh_rx_poly -mat

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Original Matlab filter (USED IN VITIS)

% ------------------------------- TX --------------------------------------
sps = 8
rolloff = 0.5;
tx_shaping_filter= rcosine(1,sps,'sqrt',rolloff);
tx_shaping_filter=tx_shaping_filter/sum(tx_shaping_filter);

fvtool(tx_shaping_filter,'Analysis','impulse')

% ------------------------------- RX --------------------------------------
nfilts = 32;
h_rx=rcosine(1,nfilts*sps,'sqrt',rolloff);             % Matched filter
h_rx=[h_rx zeros(1,nfilts-mod(length(h_rx),nfilts))];  % zeros extending
dh_rx=conv([1 0 -1],h_rx(:)');                  % derivative matched filter
dh_rx=dh_rx(2:end-1);

h_rx_poly=reshape(h_rx,nfilts,numel(h_rx)/nfilts);   % 32 path polyphase MF
dh_rx_poly=reshape(dh_rx,nfilts,numel(dh_rx)/nfilts); % 32 path polyphase dMF 

reg_len = size(h_rx_poly,2);
reg_t = zeros(1,reg_len);

fvtool(dh_rx,'Analysis','impulse')
fvtool(h_rx,'Analysis','impulse')

%% Cleanup
set(0,'DefaultFigureWindowStyle','normal')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function params = get_srrc_params()
    params.rolloff = 0.5;
    params.filt_span = 6;
    params.sps = 8;
    params.decim = 1;
    params.lin_amp = 1;
    params.shift = -10;
end

