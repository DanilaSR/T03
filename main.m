clear all; close all;

mex encode_.cpp
mex decode_.cpp

tic

% ������������ ���� 
%Code(k,n)
G = Get_G_8176_7156();
H = Get_H_8176_7156();

% �������� ������
% is_matrix_error = any(any(G*H'))

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
% is_coder_error = any(mod(H * c', 2))

% ����������
s = (u == y(1:k));
similarity = sum(s)/numel(s);


disp(['Iteration number to decode: ', num2str(iter)]);
disp(['Similarity decode: ', num2str(sum(s)), '/', num2str(numel(s)), ...
    ' (', num2str(similarity),')']);


toc






