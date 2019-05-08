#include <stdio.h>
#include <string.h>
#include <stdlib.h>// 동적할당해야함

#define FIX_CONST    20


typedef struct __test          // typedef없이 해도 되지만, 여기서 의미는 struct전체를 test라는 커스텀데이터타입을 만든 것이다. userdefdatatype. 배열은 다른데이터타입을 묶어놀 수 없었지만 구조체는가능.이게구조체를쓰느이유.다중구조나알고리즘 만들때엄청쓰게됨.
{
	int num_int;
	float num_float;
	double num_double;
	char name[FIX_CONST];
} test;

test	*init_struct(void)
{
	test *t=(test *)malloc(sizeof(test));    //sizeof해보면 36이나 40일 것. malloc동적할당할때사용. heap영역에다가 메모리를 할당하고 주소를 리턴함.  이 줄에서 (test *)붙은이유 -> malloc은 기본적으로 void pointer라서 test로 바꿔준것임. 사용할때 사용하려는 타입에 맞춰서 형변환을 해줘야 함. **주의 int *변수 이거는 주소받겠다고선언하는거임. 포인트로 쫓아가는거 아님.

	t->num_int =3;
	t->num_float =3.3f;
	t->num_double =7.7;
	strcpy(t->name, "test!!!");    //strcpy = stringcopy
	return t;
}

void print_struct(test **t)
{
	printf("num_int=%d\n", (*t)-> num_int);

	printf("num_float=%f\n", (*t)-> num_float);

	printf("num_double=%lf\n", (*t)-> num_double);

	printf("name=%s\n", (*t)-> name);
}

int main(void)
{
	test *t=NULL;
	t= init_struct();
	print_struct(&t);
	
	free(t);    // 동적할당 다 썼으면 해줘야 함..?

	return 0;
}	


