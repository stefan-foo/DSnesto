#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <math.h>

int main(int argc, char* argv[]) {
	int rank, size, recvrank;
	int value = 0, sum = 0;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {

		for (int i = 1; i < size; i++) {
			value = rand();
			MPI_Send(&value, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			value = rand();
			MPI_Send(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		for (int i = 0; i < size - 1; i++) {
			MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			std::cout << "ID: " << value << std::endl;
		}
	}
	else {
		MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		std::cout << "ID[" << rank << "] msg1: " << value << std::endl;
		MPI_Recv(&value, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		std::cout << "ID[" << rank << "] msg2: " << value << std::endl;
		MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}