#include <x86intrin.h>
#define UNROLL (4)
#define BLOCK (2)

/** Naive  Approach 
void dgemm(int m, int n, float *A, float *C)
{
  int i, j, k;

  for (i = 0; i < m; i++) // col 
  {
    for (k = 0; k < n; k++) // row 
    {
      for (j = 0; j < m; j++) // items in the col
      {
        C[i + j * m] += A[i + k * m] * A[j + k * m];
      }
    }
  }
} /** End of Naive**/

/** Reordering
// Changing the order of the loop allows for better temporal locality.
// The Matrix can be fill in a row wise fassion by using a small 
// set of numbers from A.
// eg. 
// if A = 1,2,3,4,5,6 and m=3,n=2                               ----->
// |1 4|   |1 2 3|  the column A=(1,2,3) can be          |(1*1)+ (1*2)+ (1*3)| then use the next      |(1)+(4*4) (2)+(4*5) (3)+(4*6)|    
// |2 5| * |4 5 6| ====================================> |(2*1)+ (2*2)+ (2*3)| =====================> |(2)+(5*4) (2)+(5*5) (6)+(5*6)|
// |3 6|            use to fill in the matrix row wise   |(3*1)+ (3*2)+ (3*3)|  column A=(4,5,6)      |(3)+(6*4) (6)+(6*5) (9)+(6*6)|
void dgemm(int m, int n, float *A, float *C)
{
  int row, col, i;
  for (col = 0; col < n; col++)
  {
    for (row = 0; row < m; row++)
    {
      for (i = 0; i < m; i++)
      {
        C[i + row * m] += A[i + col * m] * A[row + col * m];
      }
    }
  }
} /** End Reordering**/


/** Blocking  **/
// The idea is to split the larger matrixes into smaller ones 
void dgemm( int m, int n, float *A, float *C ) 
{
    int row, col, blockRow, blockCol, i, blockSize;
    blockSize = n;

    for (row = 0; row < m; row += blockSize)
    {
      for (col = 0; col < n; col += blockSize)
      {
        for (blockRow = row; blockRow < row + blockSize; blockRow++)
        {
          for (blockCol = col; blockCol < col + blockSize; blockCol++)
          {
            for (i = 0; i < blockSize; i++)
            {
              C[i + blockRow * m] += A[i + blockCol * m] * A[blockRow + blockCol * m];
            }
          }
        }
      }
    }
} /** End Blocking **/