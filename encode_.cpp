#include "mex.h"
#include "stdint.h"
#include "math.h"

#include "iostream"
#include "vector"
#include "numeric"

using namespace std;

vector <uint16_t> encode_(vector <uint16_t> code, vector <uint16_t> G, uint16_t rows, uint16_t cols);


void mexFunction(int nlhs, mxArray *plhs[],
				 int nrhs, const mxArray *prhs[])
{
	/* DECLARATIONS*/
	vector <uint16_t> code, seq, G;
    mwSize const* dims;
    mwSize rows, cols;
    double *a;

    dims = mxGetDimensions(prhs[1]);
    rows = dims[0];
    cols = dims[1];
	
	/* INPUTS */
	double *arg1 = mxGetPr(prhs[0]);

    // Output array:
    //plhs[1] = mxCreateDoubleMatrix(rows, rows*cols, mxREAL);

    // Access the contents of the input and output arrays:
    a = mxGetPr(prhs[1]);

	
	for (uint16_t i = 0; i < mxGetNumberOfElements(prhs[0]); i++)
	{
		code.push_back(static_cast<uint16_t>(arg1[i]));
	}

	for (uint16_t i = 0; i < rows; i++)
	{
        for (uint16_t j = 0; j < cols; j++)
        {
		    G.push_back(static_cast<uint16_t>(a[i + rows * j]));
            //mexPrintf("%f\n", a[i + rows * j]);
        } 
	}	
	
	seq = encode_(code, G, static_cast<uint16_t>(rows), static_cast<uint16_t>(cols));
	
	
	// OUTPUTS
	plhs[0] = mxCreateDoubleMatrix(1, seq.size(), mxREAL);
	double *ret = mxGetPr(plhs[0]);

	for (uint16_t i = 0; i < seq.size(); i++)
	{
		ret[i] = static_cast<double>(seq[i]);
	}
}




vector <uint16_t> encode_(vector <uint16_t> code, vector <uint16_t> G, uint16_t rows, uint16_t cols)
{

    vector <uint16_t> res;

    for (int i = 0; i < cols; i++) {
        int sum = 0;
        for (int j = 0; j < rows; j++) {
            sum += code[j] * G[i + cols * j];
        }
        res.push_back(sum);
    }
    return res;
}
