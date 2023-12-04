function res = circulant( firstRow )

matrixSize = length(firstRow);

res = zeros(matrixSize);

for i = 1:matrixSize
    res(i, :) = circshift(firstRow, i - 1);
end

end


