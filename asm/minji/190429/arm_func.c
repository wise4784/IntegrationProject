#include <stdio.h>

// arm-linux-gnueabi-gcc arm_func.c
// 일반 gcc로 하면 x86용 실행파일이 나오므로 
// arm용 실행파일이 나오지 못한다. (컴퓨터는 x86이니까)
// qemu-arm-static -> x86에서도 arm 파일이 실행 가능하도록 해 준다.
// binary translation : binary 기계어 -> x86 translation
// binary간의 변환 가능함
// gdb-multiarch로 디버깅

int mult(int num)
{
	return num * 2;
}

int main(void)
{
	int num = 3, res;

	res = mult(num);
	printf("res = %d\n", res);

	return 0;
}
