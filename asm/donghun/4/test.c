#include <stdio.h>
#include <stdlib.h>

typedef struct __queue
{
    int data;
    struct __queue *link;
}queue;

queue *ins_queue(void)
{
    queue *tmp = (queue *)malloc(sizeof(queue));
    tmp->link = NULL;
    return tmp;
}

int main(void)
{
    queue *head = ins_queue();
    return 0;
}
