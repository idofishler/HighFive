lElectrods = [1 2 3 4 5 6 7];
rElectrods = [14 13 12 11 10 9 8];

nt = 6

%Settings
for trial=1:nt
data=squeeze(eegData(1:760,:,trial));
%data = mean(eegData(1:760,:,trial),3);
t1=t(1:760);
binSize = 5;
stdLim = 3;

%Step 1: bin
bd = binData(data,binSize);
bt = binData(t1,binSize);

%Step 2: diff
dd = diffData(bd);
dt = bt(2:end);

%plot(dt*1000,[ dd(:,[4,11]) (dd(:,4)-dd(:,11))]);
%std (dd(1:40,4)-dd(1:40,11))

d1 = dd(:,lElectrods)-dd(:,rElectrods);
nd1 = d1;
s = std(d1(1:30,:));
for i=1:length(s)
    nd1(:,i) = nd1(:,i) ./ s(i);
end
subplot(3,2,trial)
plot (dt*1000,nd1(:,:));
title (['L-R diffs trial ' num2str(trial)]);
xlabel ('Time[ms]')
ylabel ('std');
ax = axis;
axis([-200 100 -15 15])
%axis([ax(1) ax(2) -10 10])
end
legend ('1','2','3','4','5','6','7')
%legend ('2','4','5')
