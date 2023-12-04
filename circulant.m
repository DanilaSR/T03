function [output] = circulant(input)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    b1 = input;
    N = [];
    for i=1:64
        S = sprintf('%d*', hex2dec(b1(2*i-1:2*i)));
        N = [N, S];
    end
    
    B = split(N, '*');
    b = cellfun(@(x)str2double(x), B);
    
    res = [];
    for i=1:64
        res = [res, dec2bin(b(i), 8)];
    end
    
    B1 = zeros(512, 512);
    for i=1:512
        for j=1:512
            arg = shift(res, i - 1);
            B1(i, j) = str2num(arg(j));
        end
    end

    output = B1;

end