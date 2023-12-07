clear all; close all;

mex encode_.cpp
mex decode_.cpp

tic

% ������������ ���� 
%Code(k,n)
G = Get_G_8176_7156();
H = Get_H_8176_7156();

% k - message bit
% n - codeword bit
% m - parity-check bit
[m, n] = size(H);
k = n - m;

% ���������
u = randi([0 1], 1, k);

% �����������
c = encode_(u, G);

% ��������� ���������
% c(1) = 1;

% �������������
[y, iter] = decode_(c, H);

% �������� ������
any(mod(H * c', 2))

toc






