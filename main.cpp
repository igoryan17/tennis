#include <iostream>
#include <mpi.h>

using namespace std;

class Ball {
public:
    void trown(int rank, int receiver) {
        MPI::COMM_WORLD.Ssend(&rank, 1, MPI::INT, receiver, 0);
        std::cout << PROCESS_INFO << rank << BALL_THROWN << PROCESS_INFO << receiver << std::endl;
    }
    void caught(int rank, int sender) {
        MPI::COMM_WORLD.Recv(&buf, 1, MPI::INT, sender, 0);
        std::cout << PROCESS_INFO << rank << BALL_CAUGHT << PROCESS_INFO << buf << std::endl;
    }
private:
    const string PROCESS_INFO = "process number ";
    const string BALL_THROWN = " trow ball to ";
    const string BALL_CAUGHT = " catch from ";
    int buf;
};

int main() {
    Ball ball;
    MPI::Init();
    int size = 0;
    size = MPI::COMM_WORLD.Get_size();

    if (size % 2 != 0) {
        MPI::Finalize();
        return -1;
    }

    int rank = 0;
    rank = MPI::COMM_WORLD.Get_rank();

    //first throw, second wait to catch
    if (rank % 2 == 0) {
        ball.trown(rank, rank + 1);
    }
    else {
        ball.caught(rank, rank - 1);
    }
    //second trow after catching to first and first catch it
    if (rank % 2 != 0) {
        ball.trown(rank, rank - 1);
    }
    else {
        ball.caught(rank, rank + 1);
    }

    MPI::Finalize();
    return 0;
}