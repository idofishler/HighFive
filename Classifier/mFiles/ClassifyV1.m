function [isEscape,dat] = ClassifyV1 (data)
%data 400 samples X 14 channels

%Clasifier settings
binSize = 5;
stdLim = 3;
stdSampleSize = 20;

%Step 1: bin
bd = binData(data,binSize);

%Step 2: diff
dd = diffData(bd);

%Step 3: Average over all electrodes
d = mean (dd,2);

arm = std(d(1:stdSampleSize));

%alarms = sum(d < -arm*stdLim);
%isEscape = alarms>0;
isEscape = d(end) < -arm*stdLim;
dat = d(end);