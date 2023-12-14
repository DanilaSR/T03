function H = Get_H_8176_7156()

ones = load("H.txt");

A = cell(2,16);

for j = 1:2
    for i = 1:16
        firstRow = zeros(1, 511);
        firstRow(1, ones(i + (j-1)*16, 1) + 1) = 1;
        firstRow(1, ones(i + (j-1)*16, 2) + 1) = 1;
        A{j, i} = circulant(firstRow);
    end
end

H = cell2mat(A);

end

