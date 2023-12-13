clear all; close all;

mex encode_.cpp
mex decode_.cpp

tic

iter_max = 5;
bps = 1;
EbN0_dB = 1;
EsN0_dB = EbN0_dB + 10*log10(bps);
EbN0 = 10.^(EbN0_dB/20);
EsN0 = 10.^(EsN0_dB/20);

% Формирование кода 
%Code(k,n)
G = Get_G_8176_7156();
H = Get_H_8176_7156();

% Проверка ошибок
% is_matrix_error = any(any(G*H'))

% k - message bit
% n - codeword bit
% m - parity-check bit
[m, n] = size(H);
k = n - m;

% Сообщение
u = randi([0 1], 1, k);

% Кодирование
c = encode_(u, G);

% Проверка ошибок
% is_coder_error = any(mod(H * c', 2))

% Добавляем искажения
sigma = 1/EbN0;
n = normrnd(0, sigma, 1, length(c)); 
y = c + n;

% Декодирование
[d, iter] = decode_(y, H, iter_max, 1.25);


% Результаты
s = (u == d(1:k));
similarity = sum(s)/numel(s);


disp(['Iteration number to decode: ', num2str(iter)]);
disp(['Similarity decode: ', num2str(sum(s)), '/', num2str(numel(s)), ...
    ' (', num2str(similarity),')']);


toc






