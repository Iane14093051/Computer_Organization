#include<stdio.h>
int main ()
{
int a, b;
FILE *fp;
fp = fopen("../input/1.txt", "r");  
if (fp == NULL)
    {
        printf("File does not exists \n");
        return 0;
    }
fscanf(fp,"%d", &a);
fscanf(fp,"%d", &b);
fclose(fp);

asm volatile("mul %[A], %[A], %[B]\n\t" //AssemblerTemplate
                :[A] "+r" (a)//OutputOperands, "=r" means write-only, "+r"
                :[B]"r" (b)//InputOperands
                );

printf("%d\n", a);
return 0;
}
