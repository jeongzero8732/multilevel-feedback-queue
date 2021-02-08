#include <stdio.h>

int main()
{
   int i, sum;
   i = 0;
   sum = 0;
   while ( scanf("%d", &i)!=EOF)
   {
      sum += i;
   }
   printf("정수의 합은 %d 입니다", sum);
   return 0;
}