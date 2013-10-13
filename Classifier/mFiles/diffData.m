function [diffData] = diffData(eegData)
diffData=zeros(size(eegData,1)-1,size(eegData,2),size(eegData,3));
for trial=1:size(eegData,3)
    for chanel=1:size(eegData,2)
        diffData(:,chanel,trial)=diff(eegData(:,chanel,trial));
    end
end
