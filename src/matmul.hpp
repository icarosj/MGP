#include "clog.h"
#include <chrono>
#include <iostream>

namespace {
  template <class T>
    void naive_matmul(long Dim, T *A, T *B, T *O) 
    {
      for(int r=0; r<Dim; r++) {
        //clog::Write(Major, "row %d\n", r);
        for(int c=0; c<Dim; c++) {
          for(int i=0; i<Dim; i++) {
            O[r*Dim + c] += A[r*Dim + i] * B[i*Dim + c];
          }
        }
      }
    }


  template <class T>
    void transpose(long Dim, T *A, T *O) 
    {
      for(int r=0; r<Dim; r++) {
        for(int c=0; c<Dim; c++) {
          O[c*Dim + r] = A[r*Dim + c];
        }
      }
    }

  template <class T>
    void tp_matmul(long Dim, T *A, T *B, T *O)
    {
      //1. transpose
      std::chrono::duration<double> diff;
      auto start = std::chrono::steady_clock::now();
      T* tpB = new T[Dim*Dim];
      transpose(Dim, B, tpB);
      auto end = std::chrono::steady_clock::now();
      diff = end-start;
      std::cout<<"transpose took "<<diff.count()<<" sec"<<std::endl;

      //. matmul, with seq read on tpB
      start = std::chrono::steady_clock::now();
      for(int r=0; r<Dim; r++) {
        clog::Write(Major, "row %d\n", r);
        for(int c=0; c<Dim; c++) {
          for(int i=0; i<Dim; i++) {
            O[r*Dim + c] += A[r*Dim + i] * tpB[c*Dim + i];
          }
        }
      } 
      end = std::chrono::steady_clock::now();
      diff = end-start;
      std::cout<<"calculation took "<<diff.count()<<" sec"<<std::endl;
    }




  template <class T>
    void block_matmul(long Dim, int bsize, T *A, T *B, T *O)
    {
      std::chrono::duration<double> diff;
      auto start = std::chrono::steady_clock::now();

      for(int rb=0; rb<Dim; rb+=bsize) {
        //clog::Write(Minor, "row %d\n", rb);
        for(int cb=0; cb<Dim; cb+=bsize) {
          for(int ib=0; ib<Dim; ib+=bsize) {
            //clog::Write(Minor, "rb %d cb %d ib %d\n", rb, cb, ib);
            //O[rb,cb] += A[rb, ib] * B[ib, cb] 
            for(int r=rb; r < rb+bsize && r<Dim; r++) {
              for(int c=cb; c < cb+bsize && c<Dim; c++) {
                for(int i=ib; i < ib+bsize && i<Dim; i++) {
                  O[r*Dim + c] += A[r*Dim + i] * B[i*Dim + c];
                  //clog::Write(Minor, "   r %d c %d i %d\n", r, c, i);
                  //clog::Write(Minor, "   A %d B %d O %d\n", A[r*Dim+i], B[i*Dim+c], O[r*Dim+c]);
                  //clog::Write(Minor, "\n");
                }
              }
            }
          }
        } 
      }
      auto end = std::chrono::steady_clock::now();
      diff = end-start;
      std::cout<<"block took "<<diff.count()<<" sec"<<std::endl;
    }
  template <class T>
    void tp_block_matmul(long Dim, int bsize, T *A, T *B, T *O)
    {
      std::chrono::duration<double> diff;
      auto start = std::chrono::steady_clock::now();
      T* tpB = new T[Dim*Dim];
      transpose(Dim, B, tpB);
      auto end = std::chrono::steady_clock::now();
      diff = end-start;
      std::cout<<"transpose took "<<diff.count()<<" sec"<<std::endl;


      start = std::chrono::steady_clock::now();

      for(int rb=0; rb<Dim; rb+=bsize) {
        //clog::Write(Minor, "row %d\n", rb);
        for(int cb=0; cb<Dim; cb+=bsize) {
          for(int ib=0; ib<Dim; ib+=bsize) {
            //clog::Write(Minor, "rb %d cb %d ib %d\n", rb, cb, ib);
            //O[rb,cb] += A[rb, ib] * B[ib, cb] 
            for(int r=rb; r < rb+bsize && r<Dim; r++) {
              for(int c=cb; c < cb+bsize && c<Dim; c++) {
                for(int i=ib; i < ib+bsize && i<Dim; i++) {
                  O[r*Dim + c] += A[r*Dim + i] * B[c*Dim + i];
                  //clog::Write(Minor, "   r %d c %d i %d\n", r, c, i);
                  //clog::Write(Minor, "   A %d B %d O %d\n", A[r*Dim+i], B[i*Dim+c], O[r*Dim+c]);
                  //clog::Write(Minor, "\n");
                }
              }
            }
          }
        } 
      }
      end = std::chrono::steady_clock::now();
      diff = end-start;
      std::cout<<"block took "<<diff.count()<<" sec"<<std::endl;
    }

