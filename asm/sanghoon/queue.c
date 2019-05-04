#include <stdio.h>
#include <stdlib.h>

typedef struct _queue queue;
struct _queue
{
	queue *link;
	int data;
};

queue *get_node(void)
{
	queue *tmp;

	tmp = (queue *)malloc(sizeof(queue));
	tmp->link = NULL;

	return tmp;
}

void enqueue(queue **head, int num)
{
	if(!*head)
	{
		(*head) = get_node();
		(*head)->data = num;
		return;
	}

	enqueue(&(*head)->link, num);
}

int main(void)
{
	queue *head = NULL;
	enqueue(&head, 10);

	return 0;
}
