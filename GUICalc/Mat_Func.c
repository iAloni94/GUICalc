#include "GUICalc.h"



int** initMat(int matSize[]) {
  
    int** mat = (int**)calloc(matSize[0], sizeof(int*));
    for (int i = 0; i < matSize[0]; i++)
        mat[i] = (int*)calloc(matSize[1], sizeof(int));

    return mat;
}



// Function to get Minor of mat[p][q] in temp[][]. n is
// current dimension of mat[][]
void getMinor(int** mat, int** temp, int p,
    int q, int n)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those
            //  element which are not in given row and
            //  column
            if (row != p && col != q)
            {
                temp[i][j++] = mat[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of mat[][]. */
int getDet(int** mat, int n, MAT *matDim, FLAGS *flags)
{
    int D = 0; // Initialize result

    if (matDim->n != matDim->m || (!(matDim->n) || !(matDim->m))) {
        flags->err = true;
        MessageBox(NULL,
            _T("Can't calculate determinante - Matrix is not square or not initialized"),
            _T("Error"),
            NULL);
        return;
    }

    //  Base case : if matrix contains single element
    if (n == 1)
        return mat[0][0];

    if (n == 2) {
        return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
    }

    int** temp; // To store cofactors
    temp = (int**)calloc(n, sizeof(int*));
    for (int i = 0; i < n-1; i++)
    {
        temp[i] = (int*)malloc(n, sizeof(int));
    }

    int sign = 1; // To store sign multiplier

    // Iterate for each element of first row
    for (int i = 0; i < n; i++)
    {
        // Getting Cofactor of mat[0][f]
        getMinor(mat, temp, 0, i, n);
        D += sign * mat[0][i] * getDet(temp, n - 1, matDim, flags);
        sign = -sign;
    }

    return D;
}


int **matMultiply(int** mat_a, int** mat_b, MAT* mat_a_dim, MAT* mat_b_dim, FLAGS *flags) {

    if (mat_a_dim->m != mat_b_dim->n) {
        MessageBox(NULL, _T("Cannot multiply A and B. Dimentions does not match"), _T("Error"), NULL);
        flags->err = true;
        return NULL;
    }

    int matDim[2] = {mat_a_dim->n, mat_b_dim->m};
    int** resMat = initMat(matDim);
    int sum = 0;
    wchar_t buff[2] = { 0 };

    for (int i = 0; i < mat_a_dim->n; i++)
    {
        for (int j = 0; j < mat_b_dim->m; j++)
        {
            for (int k = 0; k < mat_a_dim->m; k++)
            {
                resMat[i][j] += mat_a[i][k] * mat_b[k][j];
            }
        }
    }
    return resMat;
}