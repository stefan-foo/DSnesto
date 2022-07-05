#include <stdio.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
	int rank, size;
	int value = 0, sum = 0;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		value = 1;
		MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	}
	else {
		if (rank == size - 1) {
			MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
			std::cout << value + rank + 1;
		}
		else {
			MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
			sum = rank + 1 + value;
			MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}