% Generate an audio file from binary, with the requirements:
% Fs = 44.1 kHz
% Single channel
% 16 bits per sample
function [] = bin_to_wav(filename)
    % Read from binary
    fileID = fopen(filename, 'rb');
    audio_data = int16(fread(fileID, 'int16'));
    fclose(fileID);

    % Write to wav file
    audiowrite('v2x_captured_audio.wav', audio_data, 4e3, ...
               'Title', 'V2X captured audio', ...
               'Comment', '4 kHz 1 channel, 16 bits per sample', ...
               'Artist', 'V2x Motorcycle HUD', ...
               'BitsPerSample', 16);
end

