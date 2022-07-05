#include <stdio.h>
#include <mpi.h>
#include <iostream>

#define k 6
#define n 4
#define m 4
#define l 2
int main(int argc, char* argv[]) {
	int rank, size;
	int A[k][m], local_A[l][m], columns_mul_loc[m], columns_mul[m];
	int local_c[l][n], C[k][n];
	int B[m][n];


	struct {
		int value;
		int rank;
	} in, out;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < m; j++) {
				A[i][j] = i + j + 1;
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				B[i][j] = i * 2 + j;
				printf("%d ", B[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);
	}

	MPI_Scatter(&A[0][0], m * l, MPI_INT, &local_A[0][0], m * l, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(B, m * n, MPI_INT, 0, MPI_COMM_WORLD);

	in.rank = rank;
	in.value = local_A[0][0];

	printf("%d local A:\n", rank); //nalazanje proizvoda po lokalnim kolonima i minimuma
	for (int i = 0; i < m; i++) {
		columns_mul_loc[i] = 1;
		for (int j = 0; j < l; j++) {
			columns_mul_loc[i] *= local_A[j][i];

			if (in.value > local_A[i][j])
				in.value = local_A[i][j];
		}
		printf("%d ", columns_mul_loc[i]);
	}
	fflush(stdout);

	for (int i = 0; i < l; i++) { //nalazenje parcijalnog proizvoda matrica
		for (int j = 0; j < n; j++) {
			local_c[i][j] = 0;
			for (int p = 0; p < m; p++) {
				local_c[i][j] += local_A[i][p] * B[p][j];
			}
		}
	}
	
	MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&out, 1, MPI_2INT, 0, MPI_COMM_WORLD);

	MPI_Reduce(columns_mul_loc, columns_mul, m, MPI_INT, MPI_PROD, out.rank, MPI_COMM_WORLD);
	MPI_Gather(local_c, l * n, MPI_INT, C, l * n, MPI_INT, out.rank, MPI_COMM_WORLD);

	printf("\n");
	if (rank == out.rank) {
		for (int i = 0; i < m; i++)
			printf("%d ", columns_mul[i]);
		printf("\n");
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < n; j++) {
				printf("%d ", C[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();
}