#include <hcsparse.h>
#include <iostream>
#include "hc_am.hpp"
int main(int argc, char *argv[])
{
    hcsparseCsrMatrix gCsrMat;
    hcdenseMatrix gX;
    hcdenseMatrix gY;
    hcsparseScalar gAlpha;
    hcsparseScalar gBeta;

    std::vector<accelerator>acc = accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 

    hcsparseControl control(accl_view);

    if (argc != 2)
    {
        std::cout<<"Required mtx input file"<<std::endl;
        return 0;
    }

    const char* filename = argv[1];

    int num_nonzero, num_row_A, num_col_A;

    hcsparseStatus status;

    status = hcsparseHeaderfromFile(&num_nonzero, &num_row_A, &num_col_A, filename);

    if (status != hcsparseSuccess)
    {
        std::cout<<"The input file should be in mtx format"<<std::endl;
        return 0;
    } 

    int num_row_X, num_col_X, num_row_Y, num_col_Y;

    num_row_X = num_col_A;
    num_col_X = rand()%100;
    num_row_Y = num_row_A;
    num_col_Y = num_col_X;

    double *host_res = (double*) calloc(num_row_Y * num_col_Y, sizeof(double));
    double *host_X = (double*) calloc(num_row_X * num_col_X, sizeof(double));
    double *host_Y = (double*) calloc(num_row_Y * num_col_Y, sizeof(double));
    double *host_alpha = (double*) calloc(1, sizeof(double));
    double *host_beta = (double*) calloc(1, sizeof(double));

    hcsparseSetup();
    hcsparseInitCsrMatrix(&gCsrMat);
    hcsparseInitScalar(&gAlpha);
    hcsparseInitScalar(&gBeta);
    hcdenseInitMatrix(&gX);
    hcdenseInitMatrix(&gY);

    gX.values = am_alloc(sizeof(double) * num_col_X * num_row_X, acc[1], 0);
    gY.values = am_alloc(sizeof(double) * num_row_Y * num_col_Y, acc[1], 0);
    gAlpha.value = am_alloc(sizeof(double) * 1, acc[1], 0);
    gBeta.value = am_alloc(sizeof(double) * 1, acc[1], 0);

    srand (time(NULL));
    for (int i = 0; i < num_col_X * num_row_X; i++)
    {
       host_X[i] = rand()%100;
    } 

    for (int i = 0; i < num_row_Y * num_col_Y; i++)
    {
        host_res[i] = host_Y[i] = rand()%100;
    }

    host_alpha[0] = rand()%100;
    host_beta[0] = rand()%100;

    control.accl_view.copy(host_X, gX.values, sizeof(double) * num_col_X * num_row_X);
    control.accl_view.copy(host_Y, gY.values, sizeof(double) * num_row_Y * num_col_Y);
    control.accl_view.copy(host_alpha, gAlpha.value, sizeof(double) * 1);
    control.accl_view.copy(host_beta, gBeta.value, sizeof(double) * 1);

    gAlpha.offValue = 0;
    gBeta.offValue = 0;
    gX.offValues = 0;
    gY.offValues = 0;

    gX.num_rows = num_row_X;
    gX.num_cols = num_col_X;
    gX.lead_dim = num_col_X;
    gY.num_rows = num_row_Y;
    gY.num_cols = num_col_Y;
    gY.lead_dim = num_col_Y;

    gX.major = rowMajor;
    gY.major = rowMajor;

    gCsrMat.offValues = 0;
    gCsrMat.offColInd = 0;
    gCsrMat.offRowOff = 0;

    double *values = (double*)calloc(num_nonzero, sizeof(double));
    int *rowOffsets = (int*)calloc(num_row_A+1, sizeof(int));
    int *colIndices = (int*)calloc(num_nonzero, sizeof(int));

    gCsrMat.values = am_alloc(sizeof(double) * num_nonzero, acc[1], 0);
    gCsrMat.rowOffsets = am_alloc(sizeof(int) * (num_row_A+1), acc[1], 0);
    gCsrMat.colIndices = am_alloc(sizeof(int) * num_nonzero, acc[1], 0);

    status = hcsparseDCsrMatrixfromFile(&gCsrMat, filename, &control, false);
   
    control.accl_view.copy(gCsrMat.values, values, sizeof(double) * num_nonzero);
    control.accl_view.copy(gCsrMat.rowOffsets, rowOffsets, sizeof(int) * (num_row_A+1));
    control.accl_view.copy(gCsrMat.colIndices, colIndices, sizeof(int) * num_nonzero);

    if (status != hcsparseSuccess)
    {
        std::cout<<"The input file should be in mtx format"<<std::endl;
        return 0;
    }
 
    hcsparseDcsrmm(&gAlpha, &gCsrMat, &gX, &gBeta, &gY, &control); 

    for (int col = 0; col < num_col_X; col++)
    {
        int indx = 0;
        for (int row = 0; row < num_row_A; row++)
        {
            double sum = 0.0; 
            for (; indx < rowOffsets[row+1]; indx++)
            {
                sum += host_alpha[0] * host_X[colIndices[indx] * num_col_X + col] * values[indx];
            }
            host_res[row * num_col_Y + col] = sum + host_beta[0] * host_res[row * num_col_Y + col];
        }
    }

    control.accl_view.copy(gY.values, host_Y, sizeof(double) * num_row_Y * num_col_Y);

    bool isPassed = 1;  

    for (int i = 0; i < num_row_Y * num_col_Y; i++)
    {
        double diff = std::abs(host_res[i] - host_Y[i]);
        if (diff > 0.01)
        {
            std::cout<<i<<" "<<host_res[i]<<" "<<host_Y[i]<< " "<<diff<<std::endl;
            isPassed = 0;
            break;
        }
    }

    std::cout << (isPassed?"TEST PASSED":"TEST FAILED") << std::endl;

    hcsparseTeardown();

    free(host_res);
    free(host_X);
    free(host_Y);
    free(host_alpha);
    free(host_beta);
    free(values);
    free(rowOffsets);
    free(colIndices);
    am_free(gX.values);
    am_free(gY.values);
    am_free(gAlpha.value);
    am_free(gBeta.value);
    am_free(gCsrMat.values);
    am_free(gCsrMat.rowOffsets);
    am_free(gCsrMat.colIndices);

    return 0; 
}
