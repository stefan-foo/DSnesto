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
	niz[0] = rank;

	int mask = size-1;
	for (int i = 1; i < size; i <<= 1) {
		if ((rank & mask) == rank) {
			if ((rank ^ i) < rank) {
				MPI_Send(niz, i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
			}
			else {
				MPI_Recv(&niz[i], i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);
				printf("%d -> %d :", rank ^ i, rank);
				for (int k = 0; k < i*2; k++) {
					printf("%d ", niz[k]);
				}
				printf("\n");
				fflush(stdout);
			}
		}
		mask <<= 1;
	}

	MPI_Finalize();
}