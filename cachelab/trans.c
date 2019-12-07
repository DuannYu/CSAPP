/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
void transpose_32x32(int M, int N, int A[N][M], int B[M][N]);
void transpose_64x64(int M, int N, int A[N][M], int B[M][N]);
void transpose_67x61(int M, int N, int A[N][M], int B[M][N]);

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32 && N == 32)
        transpose_32x32(M, N, A, B);
    if (M == 64 && N == 64)
        transpose_64x64(M, N, A, B);
    else
        transpose_67x61(M, N, A, B);
}

/*
 * 32 * 32 transpose function
 */
char transpose_32x32_desc[] = "Transpose 32 * 32";
void transpose_32x32(int M, int N, int A[N][M], int B[M][N])
{
    for (int i = 0; i < N; i += 8)
    {
        for (int j = 0; j < M; j += 8)
        {
            for (int k = i; k < i + 8; ++k)
            {
                int temp_value0 = A[k][j];
                int temp_value1 = A[k][j+1];
                int temp_value2 = A[k][j+2];
                int temp_value3 = A[k][j+3];
                int temp_value4 = A[k][j+4];
                int temp_value5 = A[k][j+5];
                int temp_value6 = A[k][j+6];
                int temp_value7 = A[k][j+7];
              
                B[j][k] = temp_value0;
                B[j+1][k] = temp_value1;
                B[j+2][k] = temp_value2;
                B[j+3][k] = temp_value3;
                B[j+4][k] = temp_value4;
                B[j+5][k] = temp_value5;
                B[j+6][k] = temp_value6;
                B[j+7][k] = temp_value7;
            }
        }
    }
}

/*
 * 64 * 64 transpose function
 */
char transpose_64x64_desc[] = "Transpose 64 * 64";
void transpose_64x64(int M, int N, int A[N][M], int B[M][N])
{

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // i = 0, j = 1 时，m = 8;
            int m = (j << 3);
            int k, a0, a1, a2, a3, a4, a5, a6, a7;
            for (k = (i << 3); k < (i << 3) + 4; k++) {
                // 将第0行8～15元素存入内存
                a0 = A[k][m+0]; a1 = A[k][m+1]; a2 = A[k][m+2]; a3 = A[k][m+3];
                a4 = A[k][m+4]; a5 = A[k][m+5]; a6 = A[k][m+6]; a7 = A[k][m+7];

                // 8*8块的前四行
                B[m+0][k] = a0; B[m+1][k] = a1;   B[m+2][k] = a2; B[m+3][k] = a3;
                B[m][k+4] = a4; B[m+1][k+4] = a5; B[m+2][k+4] = a6; B[m+3][k+4] = a7; 
            }

            // i = 0， j = 1时，m = 4, k = 8;
            m = (i << 3) + 4;
            for (k = (j << 3); k < (j << 3) + 4; k++) {

                a0 = A[m+0][k]; a1 = A[m+1][k]; 
                a2 = A[m+2][k]; a3 = A[m+3][k];


                a4 = B[k][m+0]; a5 = B[k][m+1];
                a6 = B[k][m+2]; a7 = B[k][m+3];

                // 逐行访问前四行后四列
                B[k][m+0] = a0; B[k][m+1] = a1;
                B[k][m+2] = a2; B[k][m+3] = a3;

                // 逐行后四行前四列
                B[k+4][m-4] = a4; B[k+4][m-3] = a5; 
                B[k+4][m-2] = a6; B[k+4][m-1] = a7;

                // 逐行访问后四行后四列
                B[k+4][m+0] = A[m+0][k+4]; B[k+4][m+1] = A[m+1][k+4];  
                B[k+4][m+2] = A[m+2][k+4]; B[k+4][m+3] = A[m+3][k+4];  

            }
        }
    }
}

/*
 * 67 * 61 transpose function
 */
char transpose_67x61_desc[] = "Transpose 67 * 61";
void transpose_67x61(int M, int N, int A[N][M], int B[M][N])
{
    for (int i = 0; i < N; i += 16) {
        for (int j = 0; j < M; j += 16) {
            for (int x = i; x < N && x < i+16; x++) {
                for (int y = j; y < M && y < j+16; y++) {
                    B[y][x] = A[x][y];
                }
            }
        }
    }

}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc); 
    registerTransFunction(transpose_32x32, transpose_32x32_desc);
    registerTransFunction(transpose_64x64, transpose_64x64_desc);
    registerTransFunction(transpose_67x61, transpose_67x61_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

