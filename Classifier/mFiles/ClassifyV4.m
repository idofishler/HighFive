function [isEscape,dat] = ClassifyV4 (data)
threshold = 5000;

%size(data,2);
nch = 14;
sumAll = 0;
for i=1:nch
    for j=1:i-1
        sumAll = sumAll + (data(i,end)-data(j,end))^2;
    end
end

isEscape = sumAll > threshold;
dat = sumAll;