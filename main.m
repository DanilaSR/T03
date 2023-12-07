clear all; close all;

mex encode_.cpp
mex decode_.cpp

tic

% Формирование кода 
%Code(k,n)
G = Get_G_8176_7156();
H = Get_H_8176_7156();

% k - message bit
% n - codeword bit
% m - parity-check bit
[m, n] = size(H);
k = n - m;

% Сообщение
u = randi([0 1], 1, k);

% Кодирование
c = encode_(u, G);

% Добавляем искажения
% c(1) = 1;

% Декодирование
[y, iter] = decode_(c, H);

% Проверка ошибок
any(mod(H * c', 2))

toc






