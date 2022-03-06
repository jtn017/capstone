% Jorge Pacheco

%% Configure Script
clc;
close all;
format longG
font_size = 12;
line_width = 2;
save_plots = false;

set(0, 'DefaultFigureWindowStyle', 'docked')
[script_dir, script_name, ~] = fileparts(mfilename('fullpath'));

%% Code
char_ascii = '46 MPH';

ascii = ascii_message(char_ascii)

%% Cleanup
format longG
set(0, 'DefaultFigureWindowStyle', 'normal')
fprintf('Done with %s\n',script_name);


function ascii_char = ascii_message(char_ascii)

L = length(char_ascii);
ascii_char = zeros(1, L);

for n = 1:L
   ascii_char(n) = char2ascii(char_ascii(n));
end

end

function a = char2ascii(c)

numbers = 48:1:57;
CAP = 65:1:90;
LOW = 97:1:122;

if (c - '0' + 1) >= 0 && (c - '0' + 1) < 9
    res = c - '0' + 1;
    a = numbers(res);
elseif (c == ' ')
    a = 32;
elseif ( (c - 'A' + 1) >= 0 && (c - 'Z' + 1) < 26  )
    res = c - 'A' + 1;
    a = CAP(res);
elseif ( (c - 'a' + 1) >= 0 && (c - 'z' + 1) < 26  )
    res = c - 'a' + 1;
    a = LOW(res);
else
    res = 0;
end

end