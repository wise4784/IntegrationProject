#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void)
{
    int status;
    pid_t pid;

    if((pid = vfork()) > 0) // vfork = chd를 생성하고 parent는 블록시킨다.
                            //일반적인 fork는 메모리 전체를 싹다 복사한다. 그렇게 진행한다면 페이징을 쓸데없이 많이해서
                        // 용량을 낭비한다.
                     // VFORK는 task_struct만 생성하고 복사작업은 하지않는다.
                     // exec는 가상메모리의 레이아웃만잡는다.


    {
        waitpid(-1, &status, WNOHANG);
        printf("ls -al test Success\n");
    }

    else if(!pid)
    {
        execlp("ls", "ls", "-a", "-l", 0);
    }

    return 0;
}
