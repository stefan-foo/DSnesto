#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <math.h>
#define n 6

int main(int argc, char* argv[]) {
	int rank, size;
	int value = 0, sum = 0, tot_sum;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	struct {
		int value;
		int rank;
	} in, out;

	int sabirak, k, brProstih = 0;
	int i = 0, j = rank;

	while (i < n) {
		sabirak = i + j;
		printf("[%d] summing %d and %d\n", rank, i, j);
		k = 2;
		while (k <= sqrt(sabirak) && sabirak % k != 0) k++;
		if (k <= sqrt(sabirak)) brProstih++;

		sum += sabirak;
		j += size;
		if (j >= n) i++;
		j = j % n;
	}

	in.rank = rank;
	in.value = brProstih;

	MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&out, 1, MPI_2INT, 0, MPI_COMM_WORLD);

	MPI_Reduce(&sum, &tot_sum, 1, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);
	if (rank == out.rank) {
		printf("Total sum is: %d", tot_sum);
	}

	MPI_Finalize();
}