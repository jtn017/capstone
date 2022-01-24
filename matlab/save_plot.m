% By:   Jorge Pacheco
% Date: 11/01/2020
% simple function to save plots to disk
% Inputs:
%       h           - figure handle
%       fileName    - character array with the name of the plot
%       savePlots   - boolean variable. When true, function will save plot 
%                     to disk
function save_plot(h, fileName, savePlots)

    assert(nargin >= 2);
    
    [scriptDir, ~, ~] = fileparts(mfilename('fullpath'));
    
    outputDir = fullfile(scriptDir, 'plots');
    
    if nargin < 3, savePlots = false; end
    
    if exist(outputDir, 'dir') ~= 7
        mkdir(outputDir);
    end
    
    if isempty(fileName)
        fileName = 'unamed';
    end
    
    if savePlots
%         saveas(h, fullfile(outputDir,fileName), 'png');
        % save figure to disk at 'high resolution'
        print(h,fullfile(outputDir,fileName),'-dpng','-r300')
    end

end