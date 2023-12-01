#include "mex.h"

#include "stdint.h"
#include "math.h"

#include "iostream"
#include "vector"
#include "numeric"

#define I_MAX 10 //максимальное число итераций
#define p 0.3    //вероятность

using namespace std;

vector <uint16_t> decode_(vector <uint16_t> string, vector <uint16_t> H, int k, int n);
int check(vector <uint16_t> string, vector <uint16_t> H, int k, int n);

void mexFunction(int nlhs, mxArray *plhs[], 
                 int nrhs, const mxArray *prhs[] )
{
	/* DECLARATIONS*/
	vector <uint16_t> code, dseq, H;
    mwSize const* dims;
    mwSize rows, cols;

    dims = mxGetDimensions(prhs[1]);
    rows = dims[0];
    cols = dims[1];

	
	/* INPUTS */
	double *arg1  = mxGetPr(prhs[0]);
  double *a     = mxGetPr(prhs[1]);


	
	for (uint16_t i = 0; i < mxGetNumberOfElements(prhs[0]); i++)
	{
		code.push_back(static_cast<uint16_t>(arg1[i]));
		//mexPrintf("%u ", seq[i]);
	}
	
	for (uint16_t i = 0; i < rows; i++)
	{
        for (uint16_t j = 0; j < cols; j++)
        {
		    H.push_back(static_cast<uint16_t>(a[i + rows * j]));
            mexPrintf("%f\n", a[i + rows * j]);
        } 
	}	
	
	dseq = decode_(code, H, static_cast<uint16_t>(rows), static_cast<uint16_t>(cols));
	//mexPrintf("code.size %u\n", code.size());
	  
	
	// OUTPUTS
	plhs[0] = mxCreateDoubleMatrix(1, dseq.size(), mxREAL);
	double *ret = mxGetPr(plhs[0]);

	for (uint16_t i = 0; i < dseq.size(); i++)
	{
		ret[i] = static_cast<double>(dseq[i]);
	}
}



vector <uint16_t> decode_(vector <uint16_t> string, vector <uint16_t> H, int k, int n) {

  double R[k][n];
  double E[k][n];
  double r[n], L[n];

  for (int i = 0; i < n; i++) {
    if (string[i] == 1) {
      r[i] = log(p / (1 - p));
    } else if (string[i] == 0) {
      r[i] = log((1 - p) / p);
    }
  }

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < n; j++) {
      R[i][j] = r[j] * H[i*n + j];   //i, j
    }
  }

  int I = 1;
  while (1) {
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < n; j++) {
        double p1 = 1;
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
        string[i] = 0;
      } else if (L[i] <= 0) {
        string[i] = 1;
      }
    }

    if (check(string, H, k, n) == 0 || I == I_MAX) {
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

      I += 1;
      continue;
    }
  }

  return string;
}

int check(vector <uint16_t> string, vector <uint16_t> H, int k, int n) {

  int flag = 1;
  int sum = 0;
  for (int j = 0; j < k; j++) {
    for (int i = 0; i < n; i++) {
      sum += string[i] * H[j*n + i];
    }
    if (sum % 2 != 0) {
      flag = 0;
    }
  }

  if (flag == 1) {
    return 0;
  } else {
    return 1;
  }
}

