% Play wav file
function [] = play_wav(filename)
    [y, Fs] = audioread(filename);
    sound(y, Fs)
end

