#include <hcsparse.h>
#include <iostream>

#define TOLERANCE 0.01

int main()
{
    hcsparseScalar gR;
    hcdenseVector gX;

    std::vector<accelerator>acc = accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view()); 

    hcsparseControl control(accl_view);

    int num_elements = 100;
    float *host_res = (float*) calloc(1, sizeof(float));
    float *host_X = (float*) calloc(num_elements, sizeof(float));
    float *host_R = (float*) calloc(1, sizeof(float));

    srand (time(NULL));
    for (int i = 0; i < num_elements; i++)
    {
        host_X[i] = rand()%100;
    }
    
    array_view<float> dev_X(num_elements, host_X);
    array_view<float> dev_R(1, host_R);

    hcsparseSetup();
    hcsparseInitScalar(&gR);
    hcsparseInitVector(&gX);

    gR.value = &dev_R;
    gX.values = &dev_X;

    gR.offValue = 0;
    gX.offValues = 0;

    gX.num_values = num_elements;

    hcsparseStatus status;

    status = hcdenseSnrm2(&gR, &gX, &control);

    for (int i = 0; i < num_elements; i++)
    {
        host_res[0] += (host_X[i] * host_X[i]);
    }
    host_res[0] = sqrt(host_res[0]);

    bool ispassed = 1;
    array_view<float> *av_res = static_cast<array_view<float> *>(gR.value);

    float diff = std::abs(host_res[0] - (*av_res)[0]);
    if (diff > TOLERANCE)
    {
        std::cout<<host_res[0]<<" "<<(*av_res)[0]<<std::endl;
        ispassed = 0;
    }

    std::cout << (ispassed?"TEST PASSED":"TEST FAILED") << std::endl;

    hcsparseTeardown();

    return 0; 
}
