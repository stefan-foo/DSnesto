#include <stdio.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
	int rank, size;
	int value = 0;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* niz = new int[size];
	if (rank == 0)
		for (int i = 0; i < size; i++) niz[i] = i;

	int mask = 0;
	for (int i = size >> 1; i > 0; i >>= 1) {
		mask |= i;
		if ((rank & mask) == rank) {
			if ((rank ^ i) > rank) {
				MPI_Send(&niz[i], i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
			}
			else {
				MPI_Recv(niz, i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);
				printf("%d -> %d :", rank ^ i, rank);
				for (int k = 0; k < i; k++) {
					printf("%d ", niz[k]);
				}
				printf("\n");
				fflush(stdout);
			}
		}
	}

	MPI_Finalize();
}