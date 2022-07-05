#include <stdio.h>
#include <mpi.h>
#include <iostream>

#define n 4
#define m 3

int main(int argc, char* argv[]) {
	int size, rank;
	int A[m][n], b[n], h[m];
	int loc_A[m], loc_b;
	int loc_c[m], c[m];
	
	int mul[m];

	struct {
		int value;
		int rank;
	} in, out;

	MPI_Request req;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++)
				A[i][j] = (i + 1) * j;
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				h[j] = A[j][i];
			b[i] = i + 1;
			MPI_Isend(h, m, MPI_INT, i, 0, MPI_COMM_WORLD, &req);
		}
	}
	MPI_Recv(loc_A, m, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	//ODAVDE SE NASTAVLJA B, ZADATAK POD A OSTAVLJA DOSADASNJE VREDNOSTI U ORIGINALNOM STANJU
	MPI_Scatter(b, 1, MPI_INT, &loc_b, 1, MPI_INT, 0, MPI_COMM_WORLD);

	printf("RANK[%d]:", rank);
	for (int i = 0; i < m; i++) {
		printf("%d ", loc_A[i]);
	}
	fflush(stdout);

	in.rank = rank;
	in.value = INT_MAX;

	for (int i = 0; i < m; i++) {
		if (loc_A[i] < in.value)
			in.value = loc_A[i];
		loc_c[i] = loc_A[i] * loc_b;
	}

	MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&out, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	printf("%d\n", loc_b);
	fflush(stdout);
	MPI_Reduce(loc_A, mul, m, MPI_INT, MPI_PROD, out.rank, MPI_COMM_WORLD);
	MPI_Reduce(loc_c, c, m, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);
	if (rank == out.rank) {
		printf("Proizvod vrsta:\n");
		for (int i = 0; i < m; i++)
			printf("%d ", mul[i]);
		printf("\nProizvod matrice i vektora:\n");
		for (int i = 0; i < m; i++)
			printf("%d ", c[i]);
	}


	/*BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB*/
	fflush(stdout);
	MPI_Barrier(MPI_COMM_WORLD);
	if (!rank) printf("B-B-B-B-B-B-B-B-B-B-B-B\n");
	//MPI_Scatter(b, 1, MPI_INT, &loc_b, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (int i = 0; i < n; i++)
			MPI_Isend(&b[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &req);
	}
	MPI_Recv(&loc_b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);


	printf("RANK[%d]:", rank);
	for (int i = 0; i < m; i++) {
		printf("%d ", loc_A[i]);
	}
	fflush(stdout);

	in.rank = rank;
	in.value = INT_MAX;

	for (int i = 0; i < m; i++) {
		if (loc_A[i] < in.value)
			in.value = loc_A[i];
		loc_c[i] = loc_A[i] * loc_b;
	}

	//MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Isend(&in, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD, &req);
	if (rank == 0) {
		out.value = INT_MAX;
		for (int i = 0; i < n; i++) {
			printf("blocked?"); fflush(stdout);
			MPI_Recv(&in, 1, MPI_2INT, i, 0, MPI_COMM_WORLD, &status);
			printf("not blocked!"); fflush(stdout);
			if (in.value < out.value)
				out = in;
		}

		//MPI_Bcast(&out, 1, MPI_2INT, 0, MPI_COMM_WORLD);
		for (int i = 1; i < n; i++) {
			MPI_Send(&out, 1, MPI_2INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
		MPI_Recv(&out, 1, MPI_2INT, 0, 0, MPI_COMM_WORLD, &status);


	printf("%d\n", loc_b);
	fflush(stdout);

	if (rank == out.rank) {
		for (int i = 0; i < m; i++) {
			mul[i] = loc_A[i];
			c[i] = loc_c[i];
		}
		for (int i = 0; i < n; i++) {
			if (rank != i) {
				MPI_Recv(loc_A, m, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				MPI_Recv(loc_c, m, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
				for (int j = 0; j < m; j++) {
					mul[j] *= loc_A[j];
					c[j] += loc_c[j];
				}
			}
		}
	}
	else {
		MPI_Send(loc_A, m, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
		MPI_Send(loc_c, m, MPI_INT, out.rank, 1, MPI_COMM_WORLD);
	}

	//MPI_Reduce(loc_A, mul, m, MPI_INT, MPI_PROD, out.rank, MPI_COMM_WORLD);
	//MPI_Reduce(loc_c, c, m, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);
	if (rank == out.rank) {
		printf("Proizvod vrsta:\n");
		for (int i = 0; i < m; i++)
			printf("%d ", mul[i]);
		printf("\nProizvod matrice i vektora:\n");
		for (int i = 0; i < m; i++)
			printf("%d ", c[i]);
	}


	MPI_Finalize();
}