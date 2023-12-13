#include "mex.h"

#include "stdint.h"
#include "math.h"

#include "iostream"
#include "vector"
#include "numeric"
#include <stdlib.h>

#define I_MAX 100 //максимальное число итераций
#define p 0.3    //вероятность

using namespace std;

bool check(const vector <bool> &codeword, const vector <bool> &H, int k, int n);
vector <bool> decode_(const vector <bool> &codeword, const vector <bool> &H, int k, int n, uint16_t &iter);


void mexFunction(int nlhs, mxArray *plhs[], 
                 int nrhs, const mxArray *prhs[])
{
	mexPrintf("Start decoder\n");
	
	// DECLARATIONS
	vector <bool> codeword;
	vector <bool> decodedword;
	vector <bool> H;
    mwSize const* dims;
    uint16_t rows;
	uint16_t cols;
	uint16_t iter;
	double *arg0;
	double *arg1;
	
	// INPUTS
	arg0 = mxGetPr(prhs[0]);
	arg1 = mxGetPr(prhs[1]);
    dims = mxGetDimensions(prhs[1]);
    rows = static_cast<uint16_t>(dims[0]);
    cols = static_cast<uint16_t>(dims[1]);

	//mexPrintf("mxGetNumberOfElements(prhs[0]) = %u\n", mxGetNumberOfElements(prhs[0]));
	//mexPrintf("mxGetNumberOfElements(prhs[1]) = %u\n", mxGetNumberOfElements(prhs[1]));
	//mexPrintf("Matrix H: rows = %u, cols = %u\n", rows, cols);
	
	
	// Access the contents of the input and output arrays:
	for (uint16_t i = 0; i < mxGetNumberOfElements(prhs[0]); i++)
	{
		codeword.push_back(static_cast<bool>(arg0[i]));
	}
	//mexPrintf("codeword.size() = %u\n", codeword.size());
	
	
	for (uint16_t i = 0; i < rows; i++)
	{
        for (uint16_t j = 0; j < cols; j++)
        {
		    H.push_back(static_cast<bool>(arg1[i + rows * j]));
        } 
	}
	//mexPrintf("H.size() = %u\n", H.size());
	
	// Execute main code
	decodedword = decode_(codeword, H, rows, cols, iter);
 
	
	// OUTPUTS
	plhs[0] = mxCreateDoubleMatrix(1, decodedword.size(), mxREAL);
	plhs[1] = mxCreateNumericMatrix(1, 1, mxUINT16_CLASS, mxREAL);
	
	double *ret = mxGetPr(plhs[0]);
	for (uint16_t i = 0; i < decodedword.size(); i++)
	{
		ret[i] = static_cast<double>(decodedword[i]);
	}
	
	uint16_t *i = static_cast<uint16_t *>(mxGetData(plhs[1]));
	*i = iter;
}



vector <bool> decode_(const vector <bool> &codeword, const vector <bool> &H, int k, int n, uint16_t &iter) {

  // double R[k][n];
  // double E[k][n];
  // double r[n];
  // double L[n];
  
  vector <bool> decodedword = codeword;


  double **R = (double**) malloc(k*sizeof(double*));
  for (int i = 0; i < k; i++){
      R[i] = (double*) malloc(n*sizeof(double));
  }  

  double **E = (double**) malloc(k*sizeof(double*));
  for (int i = 0; i < k; i++){
      E[i] = (double*) malloc(n*sizeof(double));
  }

  double *r = (double*) malloc(n*sizeof(double));
  double *L = (double*) malloc(n*sizeof(double));


  for (int i = 0; i < n; i++) {
    if (decodedword[i] == 1) {
      r[i] = log(p / (1 - p));
    } else {
      r[i] = log((1 - p) / p);
    }
  }

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < n; j++) {
      R[i][j] = r[j] * H[i*n + j];   //i, j
    }
  }

  iter = 1;
  while (iter <= I_MAX) {
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < n; j++) {
        long double p1 = 1;
        if (H[i*n + j] == 1) {
          for (int q = 1; q < n; q++) {
            if (H[i*n + (j + q) % n] == 1) {
              p1 *= tanh(R[i][(j + q) % n] / 2);
            }
          }
          E[i][j] = log((1 + p1) / (1 - p1));
        }
      }
    }

    for (int i = 0; i < n; i++) {
      double sum = 0;
      for (int j = 0; j < k; j++) {
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
	
	if (check(decodedword, H, k, n) == 0 || iter == I_MAX) {
      break;
    } else {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
          double sum = 0;
          if (H[j*n + i] == 1) {
            for (int q = 1; q < k; q++) {
              if (H[( (j + q) % q )*n + i] == 1) {
                sum += E[(j + q) % q][i];
              }
            }
            R[j][i] = r[i] + sum;
          }
        }
      }
	  iter += 1;
    }
  }


  for (int i = 0; i < k; i++){
      free(R[i]);
  }

  for (int i = 0; i < k; i++){
      free(E[i]);
  }

  free(R);
  free(E);
  free(r);
  free(L);

  return codeword;
}

bool check(const vector <bool> &codeword, const vector <bool> &H, int k, int n) {

  bool flag = true;
  int sum = 0;
  for (int j = 0; j < k; j++) {
    for (int i = 0; i < n; i++) {
      sum += codeword[i] * H[j*n + i];
    }
  }
  if (sum % 2 != 0) {
    flag = false;
  }

  return !flag;
}

