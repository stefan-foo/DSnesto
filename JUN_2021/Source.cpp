#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <math.h>

#define k 4
#define m 6
#define l 2

int main(int argc, char* argv[]) {
	int rank, size;
	int A[k][m], loc_A[k][l], b[m], x[k], loc_b[l];
	int loc_sum[k], sum[k];

	MPI_Status status;
	MPI_Request req;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(time(NULL));
	if (rank == 0) {
		printf("A\n");
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < m; j++) {
				A[i][j] = rand() % 1000;
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
		printf("B\n");
		for (int i = 0; i < m; i++) {
			b[i] = rand() % 1000;
			printf("%d ", b[i]);
		}
		printf("\n");
		fflush(stdout);
		for (int p = size-1; p >= 0; p--) {
			for (int i = 0; i < k; i++) {
				for (int j = 0; j < l; j++) {
					loc_A[i][j] = A[i][p * l + j];
				}
			}
			if (p) MPI_Send(loc_A, k * l, MPI_INT, p, 0, MPI_COMM_WORLD);
		}
		fflush(stdout);
	}
	else 
		MPI_Recv(loc_A, l * k, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	
	printf("PROCESS %d\n", rank);
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < l; j++) {
			printf("%d ", loc_A[i][j]);
		}
		printf("\n");
	}

	fflush(stdout);

	MPI_Scatter(b, l, MPI_INT, loc_b, l, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < k; i++) loc_sum[i] = 0;
	//A: n * m B: m * k resulting = n*k
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < k; j++) {
	//		result[i][j] = 0;
	//		for (int k = 0; k < m; k++) {
	//			result[i][j] += A[i][k] * B[k][j];
	//		}
	//	}
	//}

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < l; j++) {
			loc_sum[i] += loc_A[i][j] * loc_b[j];
		}
	}

	MPI_Reduce(loc_sum, sum, k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("RESULTING VECTOR IS:\n");
		for (int i = 0; i < k; i++)
			printf("%d\n", sum[i]);
	}


	
	MPI_Finalize();
}