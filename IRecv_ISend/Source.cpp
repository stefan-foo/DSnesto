#include <mpi.h>
#include <limits.h>
#include <stdio.h>
#define m 4
#define n 3
void main(int argc, char* argv[])
{
	int a[m][n], b[n], rank, p, i, j;
	int lc[m], c[m], y[m], x[m], z;
	struct { int val; int rank; }min, gmin;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	if (rank == 0)
	{
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				a[i][j] = i + j;
		for (j = 0; j < n; j++)
			b[j] = 1;
	}
	if (rank == 0)
	{
		for (i = 0; i < m; i++) { x[i] = a[i][0]; }
		for (j = 1; j < p; j++)
		{
			for (i = 0; i < m; i++)
				y[i] = a[i][j];
			MPI_Send(y, m, MPI_INT, j, 0, MPI_COMM_WORLD);
		}
	}
	else
		MPI_Recv(x, m, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	MPI_Scatter(&b[0], 1, MPI_INT, &z, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < m; i++)
		lc[i] = x[i] * z;
	min.val = INT_MAX;
	for (int i = 0; i < m; i++)
		if (x[i] < min.val)
		{
			min.val = x[i]; min.rank = rank;
		}
	MPI_Reduce(&min, &gmin, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gmin, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(lc, c, m, MPI_INT, MPI_SUM, gmin.rank, MPI_COMM_WORLD);
	if (rank == gmin.rank)
	{
		for (i = 0; i < m; i++)
			printf("c[%d]=%d\n", i, c[i]);
		printf("gmin=%d", gmin.val);
	}
	MPI_Finalize();
}