#include <iostream>
#include <vector>
#include <numeric>

#include <stdint.h>
#include <math.h>

#define p 0.3

using namespace std;

vector <char> encode_(const vector <char> &message, vector <char> &G, uint16_t rows, uint16_t cols);
vector <char> decode_(const vector <double> &codeword, const vector <char> &H, int k, int n, uint16_t &iter, const uint16_t iterMax, const double EsN0);

bool check(const vector <char> &codeword, const vector <char> &H, int k, int n);


int main() {

    vector <char> H = {1, 1, 0, 1, 0, 0,
                       0, 1, 1, 0, 1, 0,
                       1, 0, 0, 0, 1, 1,
                       0, 0, 1, 1, 0, 1};
    int m = 4;
    int n = 6;
    int k = 2;

    vector <char> c = {0, 0, 1, 0, 1, 1};
    vector <double> y = {-0.1, 0.5, -0.8, 1.0, -0.7, 0.5};

    uint16_t iter;

    vector <char> decodedword = decode_(y, H, m, n, iter, 10, 1.25);

    return 0;
}


vector <char> encode_(const vector <char> &message, vector <char> &G, uint16_t rows, uint16_t cols)
{
    vector <char> res(cols);

    for (int i = 0; i < cols; i++) {
        int sum = 0;
        for (int j = 0; j < rows; j++) {
            sum += message[j] * G[i + cols * j];
        }
        res.push_back(sum);
    }
    return res;
}


vector <char> decode_(const vector <double> &codeword, const vector <char> &H, int m, int n, uint16_t &iter, const uint16_t iterMax, const double EsN0) {

    // double R[k][n];
    // double E[k][n];
    // double r[n];
    // double L[n];

    vector <char> decodedword(n);


    double **M = (double**) calloc(m, sizeof(double*));
    for (int i = 0; i < m; i++){
        M[i] = (double*) calloc(n, sizeof(double));
    }

    double **E = (double**) calloc(m, sizeof(double*));
    for (int i = 0; i < m; i++){
        E[i] = (double*) calloc(n, sizeof(double));
    }

    double *r = (double*) calloc(n, sizeof(double));
    double *L = (double*) calloc(n, sizeof(double));


    for (int i = 0; i < n; i++) {
        /* for AWGN channel model */
        r[i] = 4 * codeword[i] * EsN0;

        /* for BSC model */
        //      if (codeword[i] == 1) {
        //          r[i] = log(p / (1 - p));
        //        } else {
        //          r[i] = log((1 - p) / p);
        //        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] = r[j] * H[i*n + j];   //i, j
        }
    }

    iter = 1;
    while (iter <= iterMax) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                long double p1 = 1;
                if (H[i*n + j] == 1) {
                    for (int q = 1; q < n; q++) {
                        if (H[i*n + (j + q) % n] == 1) {
                            p1 *= tanh(M[i][(j + q) % n] / 2);
                        }
                    }
                    E[i][j] = log((1 + p1) / (1 - p1));
                }
            }
        }

        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < m; j++) {
                sum += E[j][i];
            }

            L[i] = r[i] + sum;
        }

        for (int i = 0; i < n; i++) {
            if (L[i] > 0) {
                decodedword[i] = 0;
            } else if (L[i] <= 0) {
                decodedword[i] = 1;
            }
        }

        if (check(decodedword, H, m, n) == true || iter == iterMax) {
            break;
        } else {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (H[j*n + i] == 1) {
                        double sum = 0;
                        for (int q = 0; q < m; q++) {
                            if(q != j) {
                                sum += E[q][i];
                            }
                        }
                        M[j][i] = r[i] + sum;
                    }
                }
            }
            iter += 1;
        }
    }


    for (int i = 0; i < m; i++){
        free(M[i]);
    }

    for (int i = 0; i < m; i++){
        free(E[i]);
    }

    free(M);
    free(E);
    free(r);
    free(L);

    return decodedword;
}

bool check(const vector <char> &codeword, const vector <char> &H, int k, int n) {

    uint32_t sum = 0;
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < n; i++) {
            sum += codeword[i] * H[j*n + i];
        }
        if (sum % 2 != 0) {
            return false;
        }
    }

    return true;
}

