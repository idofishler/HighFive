function [isEscape,dat] = ClassifyV2 (data)
%data 400 samples X 14 channels

%Clasifier settings
binSize = 5;
stdLim = 3;
crNumOfChanels=2;
%Step 1: bin
bd = binData(data,binSize);

%Step 2: diff
dd = diffData(bd);

%Step 3: Average over all electrodes
%d = mean (dd,2);)

for i=1:14
    arm(i) = std(dd(1:20,i));
    chanellAns(i)=dd(end,i) < -arm(i)*stdLim;
end

isEscape = sum(chanellAns)>crNumOfChanels;

dat = dd(end,1); %just first channel