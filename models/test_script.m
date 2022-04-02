fileID = fopen('nine.bin', 'w');
fwrite(fileID,[1:9]);
fclose(fileID);