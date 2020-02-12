#include <iostream>



int main(int argc, char* argv[])
{
  std::cout<<"matmul single-thread, version "<<GIT_VERSION<<std::endl;
  int Dim = atoi(argv[1]);
  int *A = new int[Dim*Dim];
  int *B = new int[Dim*Dim];
  int *C = new int[Dim*Dim]; 
  /* intialize */
  for(int r=0; r<Dim; r++) {
    for(int c=0; c<Dim; c++) {
      A[r*Dim + c] = r*100+c;
      B[r*Dim + c] = c*100+r;
      C[r*Dim + c] = 0;
    }
  }

  for(int r=0; r<Dim; r++) {
    for(int c=0; c<Dim; c++) {
      for(int i=0; i<Dim; i++) {
        C[r*Dim + c] += A[r*Dim + i] * B[i*Dim + c];
      }
    }
  }
  return 0;
}
