
template <class T>
void naive_matmul(long Dim, T *A, T *B, T *O);
template <class T>
void transpose(T *A, T *O);
template <class T>
void transpose2D(T **A, T **O);
template <class T>
void tp_matmul(long Dim, T *A, T *B, T *O);
template <class T>
void block_matmul(long Dim, int bsize, T *A, T *B, T *O);
template <class T>
void tp_block_matmul(long Dim, int bsize, T *A, T *B, T *O);
template <class T>
void block_matmul_alt(long Dim, int bsize, T *A, T *B, T *O);


template <class T>
void naive_matmul2D(long Dim, T **A, T **B, T **O);
template <class T>
void transpose2D(T **A, T **O);
template <class T>
void tp_matmul2D(long Dim, T **A, T **B, T **O);
template <class T>
void block_matmul2D(long Dim, int bsize, T **A, T **B, T **O);
template <class T>
void tp_block_matmul2D(long Dim, int bsize, T **A, T **B, T **O);
template <class T>
void block_matmul_alt2D(long Dim, int bsize, T **A, T **B, T **O);

template <class T>
void MultiplyRef1D(int n, int block, T* a, T* b, T* c);
template <class T>
void MultiplyRef2D(int n, int block, T** a, T** b, T** c);
template <class T>
void MultiplyBase2D(int n, T** a, T** b, T** c);


#include "matmul.hpp"
