#ifndef _HC_SPARSE_H_
#define _HC_SPARSE_H_

#include "hcsparse_struct.h"

#define ROWS_FOR_VECTOR 1
#define BLOCK_MULTIPLIER 3
#define BLOCKSIZE 1024
#define WGBITS 24
#define ROWBITS 32

    typedef enum hcsparseStatus_
    {
        /** @name Inherited OpenCL codes */
        /**@{*/
        hcsparseSuccess = 1,
        hcsparseInvalid = 0,
    } hcsparseStatus;

    /*!
    * \brief Initialize the hcsparse library
    * \note Must be called before any other hcsparse API function is invoked.
    *
    * \returns hcsparseSuccess
    *
    * \ingroup SETUP
    */
    hcsparseStatus hcsparseSetup( void );

    /*!
    * \brief Finalize the usage of the hcsparse library
    * Frees all state allocated by the hcsparse runtime and other internal data
    *
    * \returns hcsparseSuccess
    *
    * \ingroup SETUP
    */
    hcsparseStatus hcsparseTeardown( void );

    /*!
    * \brief Initialize a scalar structure to be used in the hcsparse library
    * \note It is users responsibility to allocate OpenCL device memory
    *
    * \param[out] scalar  Scalar structure to be initialized
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup INIT
    */
    hcsparseStatus hcsparseInitScalar( hcsparseScalar* scalar );

    /*!
    * \brief Initialize a dense vector structure to be used in the hcsparse library
    * \note It is users responsibility to allocate OpenCL device memory
    *
    * \param[out] vec  Dense vector structure to be initialized
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup INIT
    */
    hcsparseStatus hcsparseInitVector( hcdenseVector* vec );

    /*!
    * \brief Initialize a sparse matrix COO structure to be used in the hcsparse library
    * \note It is users responsibility to allocate OpenCL device memory
    *
    * \param[out] cooMatx  Sparse COO matrix structure to be initialized
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup INIT
    */
    hcsparseStatus hcsparseInitCooMatrix( hcsparseCooMatrix* cooMatx );

    /*!
    * \brief Initialize a sparse matrix CSR structure to be used in the hcsparse library
    * \note It is users responsibility to allocate OpenCL device memory
    *
    * \param[out] csrMatx  Sparse CSR matrix structure to be initialized
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup INIT
    */
    hcsparseStatus hcsparseInitCsrMatrix( hcsparseCsrMatrix* csrMatx );

    /*!
    * \brief Initialize a dense matrix structure to be used in the hcsparse library
    * \note It is users responsibility to allocate OpenCL device memory
    *
    * \param[out] denseMatx  Dense matrix structure to be initialized
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup INIT
    */
    hcsparseStatus hcdenseInitMatrix( hcdenseMatrix* denseMatx );
    /**@}*/

    //typedef struct _hcsparseControl*  hcsparseControl;

    /*! \brief Enumeration to *control the verbosity of the sparse iterative
     * solver routines.  VERBOSE will print helpful diagnostic messages to
     * console
     *
     * \ingroup SOLVER
     */

    typedef enum _print_mode
    {
        QUIET = 0,
        NORMAL,
        VERBOSE
    } PRINT_MODE;

    /*! \brief Enumeration to select the preconditioning algorithm used to precondition
     * the sparse data before the iterative solver execution phase
     *
     * \ingroup SOLVER
     */
    typedef enum _precond
    {
        NOPRECOND = 0,
        DIAGONAL
    } PRECONDITIONER;

    /*! \brief hcsparseSolverControl keeps state relevant for OpenCL operations
     * like kernel execution, memory allocation and synchronization behavior,
     * specifically for sparse iterative solvers
     *
     * \ingroup SOLVER
     */
    typedef struct _solverControl*  hcsparseSolverControl;

    /*!
    * \brief Create a hcsparseSolverControl object to *control hcsparse iterative
    * solver operations
    *
    * \param[in] precond  A valid enumeration constant from PRECONDITIONER
    * \param[in] maxIters  Maximum number of iterations to converge before timing out
    * \param[in] relTol  Relative tolerance
    * \param[in] absTol  Absolute tolerance
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup SOLVER
    */
    hcsparseSolverControl
        hcsparseCreateSolverControl( PRECONDITIONER precond, int maxIters,
                                     double relTol, double absTol );

     /*!
     * \brief Release a hcsparseSolverControl object created with hcsparseCreateSolverControl
     *
     * \param[in,out] solverControl  hcsparse object created with hcsparseCreateSolverControl
     *
     * \returns \b hcsparseSuccess
     *
     * \ingroup SOLVER
     */
    hcsparseStatus
        hcsparseReleaseSolverControl( hcsparseSolverControl solverControl );

    /*!
    * \brief Set hcsparseSolverControl state
    *
    * \param[in] solverControl  hcsparse object created with hcsparseCreateSolverControl
    * \param[in] precond A valid enumeration constant from PRECONDITIONER, how to precondition sparse data
    * \param[in] maxIters  Maximum number of iterations to converge before timing out
    * \param[in] relTol  Relative tolerance
    * \param[in] absTol  Absolute tolerance
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup SOLVER
    */
    hcsparseStatus
        hcsparseSetSolverParams( hcsparseSolverControl solverControl,
                                 PRECONDITIONER precond,
                                 int maxIters, double relTol, double absTol );

     /*!
     * \brief Set the verbosity level of the hcsparseSolverControl object
     *
     * \param[in] solverControl  hcsparse object created with hcsparseCreateSolverControl
     * \param[in] mode A valid enumeration constant from PRINT_MODE, to specify verbosity level
     *
     * \returns \b hcsparseSuccess
     *
     * \ingroup SOLVER
     */
    hcsparseStatus
        hcsparseSolverPrintMode( hcsparseSolverControl solverControl, PRINT_MODE mode );

    /*!
    * \brief Execute a single precision Conjugate Gradients solver
    *
    * \param[in] x  the dense vector to solve for
    * \param[in] A  a hcsparse CSR matrix with single precision data
    * \param[in] b  the input dense vector with single precision data
    * \param[in] solverControl  a valid hcsparseSolverControl object created with hcsparseCreateSolverControl
    * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup SOLVER
    */
    hcsparseStatus
        hcsparseScsrcg( hcdenseVector* x, const hcsparseCsrMatrix *A, const hcdenseVector *b,
                        hcsparseSolverControl solverControl, hcsparseControl *control );

    /*!
    * \brief Execute a double precision Conjugate Gradients solver
    *
    * \param[in] x  the dense vector to solve for
    * \param[in] A  a hcsparse CSR matrix with double precision data
    * \param[in] b  the input dense vector with double precision data
    * \param[in] solverControl  a valid hcsparseSolverControl object created with hcsparseCreateSolverControl
    * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup SOLVER
    */
    hcsparseStatus
        hcsparseDcsrcg( hcdenseVector* x, const hcsparseCsrMatrix *A, const hcdenseVector *b,
                        hcsparseSolverControl solverControl, hcsparseControl *control );

     /*!
     * \brief Execute a single precision Bi-Conjugate Gradients Stabilized solver
     *
     * \param[in] x  the dense vector to solve for
     * \param[in] A  the hcsparse CSR matrix with single precision data
     * \param[in] b  the input dense vector with single precision data
     * \param[in] solverControl  a valid hcsparseSolverControl object created with hcsparseCreateSolverControl
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \returns \b hcsparseSuccess
     *
     * \ingroup SOLVER
     */
    hcsparseStatus
        hcsparseScsrbicgStab( hcdenseVector* x, const hcsparseCsrMatrix *A, const hcdenseVector *b,
                              hcsparseSolverControl solverControl, hcsparseControl *control );

    /*!
    * \brief Execute a double precision Bi-Conjugate Gradients Stabilized solver
    *
    * \param[in] x  the dense vector to solve for
    * \param[in] A  a hcsparse CSR matrix with double precision data
    * \param[in] b  the input dense vector with double precision data
    * \param[in] solverControl  a valid hcsparseSolverControl object created with hcsparseCreateSolverControl
    * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
    *
    * \returns \b hcsparseSuccess
    *
    * \ingroup SOLVER
    */
    hcsparseStatus
        hcsparseDcsrbicgStab( hcdenseVector* x, const hcsparseCsrMatrix *A, const hcdenseVector *b,
                              hcsparseSolverControl solverControl, hcsparseControl *control );
    /**@}*/

    /*!
    * \defgroup FILE Support functions provided to read sparse matrices from file
    *
    * \brief Functions to help read the contents of matrix market files from disk
    */
    /**@{*/

    /*!
    * \brief Read the sparse matrix header from file
    *
    * \param[out] nnz  The number of non-zeroes present in the sparse matrix structure
    * \param[out] row  The number of rows in the sparse matrix
    * \param[out] col  The number of columns in the sparse matrix
    * \param[in] filePath  A path in the file-system to the sparse matrix file
    *
    * \note At this time, only matrix market (.MTX) files are supported
    * \warning The value returned in nnz is the maximum possible number of non-zeroes from the sparse
    * matrix on disk (can be used to allocate memory).  The actual number of non-zeroes may be less,
    * depending if explicit zeroes were stored in file.
    * \returns \b hcsparseSuccess
    *
    * \ingroup FILE
    */
    hcsparseStatus
        hcsparseHeaderfromFile( int* nnz, int* row, int* col, const char* filePath );

    /*!
    * \brief Read sparse matrix data from file in single precision COO format
    * \details This function reads the contents of the sparse matrix file into hcsparseCooMatrix data structure.
    * The data structure represents the contents of the sparse matrix data in OpenCL device memory.
    * This function sorts the values read (on host) by row, then column before copying them into
    * device memory
    * \param[out] cooMatx  The COO sparse structure that represents the matrix in device memory
    * \param[in] filePath  A path in the file-system to the sparse matrix file
    * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
    * \param[in] read_explicit_zeroes If the file contains values explicitly declared zero, this *controls
    * whether they are stored in the COO
    *
    * \note The number of non-zeroes actually read from the file may be different than the number of
    * non-zeroes reported from the file header. Symmetrix matrices may store up to twice as many non-zero
    * values compared to the number of values in the file. Explicitly declared zeroes may be stored
    * or not depending on the input \p read_explicit_zeroes.
    * \note The OpenCL device memory must be allocated before the call to this function.
    * \post The sparse data is sorted first by row, then by column.
    * \returns \b hcsparseSuccess
    *
    * \ingroup FILE
    */
    hcsparseStatus
        hcsparseSCooMatrixfromFile( hcsparseCooMatrix* cooMatx, const char* filePath, hcsparseControl *control, bool read_explicit_zeroes );

    /*!
     * \brief Read sparse matrix data from file in double precision COO format
     * \details This function reads the contents of the sparse matrix file into hcsparseCooMatrix data structure.
     * The data structure represents the contents of the sparse matrix data in OpenCL device memory.
     * This function sorts the values read (on host) by row, then column before copying them into
     * device memory.  If the data on disk is stored in single precision, this function will
     * up-convert the values to double.
     * \param[out] cooMatx  The COO sparse structure that represents the matrix in device memory
     * \param[in] filePath  A path in the file-system to the sparse matrix file
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \param[in] read_explicit_zeroes If the file contains values explicitly declared zero, this *controls
     * whether they are stored in the COO
     *
     * \note The number of non-zeroes actually read from the file may be less than the number of
     * non-zeroes reported from the file header. Symmetrix matrices may store up to twice as many non-zero
     * values compared to the number of values in the file. Explicitly declared zeroes may be stored
     * or not depending on the input \p read_explicit_zeroes.
     * \note The OpenCL device memory must be allocated before the call to this function.
     * \post The sparse data is sorted first by row, then by column.
     * \returns \b hcsparseSuccess
     *
     * \ingroup FILE
     */
    hcsparseStatus
        hcsparseDCooMatrixfromFile( hcsparseCooMatrix* cooMatx, const char* filePath, hcsparseControl *control, bool read_explicit_zeroes );

    /*!
     * \brief Read sparse matrix data from file in single precision CSR format
     * \details This function reads the contents of the sparse matrix file into hcsparseCsrMatrix data structure.
     * The data structure represents the contents of the sparse matrix data in OpenCL device memory.
     * This function sorts the values read (on host) by row, then column before copying them into
     * device memory
     * \param[out] csrMatx  The CSR sparse structure that represents the matrix in device memory
     * \param[in] filePath  A path in the file-system to the sparse matrix file
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \param[in] read_explicit_zeroes If the file contains values explicitly declared zero, this *controls
     * whether they are stored in the CSR
     *
     * \note The number of non-zeroes actually read from the file may be less than the number of
     * non-zeroes reported from the file header. Symmetrix matrices may store up to twice as many non-zero
     * values compared to the number of values in the file. Explicitly declared zeroes may be stored
     * or not depending on the input \p read_explicit_zeroes.
     * \note The OpenCL device memory must be allocated before the call to this function.
     * \post The sparse data is sorted first by row, then by column.
     * \returns \b hcsparseSuccess
     *
     * \ingroup FILE
     */
    hcsparseStatus
        hcsparseSCsrMatrixfromFile( hcsparseCsrMatrix* csrMatx, const char* filePath, hcsparseControl *control, bool read_explicit_zeroes );

    /*!
     * \brief Read sparse matrix data from file in double precision CSR format
     * \details This function reads the contents of the sparse matrix file into hcsparseCsrMatrix data structure.
     * The data structure represents the contents of the sparse matrix data in OpenCL device memory.
     * This function sorts the values read (on host) by row, then column before copying them into
     * device memory.  If the data on disk is stored in single precision, this function will
     * up-convert the values to double.
     * \param[out] csrMatx  The CSR sparse structure that represents the matrix in device memory
     * \param[in] filePath  A path in the file-system to the sparse matrix file
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \param[in] read_explicit_zeroes If the file contains values explicitly declared zero, this *controls
     * whether they are stored in the CSR
     *
     * \note The number of non-zeroes actually read from the file may be less than the number of
     * non-zeroes reported from the file header. Symmetrix matrices may store up to twice as many non-zero
     * values compared to the number of values in the file. Explicitly declared zeroes may be stored
     * or not depending on the input \p read_explicit_zeroes.
     * \note The OpenCL device memory must be allocated before the call to this function.
     * \post The sparse data is sorted first by row, then by column
     * \returns \b hcsparseSuccess
     *
     * \ingroup FILE
     */
    hcsparseStatus
        hcsparseDCsrMatrixfromFile( hcsparseCsrMatrix* csrMatx, const char* filePath, hcsparseControl *control, bool read_explicit_zeroes );

    /*!
     * \brief Calculate the amount of device memory required to hold meta-data for csr-adaptive SpM-dV algorithm
     * \details CSR-adaptive is a high performance sparse matrix times dense vector algorithm.  It requires a pre-processing
     * step to calculate meta-data on the sparse matrix.  This meta-data is stored alongside and carried along
     * with the other matrix data.  This function initializes the rowBlockSize member variable of the csrMatx
     * variable with the appropriate size.  The client program is responsible to allocate device memory in rowBlocks
     * of this size before calling into the library compute routines.
     * \param[in,out] csrMatx  The CSR sparse structure that represents the matrix in device memory
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup FILE
    */
    hcsparseStatus
        hcsparseCsrMetaSize( hcsparseCsrMatrix* csrMatx, hcsparseControl *control );

    /*!
     * \brief Calculate the meta-data for csr-adaptive SpM-dV algorithm
     * \details CSR-adaptive is a high performance sparse matrix times dense vector algorithm.  It requires a pre-processing
     * step to calculate meta-data on the sparse matrix.  This meta-data is stored alongside and carried along
     * with the other matrix data.  This function calculates the meta data and stores it into the rowBlocks member of
     * the hcsparseCsrMatrix.
     * \param[in,out] csrMatx  The CSR sparse structure that represents the matrix in device memory
     * \param[in] *control  A valid hcsparseControl created with hcsparseCreateControl
     * \note This function assumes that the memory for rowBlocks has already been allocated by client program
     *
     * \ingroup FILE
     */
    hcsparseStatus
        hcsparseCsrMetaCompute( hcsparseCsrMatrix* csrMatx, hcsparseControl *control );
    /**@}*/

    /*!
     * \brief Single precision scale dense vector by a scalar
     * \details \f$ r \leftarrow \alpha \ast y \f$
     * \param[out] r  Output dense vector
     * \param[in] alpha  Scalar value to multiply
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSscale( hcdenseVector* r,
                       const hcsparseScalar* alpha,
                       const hcdenseVector* y,
                       const hcsparseControl *control );

    /*!
     * \brief Double precision scale dense vector by a scalar
     * \details \f$ r \leftarrow \alpha \ast y \f$
     * \param[out] r  Output dense vector
     * \param[in] alpha  Scalar value to multiply
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDscale( hcdenseVector* r,
                       const hcsparseScalar* alpha,
                       const hcdenseVector* y,
                       const hcsparseControl *control );

    /*!
     * \brief Single precision scale dense vector and add dense vector
     * \details \f$ r \leftarrow \alpha \ast x + y \f$
     * \param[out] r  Output dense vector
     * \param[in] alpha  Scalar value to multiply
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSaxpy( hcdenseVector* r,
                      const hcsparseScalar* alpha, const hcdenseVector* x,
                      const hcdenseVector* y,
                      const hcsparseControl *control );

    /*!
     * \brief Double precision scale dense vector and add dense vector
     * \details \f$ r \leftarrow \alpha \ast x + y \f$
     * \param[out] r  Output dense vector
     * \param[in] alpha  Scalar value to multiply
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDaxpy( hcdenseVector* r,
                      const hcsparseScalar* alpha, const hcdenseVector* x,
                      const hcdenseVector* y,
                      const hcsparseControl *control );

    /*!
     * \brief Single precision scale dense vector and add scaled dense vector
     * \details \f$ r \leftarrow \alpha \ast x + \beta \ast y \f$
     * \param[out] r  Output dense vector
     * \param[in] alpha  Scalar value for x
     * \param[in] x  Input dense vector
     * \param[in] beta  Scalar value for y
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSaxpby( hcdenseVector* r,
                       const hcsparseScalar* alpha, const hcdenseVector* x,
                       const hcsparseScalar* beta,
                       const hcdenseVector* y,
                       const hcsparseControl *control );

    /*!
     * \brief Double precision scale dense vector and add scaled dense vector
     * \details \f$ r \leftarrow \alpha \ast x + \beta \ast y \f$
     * \param[out] r  Output dense vector
     * \param[in] alpha  Scalar value for x
     * \param[in] x  Input dense vector
     * \param[in] beta  Scalar value for y
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDaxpby( hcdenseVector* r,
                       const hcsparseScalar* alpha, const hcdenseVector* x,
                       const hcsparseScalar* beta,
                       const hcdenseVector* y,
                       const hcsparseControl *control );

    /*!
     * \brief Reduce integer elements of a dense vector into a scalar value
     * \details Implicit plus operator
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseIreduce( hcsparseScalar* s,
                        const hcdenseVector* x,
                        const hcsparseControl *control );

    /*!
     * \brief Reduce single precision elements of a dense vector into a scalar value
     * \details Implicit plus operator
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSreduce( hcsparseScalar* s,
                        const hcdenseVector* x,
                        const hcsparseControl *control );

    /*!
     * \brief Reduce double precision elements of a dense vector into a scalar value
     * \details Implicit plus operator
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDreduce( hcsparseScalar* s,
                        const hcdenseVector* x,
                        const hcsparseControl *control );

    /*!
     * \brief Calculate the single precision L1 norm of a dense vector
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSnrm1( hcsparseScalar* s,
                      const hcdenseVector* x,
                      const hcsparseControl *control );

    /*!
     * \brief Calculate the double precision L1 norm of a dense vector
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDnrm1( hcsparseScalar *s,
                      const hcdenseVector* x,
                      const hcsparseControl *control );

    /*!
     * \brief Calculate the single precision L2 norm of a dense vector
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSnrm2( hcsparseScalar* s,
                      const hcdenseVector* x,
                      const hcsparseControl *control );

    /*!
     * \brief Calculate the double precision L2 norm of a dense vector
     * \param[out] s  Output scalar
     * \param[in] x  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDnrm2( hcsparseScalar* s,
                      const hcdenseVector* x,
                      const hcsparseControl *control );

    /*!
     * \brief Calculates the single precision dot-product of a dense vector
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSdot( hcsparseScalar* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Calculates the double precision dot-product of a dense vector
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDdot( hcsparseScalar* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

                 /* element-wise operations for dense vectors +, -, *, / */

    /*!
     * \brief Element-wise single precision addition of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSadd( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise double precision addition of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDadd( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise single precision subtraction of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSsub( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise double precision subtraction of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDsub( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise single precision multiplication of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSmul( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise double precision multiplication of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDmul( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise single precision division of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseSdiv( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );

    /*!
     * \brief Element-wise double precision division of two dense vectors
     * \param[out] r  Output dense vector
     * \param[in] x  Input dense vector
     * \param[in] y  Input dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-1
     */
    hcsparseStatus
        hcdenseDdiv( hcdenseVector* r,
                     const hcdenseVector* x,
                     const hcdenseVector* y,
                     const hcsparseControl *control );
    /**@}*/

    /*!
     * \brief Single precision CSR sparse matrix times dense vector
     * \details \f$ y \leftarrow \alpha \ast A \ast x + \beta \ast y \f$
     * If the CSR sparse matrix structure has rowBlocks information included,
     * then the csr-adaptive algorithm is used.  Otherwise, the csr-vector
     * algorithm is used.
     * \param[in] alpha  Scalar value to multiply against sparse matrix
     * \param[in] matx  Input CSR sparse matrix
     * \param[in] x  Input dense vector
     * \param[in] beta  Scalar value to multiply against sparse vector
     * \param[out] y  Output dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-2
    */
    hcsparseStatus
        hcsparseScsrmv( const hcsparseScalar* alpha,
                        const hcsparseCsrMatrix* matx,
                        const hcdenseVector* x,
                        const hcsparseScalar* beta,
                        hcdenseVector* y,
                        const hcsparseControl *control );

    /*!
     * \brief Double precision CSR sparse matrix times dense vector
     * \details \f$ y \leftarrow \alpha \ast A \ast x + \beta \ast y \f$
     * If the CSR sparse matrix structure has rowBlocks information included,
     * then the csr-adaptive algorithm is used.  Otherwise, the csr-vector
     * algorithm is used.
     * \param[in] alpha  Scalar value to multiply against sparse matrix
     * \param[in] matx  Input CSR sparse matrix
     * \param[in] x  Input dense vector
     * \param[in] beta  Scalar value to multiply against sparse vector
     * \param[out] y  Output dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-2
    */
    hcsparseStatus
        hcsparseDcsrmv( const hcsparseScalar* alpha,
                        const hcsparseCsrMatrix* matx,
                        const hcdenseVector* x,
                        const hcsparseScalar* beta,
                        hcdenseVector* y,
                        const hcsparseControl *control );


    /*!
     * \brief Single precision COO sparse matrix times dense vector
     * \details \f$ y \leftarrow \alpha \ast A \ast x + \beta \ast y \f$
     * \param[in] alpha  Scalar value to multiply against sparse matrix
     * \param[in] matx  Input COO sparse matrix
     * \param[in] x  Input dense vector
     * \param[in] beta  Scalar value to multiply against sparse vector
     * \param[out] y  Output dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-2
    */
    hcsparseStatus
        hcsparseScoomv( const hcsparseScalar* alpha,
                        const hcsparseCooMatrix* matx,
                        const hcdenseVector* x,
                        const hcsparseScalar* beta,
                        hcdenseVector* y,
                        const hcsparseControl *control );

    /*!
     * \brief Double precision COO sparse matrix times dense vector
     * \details \f$ y \leftarrow \alpha \ast A \ast x + \beta \ast y \f$
     * \param[in] alpha  Scalar value to multiply against sparse matrix
     * \param[in] matx  Input COO sparse matrix
     * \param[in] x  Input dense vector
     * \param[in] beta  Scalar value to multiply against sparse vector
     * \param[out] y  Output dense vector
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     *
     * \ingroup BLAS-2
    */
    hcsparseStatus
        hcsparseDcoomv( const hcsparseScalar* alpha,
                        const hcsparseCooMatrix* matx,
                        const hcdenseVector* x,
                        const hcsparseScalar* beta,
                        hcdenseVector* y,
                        const hcsparseControl *control );
    /**@}*/

    /*!
     * \defgroup BLAS-3 Sparse L3 BLAS operations
     *
     * \brief Sparse BLAS level 3 routines for sparse matrix dense matrix
     * \details Level 3 BLAS operations are defined by order \f$ N^3 \f$ operations,
     * usually in the form of a matrix times a matrix.
     * \ingroup BLAS
     */
    /**@{*/

    /*!
     * \brief Single precision CSR sparse matrix times dense matrix
     * \details \f$ C \leftarrow \alpha \ast A \ast B + \beta \ast C \f$
     * \param[in] alpha  Scalar value to multiply against sparse matrix
     * \param[in] sparseMatA  Input CSR sparse matrix
     * \param[in] denseMatB  Input dense matrix
     * \param[in] beta  Scalar value to multiply against dense matrix
     * \param[out] denseMatC  Output dense matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \note This routine is currently implemented as a batched level 2 matrix
     * times a vector.
     *
     * \ingroup BLAS-3
    */
    hcsparseStatus
        hcsparseScsrmm( const hcsparseScalar* alpha,
                        const hcsparseCsrMatrix* sparseMatA,
                        const hcdenseMatrix* denseMatB,
                        const hcsparseScalar* beta,
                        hcdenseMatrix* denseMatC,
                        const hcsparseControl *control );

    /*!
     * \brief Double precision CSR sparse matrix times dense matrix
     * \details \f$ C \leftarrow \alpha \ast A \ast B + \beta \ast C \f$
     * \param[in] alpha  Scalar value to multiply against sparse matrix
     * \param[in] sparseMatA  Input CSR sparse matrix
     * \param[in] denseMatB  Input dense matrix
     * \param[in] beta  Scalar value to multiply against dense matrix
     * \param[out] denseMatC  Output dense matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \note This routine is currently implemented as a batched level 2 matrix
     * times a vector.
     *
     * \ingroup BLAS-3
    */
    hcsparseStatus
        hcsparseDcsrmm( const hcsparseScalar* alpha,
                        const hcsparseCsrMatrix* sparseMatA,
                        const hcdenseMatrix* denseMatB,
                        const hcsparseScalar* beta,
                        hcdenseMatrix* denseMatC,
                        const hcsparseControl *control );

    /*!
     * \brief Single Precision CSR Sparse Matrix times Sparse Matrix
     * \details \f$ C \leftarrow A \ast B \f$
     * \param[in] sparseMatA Input CSR sparse matrix
     * \param[in] sparseMatB Input CSR sparse matrix
     * \param[out] sparseMatC Output CSR sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The input sparse matrices data must first be sorted by rows, then by columns
     * \ingroup BLAS-3
     */
   hcsparseStatus
      hcsparseScsrSpGemm( const hcsparseCsrMatrix* sparseMatA,
                          const hcsparseCsrMatrix* sparseMatB,
                                hcsparseCsrMatrix* sparseMatC,
                          const hcsparseControl *control );
    /**@}*/

    /*!
     * \defgroup CONVERT Matrix conversion routines
     *
     * \brief Sparse matrix routines to convert from one sparse format into another
     * \note Input sparse matrices have to be sorted by row and then by column.
     * The sparse conversion routines provided by hcsparse require this as a pre-condition.  The hcsparse
     * matrix file reading routines `hcsparse_C__MatrixfromFile` guarantee this property as a post-condition.
     */
    /**@{*/

    /*!
     * \brief Convert a single precision CSR encoded sparse matrix into a COO encoded sparse matrix
     * \param[in] csr  Input CSR encoded sparse matrix
     * \param[out] coo  Output COO encoded sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseScsr2coo( const hcsparseCsrMatrix* csr,
                          hcsparseCooMatrix* coo,
                          const hcsparseControl *control );

    /*!
     * \brief Convert a double precision CSR encoded sparse matrix into a COO encoded sparse matrix
     * \param[in] csr  Input CSR encoded sparse matrix
     * \param[out] coo  Output COO encoded sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseDcsr2coo( const hcsparseCsrMatrix* csr,
                          hcsparseCooMatrix* coo,
                          const hcsparseControl *control );

    /*!
     * \brief Convert a single precision COO encoded sparse matrix into a CSR encoded sparse matrix
     * \param[in] coo  Input COO encoded sparse matrix
     * \param[out] csr  Output CSR encoded sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseScoo2csr( const hcsparseCooMatrix* coo,
                          hcsparseCsrMatrix* csr,
                          const hcsparseControl *control );

    /*!
     * \brief Convert a double precision COO encoded sparse matrix into a CSR encoded sparse matrix
     * \param[in] coo  Input COO encoded sparse matrix
     * \param[out] csr  Output CSR encoded sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseDcoo2csr( const hcsparseCooMatrix* coo,
                          hcsparseCsrMatrix* csr,
                          const hcsparseControl *control );

    /*!
     * \brief Convert a single precision CSR encoded sparse matrix into a dense matrix
     * \param[in] csr  Input CSR encoded sparse matrix
     * \param[out] A  Output dense matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseScsr2dense( const hcsparseCsrMatrix* csr,
                            hcdenseMatrix* A,
                            const hcsparseControl *control );

    /*!
     * \brief Convert a double precision CSR encoded sparse matrix into a dense matrix
     * \param[in] csr  Input CSR encoded sparse matrix
     * \param[out] A  Output dense matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseDcsr2dense( const hcsparseCsrMatrix* csr,
                            hcdenseMatrix* A,
                            hcsparseControl *control );

    /*!
     * \brief Convert a single precision dense matrix into a CSR encoded sparse matrix
     * \param[in] A  Input dense matrix
     * \param[out] csr  Output CSR encoded sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseSdense2csr( const hcdenseMatrix* A,
                            hcsparseCsrMatrix* csr,
                            const hcsparseControl *control );

    /*!
     * \brief Convert a double precision dense matrix into a CSR encoded sparse matrix
     * \param[in] A  Input dense matrix
     * \param[out] csr  Output CSR encoded sparse matrix
     * \param[in] *control A valid hcsparseControl created with hcsparseCreateControl
     * \pre The sparse matrix data must first be sorted by rows, then by columns
     *
     * \ingroup CONVERT
     */
    hcsparseStatus
        hcsparseDdense2csr( const hcdenseMatrix* A, hcsparseCsrMatrix* csr,
                            const hcsparseControl *control );
    /**@}*/

#endif // _HC_SPARSE_H_