#include <stdio.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
	int rank, size, sum = 0;
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	int segmentLength;
	int* segment, *first;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	segmentLength = 2 * rank;
	segment = (int*)malloc(segmentLength * sizeof(int));

	if (rank == 0) {
		first = arr;
		for (int i = 1; i < size; i++) {
			MPI_Send(first, i*2, MPI_INT, i, 0, MPI_COMM_WORLD);
			first += i * 2;
		}
	}
	else {
		MPI_Recv(segment, segmentLength, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		for (int i = 0; i < segmentLength; i++) {
			sum += segment[i];
		}
		std::cout << "RANK[" << rank << "]: " << sum << std::endl;
	}


	MPI_Finalize();
}