mex encode_.cpp
mex decode_.cpp

tic

G = [1, 0, 0, 1, 0, 1; 
     0, 1, 0, 1, 1, 1; 
     0, 0, 1, 0, 1, 1];

H = [1, 1, 0, 1, 0, 0;
     0, 1, 1, 0, 1, 0;
     1, 1, 1, 0, 0, 1];

% ���������
u = [0, 0, 1];

% �����������
c = encode_(u, G);

% ��������� ���������
% c(1) = 1;

% �������������
[y, iter] = decode_(c, H);

% �������� ������
mod(H * c', 2)

toc

%G = zeros(7154,8176);
% I = eye(7154, 7154);
% Z = zeros(7154, 1022);
% G = [I, Z];
% 
% B1 = '55BF56CC55283DFEEFEA8C8CFF04E1EBD9067710988E25048D67525426939E2068D2DC6FCD2F822BEB6BD96C8A76F4932AAE9BC53AD20A2A9C86BB461E43759C';
% 
% N = [];
% for i=1:64
%     S = sprintf('%d*', hex2dec(B1(2*i-1:2*i)));
%     N = [N, S];
% end
% 
% B = split(N, '*');
% b = cellfun(@(x)str2double(x), B);
% 
% res = [];
% for i=1:64
%     res = [res, dec2bin(b(i), 8)];
% end
% 
% 
% 
% shift = shift(res);


