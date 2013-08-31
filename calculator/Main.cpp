#include <stdio.h>
#include "grammar.h"
#include "scanner.h"
#include "analyser.h"

void pause()
{
	printf("Press Enter to continue\r\n");
	getchar();
}

int main()
{
	//double d = (((-(1+2))*3)/4)+10-1-(-1)+((0.5*(-1.2))/0.999);//=7.1493994

	double d = -2/-2;
	Grammar g;
	pause();
	Scanner s(&g);
	pause();

	printf("\r\n");
	return 0;
}