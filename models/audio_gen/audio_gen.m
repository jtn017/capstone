% Script description:
%
% Generate an audio file for OTA transmission, with the requirements:
% Fs = 44.1 kHz
% Single channel
% 16 bits per sample

%% Choose desired file
orig_wav_file = 'latinnova.wav';
orig_wav_info = audioinfo(orig_wav_file);

%% Read audio from desired file
% Choose amount of samples from file
num_sec = 60;
num_samp = orig_wav_info.SampleRate * num_sec;

if num_samp > orig_wav_info.TotalSamples
    num_samp = orig_wav_info.TotalSamples;
end

% Read audio file
[orig_y, orig_Fs] = audioread(orig_wav_file, [1, num_samp], 'native');

% Play audio
if 0
    sound(orig_y, orig_Fs);
end

%% Write audio in desired format
% Fs = 44.1 kHz, single channel, 16 bits per sample
new_wav_file = 'sample_audio.wav';
audiowrite(new_wav_file, orig_y(:, 1), orig_Fs, ...
           'Title', 'Latinnova', ...
           'Comment', 'From Ragnarok Online', ...
           'Artist', 'SoundTeMP', ...
           'BitsPerSample', 16);

% Also write to binary file
new_bin_fileID = fopen('sample_audio.bin', 'w');
fwrite(new_bin_fileID, orig_y(:, 1), 'int16');
fclose(new_bin_fileID);

% Read audio file
new_wav_info = audioinfo(new_wav_file);
[new_y, new_Fs] = audioread(new_wav_file, 'native');

% Play audio
if 0
    sound(new_y, new_Fs);
end
