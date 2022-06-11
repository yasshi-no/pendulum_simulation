#include <gauss_elimination.h>

#include <cmath>

using namespace std;

void gauss_elimination(double **A, double *b, double *x, int n)
{
    for(int i = 0; i < n; i++) {
        double mx = 0;
        int row = 0;
        for(int j = i; j < n; j++) {
            if(abs(A[j][i]) > mx) {
                mx = abs(A[j][i]);
                row = j;
            }
        }

        double *temp1 = A[i];
        A[i] = A[row];
        A[row] = temp1;

        double temp2 = b[i];
        b[i] = b[row];
        b[row] = temp2;

        temp2 = 1.0 / A[i][i];
        for(int j = i + 1; j < n; j++) {
            A[i][j] = A[i][j] * temp2;
        }
        b[i] = b[i] * temp2;
        for(int j = i + 1; j < n; j++) {
            double temp = A[j][i];
            for(int k = 0; k < n; k++) {
                A[j][k] = A[j][k] - temp * A[i][k];
            }
            b[j] = b[j] - temp * b[i];
        }
    }

    for(int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for(int j = n - 1; j > i; j--) {
            x[i] = x[i] - A[i][j] * x[j];
        }
    }
    return;
}