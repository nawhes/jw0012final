#include <stdio.h>
void master(void) {
	printf("master!!\n");
}
void printname(void) {
	printf("nawhes!!\n");
}

void second(void) {
	printf("second\n");
}

void printname(void) {
	printf("nawhes\n");
}

int main(void) {
	printname();
	return 0;
}
