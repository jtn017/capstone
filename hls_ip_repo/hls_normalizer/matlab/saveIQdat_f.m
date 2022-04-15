%% Save IQ to .dat file
%  Author: Ryan Hiser
%  Project: Capstone V2X Motorcycle HUD
%  Filename: saveIQdat.m
%
%  Description: Saves IQ to .dat file in float
%

function data = saveIQdat_f(signal,fileName)
    assert(nargin >= 2);

    [scriptDir,~,~] = fileparts(mfilename('fullpath'));
    outputDir = fullfile(scriptDir,'datafiles');

    if exist(outputDir,'dir') ~= 7
        mkdir(outputDir);
    end

    if isempty(fileName)
        fileName = 'unamed';
    end

    fid=fopen(fullfile(outputDir,fileName),'w');
    re = real(signal);
    im = imag(signal);
    data = [re; im];
    data = reshape(data, 1, []);

    %for i=1:length(data)
        fwrite(fid,data,'float','native');
    %end
    fid=fclose(fid);

end

