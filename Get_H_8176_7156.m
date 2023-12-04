function H = Get_H_8176_7156()

ones = load('Specification of Circulants LDPC (8176, 7156) Parity Check Matrix.txt');
A = cell(2,16);

for j = 1:2
    for i = 1:16
        firstRow = zeros(1, 511);
        one = ones(j*i, :) + 1;
        firstRow(one) = 1;
        A{j, i} = circulant(firstRow);
    end
end

H = cell2mat(A);
end

