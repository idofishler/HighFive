function [binedData] = binData(eegData,sizeOfBins)
binedData=zeros(size(eegData,1)/sizeOfBins,size(eegData,2),size(eegData,3));
for trial=1:size(eegData,3)
    for chanel=1:size(eegData,2)
        binedData(:,chanel,trial)=mean(reshape(eegData(:,chanel,trial),sizeOfBins,[]));
    end
end
