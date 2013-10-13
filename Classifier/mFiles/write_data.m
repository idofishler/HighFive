function write_data( input, answer, filename_prefix)
%Write data exports data that was entered to classifier to log file format

if answer
    answer_txt = 'true';
else
    answer_txt = 'false';
end
f = fopen(sprintf('%s_%s.txt', filename_prefix, answer_txt), 'w');
for i=1:size(input,1)
    fprintf(f,'%f,',i);
    for j=1:size(input,2)
        fprintf(f,'%f,',input(i,j));
    end
    fprintf(f,'\n');
end
fclose(f);
