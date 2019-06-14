#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    if((pid = fork()) > 0) // 부모가 있으
    {
        wait(&status);  //자식이 죽을 때까지 기다리기
        //뭘로 죽었는지 확인하기
        if(!(status & 0xff))
            printf("(exit) status: %d\n", WEXITSTATUS(status)); //정상종료
        else if(!((status >> 8) & 0xff))
            printf("(signal) status: %d\n", status & 0x7f); //몇 번 시그널에 의해 종료된 경우
    }
#if 0
    16비트 가져오는데,
    정상종료에 대한 status code는 상위에 있어야 함.
    즉, 16비트 중 상위 8비트는 정상종료
    하위 8비트는 비정상종료.

    어디서 abort가 발생했는지, 어디서 문제가 일어나는지 확인하는 소스코드이다.
    wait(), abort(): system call이다.
#endif

    else if(!pid) // pid: 자식이 없으면
    {
        abort();    //강제종료 시그널
    }

    else
    {
        perror("fork() ");
        exit(-1);
    }

    return 0;

}
#if 0
    putty cmd창에 kill -l 넣기 (실행 파일 리스트 전부 보여줌)
    6번 시그널(ABRT)에 의해 프로세스가 종료되었다.
    void (* siganl(int num void(* handler)(int)))(int);
#endif
