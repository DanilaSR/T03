function [outputArg1] = shift(inputArg1)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

outputArg1 = inputArg1;

outputArg1(1) = inputArg1(512);
for i=2:512
    outputArg1(i) = inputArg1(i-1);
end

end