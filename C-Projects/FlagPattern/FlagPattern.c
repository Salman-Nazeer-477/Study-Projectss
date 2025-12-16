#include<stdio.h>
int main(int argc, char *argv[])
{
	int h = 12;
	int f = h / 3;
	int w = f * 2;
	for(int i = 0; i < h; i++){
	    if(i<f)
	        for(int j = 0; j < w; j++)
	            printf("*");
	    else printf("*");
	    printf("\n");
	}
}
