close all
binSize = 10;

%Select data
trial = 1;
nt = 11;
t=(1:1000);
for trial=1:1
%data=goodTrial;
data = squeeze(eegData(:,:,trial));
size(data)
%Trim data
data = data(1:760,:);
t = t(1:760);


%Make classifier
bd = binData(data,binSize);
bt = binData(t,binSize);
dd = diffData(bd);
dt = bt(1:(end-1));

%Average over all electrodes
d = mean (dd,2);
%d = mean (dd(:,10:14),2);
arm = std(d(1:20));

%Classifier plot
f = (d<(-arm*3))*200;
figure (1)
subplot(nt/2,3,trial)
plot (dt*1000,[d f]);
ax = axis;
axis([-500 200 ax(3) ax(4)])
end