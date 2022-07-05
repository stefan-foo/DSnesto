#include <stdio.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
	int niz[] = { 77, 22, 11, 4, 5, 2, 55, 18, 24, 14, 13, 11 };
	int rank, size, segmentSize = 0, N = 12;
	int* segment;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	segmentSize = N / size;
	segment = (int*)malloc(segmentSize * sizeof(int));
	
	MPI_Scatter(niz, segmentSize, MPI_INT, segment, segmentSize, MPI_INT, 0, MPI_COMM_WORLD);

	int mymin = segment[0];
	for (int i = 1; i < segmentSize; i++)
		mymin = segment[i] < mymin ? segment[i] : mymin;

	int min;
	MPI_Reduce(&mymin, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	if (rank == 0)
		std::cout << min << std::endl;

	MPI_Finalize();
}