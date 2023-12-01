% % mex encode_.cpp
% % mex decode_.cpp
% 
tic

G = [1, 0, 0, 1, 0, 1; 
     0, 1, 0, 1, 1, 1; 
     0, 0, 1, 0, 1, 1];

H = [1, 1, 0, 1, 0, 0;
     0, 1, 1, 0, 1, 0;
     1, 0, 0, 0, 1, 1;
     0, 0, 1, 1, 0, 1];


code = [0, 0, 1];

seq  = encode_(code, G, size(G, 1), size(G, 2));
dseq = decode_( seq, H, size(H, 1), size(H, 2));

toc