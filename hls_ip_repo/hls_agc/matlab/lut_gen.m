%% LUT GEN
%  Author: Ryan Hiser
%  Project: Capstone V2X Motorcycle HUD
%  Filename: lut_gen.m
%
%  Description: generates look up table for 2^x
%

clear all;
close all;
clc
format long
%% Do Quantization of 2^x 
ABS_MAX_RANGE = 8;
ARG_BITS = 8;
EXP_BITS = 8;
x = [-ABS_MAX_RANGE*(2^ARG_BITS):1:ABS_MAX_RANGE*(2^(ARG_BITS))-1]'/(2^ARG_BITS);
y = 2.^x;
y_quantized = round(y/max(y)*(2^EXP_BITS-1));

figure()
subplot(1,2,1)
plot(x,y, 'r');
subplot(1,2,2)
plot(x,y_quantized/2^4,'b+-');

%% Create LUT for HLS
% format for direct copy and paste to HLS project.

fid = fopen("../lut.h","w");
fprintf(fid,"#ifndef LUT_H_\n");
fprintf(fid,"#define LUT_H_\n\n");
fprintf(fid,"#define MIDDLE %d\n\n", round(length(x)/2));
fprintf(fid,"const lut_t EXP2_LUT[%d] = {",length(x));

for i = 1:length(x)
    LUT_sv(i) = y(i);
    if(i == length(x))
        fprintf(fid,"%0.16f};\n\n",y(i));
    else
        fprintf(fid,"%0.16f,",y(i));
    end
end

fprintf(fid,"#endif // LUT_H_\n");
fclose(fid);