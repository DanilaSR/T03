% % mex encode_.cpp
% % mex decode_.cpp
% 
% tic
% 
% G = [1, 0, 0, 1, 0, 1; 
%      0, 1, 0, 1, 1, 1; 
%      0, 0, 1, 0, 1, 1];
% 
% H = [1, 1, 0, 1, 0, 0;
%      0, 1, 1, 0, 1, 0;
%      1, 0, 0, 0, 1, 1;
%      0, 0, 1, 1, 0, 1];
% 
% 
% code = [0, 0, 1];
% 
% seq  = encode_(code, G, size(G, 1), size(G, 2));
% 
% dseq = decode_( seq, H, size(H, 1), size(H, 2));
% 
% toc

%G = zeros(7154,8176);
I = eye(7154, 7154);
Z = zeros(7154, 1022);
G = [I, Z];

%b1 = '55BF56CC55283DFEEFEA8C8CFF04E1EBD9067710988E25048D67525426939E2068D2DC6FCD2F822BEB6BD96C8A76F4932AAE9BC53AD20A2A9C86BB461E43759C';
% b12 =
% b21 =
% b22 = 
    
H = Get_H_8176_7156;
%G = Get_G_8176_7156;
 


%B1 = circulant(b1);



