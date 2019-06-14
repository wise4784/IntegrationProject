#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define PTEST 250000000/8

int arr[PTEST] = {7};
int init[PTEST];

typedef struct _arg
{
	int start;
	int end;
}arg;

typedef struct timeval tv;

double get_runtime(tv start, tv end)
{
	end.tv_usec = end.tv_usec - start.tv_usec;
	end.tv_sec = end.tv_sec - start.tv_sec;
	end.tv_usec += end.tv_sec * 1000000;

	printf("runtime = %lf sec\n", end.tv_usec /1000000.0);
	return end.tv_usec / 1000000.0;
}

void * t1(void *x)
{
	int i;
	arg set = *(arg*)x;
	
	for(i= set.start; i<set.end; i++)
		arr[i] = arr[i-1] + init[i];

	pthread_exit((void*)arr[i-1]);
}

void * t2(void *x)
{
	int i, sum=0;
	arg set = *(arg*)x;
	
	for(i=set.start; i<set.end; i++)
		sum += init[i];

	pthread_exit((void*)sum);
}

int main(void)
{
	pthread_t thread[3] = {0};
	unsigned int i, cnt =0;
	tv start, end;
	int ret[3] = {0};
	int fin = 0;
	arg one = {1, PTEST/2};
	arg two = {PTEST / 2, PTEST};

	srand(time(NULL));

	for(i=0; i<PTEST; i++)
		init[i] = rand() % 10 + 1;

// 처음 수식
	gettimeofday(&start, NULL);

	for(i=1; i<PTEST; i++)
		arr[i] = arr[i-1] + init[i];

	gettimeofday(&end, NULL);

	printf("arr[%d] = %d\n", PTEST -1, arr[PTEST -1]);
	get_runtime(start, end);

	arr[PTEST -1] = 0;

//캐시 친화성 고려

	gettimeofday(&start, NULL);
	
	for(i=0; i<PTEST; i++)
		arr[PTEST-1] += init[i];
	arr[PTEST-1] += arr[0];

	gettimeofday(&end, NULL);

	printf("arr[%d] = %d\n", PTEST-1, arr[PTEST-1]);
	get_runtime(start, end);

	arr[PTEST-1] = 0;
//쓰레드 2개
	gettimeofday(&start, NULL);

	pthread_create(&thread[0], NULL, t1, (void *)&one);
	pthread_create(&thread[1], NULL, t2, (void *)&two);

	pthread_join(thread[0], (void **)&ret[0]);
	pthread_join(thread[1], (void **)&ret[1]);

	gettimeofday(&end, NULL);

	printf("ret[0] = %d\n", ret[0]);
	printf("ret[1] = %d\n", ret[1]);
	printf("arr[%d] = %d\n", PTEST -1, ret[0] + ret[1]);
	get_runtime(start, end);

	return 0;
}