  template <class T>
    void block_matmul2(long Dim, int bsize, T *A, T *B, T *O)
    {
      std::chrono::duration<double> diff;
      auto start = std::chrono::steady_clock::now();

      for(int rb=0; rb<Dim; rb+=bsize) {
        //clog::Write(Minor, "row %d\n", rb);
        for(int cb=0; cb<Dim; cb+=bsize) {
          for(int ib=0; ib<Dim; ib+=bsize) {
            //clog::Write(Minor, "rb %d cb %d ib %d\n", rb, cb, ib);
            //O[rb,ib] += A[rb, cb] * B[cb, ib] 
            for(int r=rb; r < rb+bsize && r<Dim; r++) {
              for(int c=cb; c < cb+bsize && c<Dim; c++) {
                for(int i=ib; i < ib+bsize && i<Dim; i++) {
                  O[r*Dim + c] += A[r*Dim + i] * B[i*Dim + c];
                  //clog::Write(Minor, "   r %d c %d i %d\n", r, c, i);
                  //clog::Write(Minor, "   A %d B %d O %d\n", A[r*Dim+i], B[i*Dim+c], O[r*Dim+c]);
                  //clog::Write(Minor, "\n");
                }
              }
            }
          }
        } 
      }
      auto end = std::chrono::steady_clock::now();
      diff = end-start;
      std::cout<<"block took "<<diff.count()<<" sec"<<std::endl;
    }



  template <class T>
  void Multiply(int n, int block, T* a, T* b, T* c)
  {
    int bi=0;
    int bj=0;
    int bk=0;
    int i=0;
    int j=0;
    int k=0;
    int blockSize=block; 

    for(bi=0; bi<n; bi+=blockSize)
      for(bj=0; bj<n; bj+=blockSize)
        for(bk=0; bk<n; bk+=blockSize)
          for(i=0; i<blockSize; i++)
            for(j=0; j<blockSize; j++)
              for(k=0; k<blockSize; k++)
                c[(bi+i)*n+bj+j] += a[(bi+i)*n+bk+k]*b[(bk+k)*n+bj+j];
  }

  template <class T>
    void Multiply2(int n, int block, T** a, T** b, T** c)
    {
      int bi=0;
      int bj=0;
      int bk=0;
      int i=0;
      int j=0;
      int k=0;
      int blockSize=block; 

      for(bi=0; bi<n; bi+=blockSize)
        for(bj=0; bj<n; bj+=blockSize)
          for(bk=0; bk<n; bk+=blockSize)
            for(i=0; i<blockSize; i++)
              for(j=0; j<blockSize; j++)
                for(k=0; k<blockSize; k++)
                  c[bi+i][bj+j] += 
                    a[bi+i][bk+k]
                    *b[bk+k][bj+j];
    }
  template <class T>
    void Multiply3(int n, T** a, T** b, T** c)
    {
      int i=0;
      int j=0;
      int k=0;

      for(i=0; i<n; i++)
        for(j=0; j<n; j++)
          for(k=0; k<n; k++)
            c[i][j] += 
              a[i][k]
              *b[k][j];
    }

}
