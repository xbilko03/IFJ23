#include <stdio.h>

int main(int argc, char **argv)
{
	printf("\nBegin\n");
	//Open File
	char c;

	while((c = getc(stdin)) != EOF)
	{
		putchar(c);
	}



	printf("\nEnd\n");
	return 0;
}