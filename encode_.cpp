#include "mex.h"
#include "stdint.h"
#include "math.h"

#include "iostream"
#include "vector"
#include "numeric"

using namespace std;

vector <bool> encode_(vector <bool> message, vector <bool> G, uint16_t rows, uint16_t cols);


void mexFunction(int nlhs, mxArray *plhs[],
				 int nrhs, const mxArray *prhs[])
{
	// DECLARATIONS
	vector <bool> message;
	vector <bool> codeword;
	vector <bool> G;
    mwSize const* dims;
    uint16_t rows;
	uint16_t cols;
	double *arg0;
    double *arg1;

	// INPUTS
	arg0 = mxGetPr(prhs[0]);
	arg1 = mxGetPr(prhs[1]);
    dims = mxGetDimensions(prhs[1]);
    rows = static_cast<uint16_t>(dims[0]);
    cols = static_cast<uint16_t>(dims[1]);
	
    // Output array:
    //plhs[1] = mxCreateDoubleMatrix(rows, rows*cols, mxREAL);

    // Access the contents of the input and output arrays:
	for (uint16_t i = 0; i < mxGetNumberOfElements(prhs[0]); i++)
	{
		message.push_back(static_cast<bool>(arg0[i]));
	}

	for (uint16_t i = 0; i < rows; i++)
	{
        for (uint16_t j = 0; j < cols; j++)
        {
		    G.push_back(static_cast<bool>(arg1[i + rows * j]));
            //mexPrintf("%f\n", arg1[i + rows * j]);
        } 
	}	
	
	// Execute main code
	codeword = encode_(message, G, rows, cols);
	
	
	// OUTPUTS
	plhs[0] = mxCreateDoubleMatrix(1, codeword.size(), mxREAL);
	
	double *ret = mxGetPr(plhs[0]);

	for (uint16_t i = 0; i < codeword.size(); i++)
	{
		ret[i] = static_cast<double>(codeword[i]);
	}
}




vector <bool> encode_(vector <bool> message, vector <bool> G, uint16_t rows, uint16_t cols)
{

    vector <bool> res;

    for (int i = 0; i < cols; i++) {
        int sum = 0;
        for (int j = 0; j < rows; j++) {
            sum += message[j] * G[i + cols * j];
        }
        res.push_back(sum);
    }
    return res;
}
