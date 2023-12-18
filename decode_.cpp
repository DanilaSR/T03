#include "mex.h"

#include "stdint.h"
#include "math.h"

#include "iostream"
#include "vector"
#include "numeric"
#include <stdlib.h>

#define p 0.3    //вероятность

using namespace std;

bool check(const vector <char> &codeword, const vector <char> &H, int k, int n);
vector <char> decode_(const vector <double> &codeword, const vector <char> &H, int k, int n, uint16_t &iter, const uint16_t iterMax, const double EsN0);


void mexFunction(int nlhs, mxArray *plhs[], 
                 int nrhs, const mxArray *prhs[])
{
	//mexPrintf("Start decoder\n");
	
	// DECLARATIONS
	vector <double> codeword;
	vector <char> decodedword;
	vector <char> H;
	uint16_t iter;
	uint16_t iter_max;
	double EsN0;
	
	// INPUTS
	double *arg0 = mxGetPr(prhs[0]);
	double *arg1 = mxGetPr(prhs[1]);
	double *arg2 = mxGetPr(prhs[2]);
	double *arg3 = mxGetPr(prhs[3]);
    mwSize const* dims = mxGetDimensions(prhs[1]);
    uint16_t rows = static_cast<uint16_t>(dims[0]);
    uint16_t cols = static_cast<uint16_t>(dims[1]);

	//mexPrintf("mxGetNumberOfElements(prhs[0]) = %u\n", mxGetNumberOfElements(prhs[0]));
	//mexPrintf("mxGetNumberOfElements(prhs[1]) = %u\n", mxGetNumberOfElements(prhs[1]));
	//mexPrintf("Matrix H: rows = %u, cols = %u\n", rows, cols);
	
	
	// Access the contents of the input and output arrays:
	for (uint16_t i = 0; i < mxGetNumberOfElements(prhs[0]); i++)
	{
		codeword.push_back(static_cast<double>(arg0[i]));
	}
	//mexPrintf("codeword.size() = %u\n", codeword.size());
	
	
	for (uint16_t i = 0; i < rows; i++)
	{
        for (uint16_t j = 0; j < cols; j++)
        {
		    H.push_back(static_cast<char>(arg1[i + rows * j]));
        } 
	}
	//mexPrintf("H.size() = %u\n", H.size());
	
	iter_max = static_cast<uint16_t>(*arg2);
	EsN0 = static_cast<double>(*arg3);
	
	// Execute main code
	decodedword = decode_(codeword, H, rows, cols, iter, iter_max, EsN0);
 
	
	// OUTPUTS
	plhs[0] = mxCreateDoubleMatrix(1, decodedword.size(), mxREAL);
	plhs[1] = mxCreateNumericMatrix(1, 1, mxUINT16_CLASS, mxREAL);
	
	double *ret = mxGetPr(plhs[0]);
	for (uint16_t i = 0; i < decodedword.size(); i++)
	{
		ret[i] = static_cast<char>(decodedword[i]);
	}
	
	uint16_t *i = static_cast<uint16_t *>(mxGetData(plhs[1]));
	*i = iter;
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
                    if (H[j + i*m] == 1) {
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