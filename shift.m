function [output] = shift(input, t)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

output = input;

%outputArg1(1) = inputArg1(512);
for i=1:512
    output(i) = input(mod(i - t + 511, 512) + 1);
end

end