#include <stdio.h>
#include <stdlib.h>

typedef struct k
{
	int a;
	int b;
}Node;

struct kk
{
	Node* ary;
};



int main()
{
	struct kk x;
	Node y;

	

	x.ary=&y;
	
	y.a=3;

	printf("%d",x.ary->a);

	return 0;
}