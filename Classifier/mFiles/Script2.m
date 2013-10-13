
%Run clasifier over time 
%Prior to running the script load eegData
classifierLength = 400;
isZoom = false;
isPlotAlternative=false;

nt = size (eegData,3);
for trial=1:nt
    data = squeeze(eegData(:,:,trial));
    
    %Run the classifier for each datapoint
    classifierAnswer = zeros (size(data,1)-(classifierLength-1),1);
    classifierT = zeros (length(classifierAnswer),1);
    classifierValue = zeros (length(classifierAnswer),1);
    
    classifierAnswer_Alt = zeros (size(data,1)-(classifierLength-1),1);
    classifierValue_Alt = zeros (length(classifierAnswer),1);
    
    for i=1:length(classifierT)
        d = data(i:(i+classifierLength-1),:);
        classifierT(i) = t(i+classifierLength-1);
        [classifierAnswer(i),classifierValue(i)] = ClassifyV1(d);
        
        if isPlotAlternative
            [classifierAnswer_Alt(i),classifierValue_Alt(i)] = ClassifyV2(d);
        end
    end
    
    %Plot
    figure(1)
    subplot(nt/2,3,trial)
    plot (t*1000,data); 
    hold on
    plot (classifierT*1000,classifierAnswer*(max(data(:))-min(data(:)))+min(data(:)));
    if isPlotAlternative
        plot (classifierT*1000,classifierAnswer_Alt*(max(data(:))-min(data(:)))+min(data(:)),'r');
    end
    hold off
    if isZoom
        ax = axis;
        axis([-500 200 ax(3) ax(4)])
    end
        
    inds_true = find(classifierAnswer>10);
    
    figure(2)
    subplot(nt/2,3,trial)
    plot (classifierT*1000,classifierValue);
    if isPlotAlternative
        plot (classifierT*1000,classifierValue_Alt,'r');
    end
    hold on
    plot (classifierT*1000,classifierAnswer*max(abs(classifierValue)));
    if isPlotAlternative
        plot (classifierT*1000,classifierAnswer_Alt*max(abs(classifierValue_Alt)),'r');
    end
    hold off
    
    if isZoom
        ax = axis;
        axis([-500 200 ax(3) ax(4)])
    end
    %title(['t ' num2str(t(inds_true(1))*1000)]);
end