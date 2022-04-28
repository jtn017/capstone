%% Save IQ to .dat file
%  Author: Ryan Hiser
%  Project: Capstone V2X Motorcycle HUD
%  Filename: saveIQdat.m
%
%  Description: Saves IQ to .dat file
%

function data = saveIQdat(signal,bits,fileName)
    assert(nargin >= 2);

    [scriptDir,~,~] = fileparts(mfilename('fullpath'));
    outputDir = fullfile(scriptDir,'datafiles');

    if exist(outputDir,'dir') ~= 7
        mkdir(outputDir);
    end

    if isempty(fileName)
        fileName = 'unamed';
    end

    fid=fopen(fullfile(outputDir,fileName),'a');

    scale = (2^(bits-1)-1);
    rx_bpsk_sv = (signal./max([max(abs(real(signal))) max(abs(imag(signal)))])).*scale;
    re = real(rx_bpsk_sv);
    im = imag(rx_bpsk_sv);
    re = round(re);
    im = round(im);
    data = [re; im];
    data = reshape(data, 1, []);

    for i=1:length(rx_bpsk_sv)
        fwrite(fid,data(i),'short','native');
    end
    fid=fclose(fid);

end