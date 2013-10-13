trial=3;
avg=mean(eegData(1:20,[4,11],trial));
nData=squeeze(eegData(:,[4,11],trial));
nData(:,1) = nData(:,1)-avg(1);
nData(:,2) = nData(:,2)-avg(2);
figure(2); plot(t,nData);