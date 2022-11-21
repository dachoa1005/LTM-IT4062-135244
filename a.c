// C program to demonstrate the problem when
// fgets()/gets() is used after scanf()
#include <stdio.h>

int main()
{
	//scan an interger x -> print x, then scan a string s
	// -> print s
	int x;
	char s[100];
	scanf("%d", &x);
	printf("%d", x);
	scanf("%s", s);
	printf("%s", s);
	return 0;
	
}
