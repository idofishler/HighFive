function [ eegData, answer,times ] = read_data( filename )
%READ_DATA Summary of this function goes here
%   Detailed explanation goes here
f = fopen(filename);
headers = fgetl(f);
next_line = fgetl(f);
i = 1;
while ischar(next_line)
    [values, count, errmsg, nextindex] = sscanf(next_line, '%f,');
    eegData(i,:) = values(2:15);
    answer(i) = values(20);
    %answer(i)= rand(1);
    times(i,:)= values(16:19);
    i = i+1;
    next_line = fgetl(f);
end
%t = [0:(size(eegData,1)-1)]/128;
fclose(f);
end

