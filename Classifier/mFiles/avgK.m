function [avgKeegData] = avgK (k, eegData)
j=1
avgKeegData=zeroes(6,14,160);
for l=[1:6]
    l
    for m=[1:14] 
        m;
        j*(k+1)
        while j*(k+1)-1<769    
            avgKeegData(j,m,l)=mean(eegData(j*k:j*(k+1)-1,m,l));
            j=j+1
        end
    end
end

