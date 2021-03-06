#include "hcsparse.h"
template <typename T>
hcsparseStatus
Norm1 (hcsparseScalar* pS,
       const hcdenseVector* pX,
       hcsparseControl* control)
{
    return reduce<T, RO_FABS>(pS, pX, control);
}
