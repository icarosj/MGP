#include <iostream>
#include <chrono>
#include "clog.h"
#include "p_assert.h"
#include "backtrace.h"
#include "answer_64_64.h"
#include "matmul.h"

template <class T>
void run(long Dim, int bsize)
{
  long size = Dim*Dim;
  T *A = new T[size];
  T *B = new T[size];
  T *C = new T[size]; 
  T *tpC = new T[size]; 
  T *blkC = new T[size]; 
  T *blkC2 = new T[size]; 
  T *tpblkC = new T[size]; 
  T *copyC = new T[size]; 


  T **A2 = new T*[Dim];
  T **B2 = new T*[Dim];
  T **C2 = new T*[Dim];
  for(int r=0; r<Dim; r++) {
		A2[r] = new T[Dim];
		B2[r] = new T[Dim];
		C2[r] = new T[Dim];
	}

  /* intialize */
  std::chrono::duration<double> diff;
  auto start = std::chrono::steady_clock::now();
  for(int r=0; r<Dim; r++) {
    for(int c=0; c<Dim; c++) {
      A[r*Dim + c] = r*100+c;
      B[r*Dim + c] = c*100+r;
      C[r*Dim + c] = 0;
      tpC[r*Dim + c] = 0;
      blkC[r*Dim + c] = 0;

      A2[r][c] = r*100+c;
      B2[r][c] = c*100+r;
      C2[r][c] = 0;
    }
  }
  auto end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"init arrays took "<<diff.count()<<" sec"<<std::endl;

//goto copy;



  //do naive matmul
  start = std::chrono::steady_clock::now();
  naive_matmul(Dim, A, B, C);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"normal matmul took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;

  //transposed matmul
  start = std::chrono::steady_clock::now();
  tp_matmul(Dim, A, B, tpC);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"tp matmul took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;


  //block matmul
  start = std::chrono::steady_clock::now();
  block_matmul(Dim, bsize, A, B, blkC);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"block matmul took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;
  

  //block matmul2
  start = std::chrono::steady_clock::now();
  tp_block_matmul(Dim, bsize, A, B, blkC2);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"tp block matmul took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;
  
//tpblock matmul2
  start = std::chrono::steady_clock::now();
  block_matmul2(Dim, bsize, A, B, tpblkC);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"block matmul2 took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;


copy:
  //copied matmul
  start = std::chrono::steady_clock::now();
  Multiply(Dim, bsize, A, B, copyC);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"copy matmul took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;
  

  //copied matmul2
  start = std::chrono::steady_clock::now();
  Multiply2(Dim, bsize, A2, B2, C2);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"copy matmul2 took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;
 
  //copied matmul3
  start = std::chrono::steady_clock::now();
  Multiply3(Dim, A2, B2, C2);
  end = std::chrono::steady_clock::now();
  diff = end-start;
  std::cout<<"copy matmul3 took "<<diff.count()<<" sec"<<std::endl;
	std::cout<<std::endl;
  
  /*
  std::cout<<"A = ["<<std::endl;
  for(int r=0; r<Dim; r++) {
    for(int c=0; c<Dim; c++) {
      std::cout<<A[r*Dim + c]<<", ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"]"<<std::endl;

  std::cout<<"B = ["<<std::endl;
  for(int r=0; r<Dim; r++) {
    for(int c=0; c<Dim; c++) {
      std::cout<<B[r*Dim + c]<<", ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"]"<<std::endl;
  */
  

  if(Dim==64) {
    clog::Write(Info, "checking answers against 64x64 truth..\n");
    int num_err=0;
    for(int r=0; r<Dim; r++) {
      for(int c=0; c<Dim; c++) {
        if(C[r*Dim + c] != answer_64_64[r*Dim + c]) {
          num_err++;
          //std::cout<<"["<<r<<"]["<<c<<"]: "<<C[r*Dim + c] <<" : "<<answer_64_64[r*Dim + c]<<std::endl;
        }
      }
    }
    clog::Write(Info, "Done. %d errors found.\n", num_err);
  }

  //naive versus tp check
  {
    clog::Write(Info, "checking answers naive:tp\n");
    int num_err=0;
    for(int r=0; r<Dim; r++) {
      for(int c=0; c<Dim; c++) {
        if(C[r*Dim + c] != tpC[r*Dim + c]) {
          num_err++;
          //std::cout<<"["<<r<<"]["<<c<<"]: "<<C[r*Dim + c] <<" : "<<tpC[r*Dim + c]<<std::endl;
        }
      }
    }
    clog::Write(Info, "Done. %d errors found.\n", num_err);
  }

{
    clog::Write(Info, "checking answers naive:blk\n");
    int num_err=0;
    for(int r=0; r<Dim; r++) {
      for(int c=0; c<Dim; c++) {
        if(C[r*Dim + c] != tpC[r*Dim + c]) {
          num_err++;
          //std::cout<<"["<<r<<"]["<<c<<"]: "<<C[r*Dim + c] <<" : "<<blkC[r*Dim + c]<<std::endl;
        }
      }
    }
    clog::Write(Info, "Done. %d errors found.\n", num_err);
  }
}


int main(int argc, char* argv[])
{
  install_backtrace_handler();
  std::cout<<"matmul single-thread, version "<<GIT_VERSION<<std::endl;
  long Dim = atoi(argv[1]);
  long bsize= atoi(argv[2]);
  int debug_level= atoi(argv[3]);
  clog::SetLevel(debug_level);
  std::cout<<"Mat Size "<<Dim<<", "<<Dim<<std::endl;

run<float>(Dim, bsize);

return 0;
}