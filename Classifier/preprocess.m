load('training sets/BrainHackDay1_Trial2-dataSet.mat');
pt = t < 0 & t > -2;
figure;
for i = 1:size(eegData, 3)
eegData1 = squeeze(eegData(:,:,i));
save(sprintf('BrainHackDay1_Test1_Trial%d', i), 'eegData1', '-ascii')
eegData2 = zscore(eegData1(pt,:));
t2 = t(pt);
%eegData3 = bsxfun(@minus, eegData2, mean(eegData2,2));
eegData3 = eegData2(:,1:7) - eegData2(:,14:-1:8);
subplot(3,1,1)
plot(t, eegData1);
subplot(3,1,2)
plot(t2, eegData2);
subplot(3,1,3)
plot(t2, eegData3);
keyboard
end