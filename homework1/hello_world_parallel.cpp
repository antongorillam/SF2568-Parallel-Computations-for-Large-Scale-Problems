
#include <stdio.h>
#include <string> 
#include <cstring> 
#include <mpi.h>
#include <sstream>
using namespace std;

int main(int argc, char **argv) {

    int rank, size, tag, rc, i;
    MPI_Status status;
    char message[20];

    rc = MPI_Init(&argc, &argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    tag = 42;

    char message_list[size][20];

    if (rank == 0) {
        strcpy(message, "from rank");
        for (i = 1; i < size; i++) {
            rc = MPI_Send(message, 13, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            rc = MPI_Recv(message, 13, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
            strcpy(message_list[i], message);
        }
        for (i = 1; i < size; i++) {
            printf("rank %d : %s\n", rank, message_list[i]);
        }
    } else {
        rc = MPI_Recv(message, 13, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        string rank_str = to_string(rank);
        // printf("rank %d : %s\n", rank, rank_str.c_str());
        strcpy(message, rank_str.c_str());
        rc = MPI_Send(message, 13, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }
    // printf("rank %d : %.13s\n", rank, message);
    rc = MPI_Finalize();

}