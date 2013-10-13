function [isEscape,dat] = ClassifyV3 (data)

lElectrods = [1 2 3 4 5 6 7];
rElectrods = [14 13 12 11 10 9 8];

threshold = 3;
crNumOfChanels=1;

%Settings
%for trial=1:nt
%data=squeeze(eegData(1:760,:,trial));
%data = mean(eegData(1:760,:,trial),3);
%t1=t(1:760);
binSize = 5;
stdLim = 3;
stdSampleSize = 30;

%Step 1: bin
bd = binData(data,binSize);
%bt = binData(t,binSize);

%Step 2: diff
dd = diffData(bd);
%dt = bt(2:end);

%plot(dt*1000,[ dd(:,[4,11]) (dd(:,4)-dd(:,11))]);
%std (dd(1:40,4)-dd(1:40,11))

d1 = dd(:,lElectrods)-dd(:,rElectrods);
nd1 = d1;
s = std(d1(1:stdSampleSize,:));
for i=1:length(s)
    nd1(:,i) = nd1(:,i) ./ s(i);
    nd1(end,i)
    chanelAns(i)= abs(nd1(end,i))>threshold;
end
 isEscape=sum(chanelAns)>crNumOfChanels;
 dat=1;
end


