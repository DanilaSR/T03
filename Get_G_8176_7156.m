function G = Get_G_8176_7156()

filename = "G.txt";

fid = fopen(filename);
data = fscanf(fid, '%1X');
fclose(fid);

C = dec2bin(data) - '0';
D = cell(length(data), 1);

for i = 1:length(data)
    D{i} = C(i,:)';
end

E = cell2mat(D);

B = cell(14,2);

for j = 1:14
    sz = 512;
    idx = j*2;
    
    firstRow = E(2+sz*(idx-2):sz*(idx-1))';
    B{j,1} = circulant(firstRow);
    
    firstRow = E(2+sz*(idx-1):sz*(idx))';
    B{j,2} = circulant(firstRow);
end

B = cell2mat(B);

G = [eye(length(B)) B];

end