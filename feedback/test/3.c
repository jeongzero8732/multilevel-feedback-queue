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
   printf("������ ���� %d �Դϴ�", sum);
   return 0;
}