#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <sstream>
#include <time.h>

#define N 16

int main(int argc, char* argv[]) {
	int rank, size;
	float start, end, Istart, Iend, gotback;

	MPI_Request req1, req2;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::cout << "LOL" << std::endl;
	if (rank == 0) {
		start = MPI_Wtime();
		for (int i = 1; i < size; i++)
			MPI_Send(&start, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		for (int i = 1; i < size; i++)
			MPI_Recv(&gotback, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
		end = MPI_Wtime();

		Istart = MPI_Wtime();
		for (int i = 1; i < size; i++) {
			MPI_Isend(&start, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &req1);
			MPI_Irecv(&gotback, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &req2);
			MPI_Wait(&req2, &status);
		}
		Iend = MPI_Wtime();

		std::cout << "t1: " << start << " t2: " << end << " diff: " << end - start << std::endl;
		std::cout << "It1: " << Istart << " It2: " << Iend << " diff: " << Iend - Istart << std::endl;
	}
	else {
		MPI_Recv(&gotback, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Send(&gotback, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);

		MPI_Irecv(&gotback, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req1);
		MPI_Wait(&req1, &status);
		MPI_Isend(&gotback, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req2);
		MPI_Wait(&req2, &status);
	}
	MPI_Finalize();
}