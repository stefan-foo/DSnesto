#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <math.h>
#include <string>

int main(int argc, char* argv[]) {
	int rank, size;
	int value = 0, sum = 0;

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int stepen = 1;
	int sqr = log2(size);

	sum = rank + 1;
	int i = 0;
	for (i; i < sqr; i++) {
		if (rank % (stepen*2)) {
			MPI_Send(&sum, 1, MPI_INT, rank - stepen, 0, MPI_COMM_WORLD);
			//std::cout << "rank " << rank << " sends " << sum << " to rank " << rank - stepen << std::endl;
			break;
		}
		else {
			MPI_Recv(&value, 1, MPI_INT, rank + stepen, 0, MPI_COMM_WORLD, &status);
			sum += value;
			//std::cout << "rank " << rank << " receives " << value << " from " << rank + stepen
			//	<< " sum is " << sum << std::endl;
		}
		stepen *= 2;
	}
	if (i == sqr)
		std::cout << sum;
	MPI_Finalize();
}