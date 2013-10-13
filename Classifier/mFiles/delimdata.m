function [rowNum]=delimdata(preEegData,timeFile)
 times=csvread(timeFile);
 times(:,4)=times(:,4)./1000;
 i=1;
 j=1;
while j<size(times,1) && i<size(preEegData,1);
    if preEegData(i,15:18)==times(j,:)
        rowNum(j)=i;
        j=j+1;
    end
    i=i+1;
end
        



