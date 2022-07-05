#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <time.h>
int main(int argc, char* argv[]) {
	int rank, size;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(time(NULL));
	int value = rand() % 100;
	
	int act = 0;
	int korak = 1;
	for (int i = 1; i < size; i = i << 1) {
		act |= i;
		if ((act & rank) == rank) {
			if ((rank ^ i) > rank) {
				MPI_Send(&value, 1, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
			}
			else {
				MPI_Recv(&value, 1, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);
				printf("%d - (korak=%d) -> %d : = :val: %d", rank ^ i, korak, rank, value);
			}
		}
		korak++;
	}

	MPI_Finalize();
}