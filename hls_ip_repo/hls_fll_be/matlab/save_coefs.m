%% Save Coefs
%  Author: Ryan Hiser
%  Project: Capstone V2X Motorcycle HUD
%  Filename: save_coefs.m
%
%  Description: Saves Coeficients in a desired format to a file.

function save_coefs(fid,coef_name,x)

fprintf(fid,"const lut_t %s[%d] = {",coef_name,length(x));
for i = 1:length(x)
    if(i == length(x))
        fprintf(fid,"%0.16f};\n",x(i));
    else
        fprintf(fid,"%0.16f,",x(i));
    end
end

end

