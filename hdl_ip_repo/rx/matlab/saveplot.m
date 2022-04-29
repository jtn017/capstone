% Jorge Pacheco
% Function to save plots to plots folder in current directory
function saveplot(h,fileName, savePlots)
    assert(nargin >= 2);

    [scriptDir,~,~] = fileparts(mfilename('fullpath'));

    outputDir = fullfile(scriptDir,'plots');

    if nargin < 3, savePlots = false; end

    if exist(outputDir,'dir') ~= 7
        mkdir(outputDir);
    end

    if isempty(fileName)
        fileName = 'unamed';
    end

    if savePlots
        print(h,fullfile(outputDir,fileName),'-dpng','-r300')
    end
end