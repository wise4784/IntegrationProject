#include <signal.h>
#include <unistd.h>


int main(void)
{
    signal(SIGINT, SIG_IGN); // SIG_IGN = SIG신호를 ignore한다.
    pause();

    return 0;


}
