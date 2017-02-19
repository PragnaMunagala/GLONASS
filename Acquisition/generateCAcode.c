#include "generateCAcode.h"
void generateCAcode(short * p2CAcode)
{
	int i,reg[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1},j,temp;
	for(i=0;i<511;i++)
	{
		temp=reg[4]*reg[8];
		p2CAcode[i]=-reg[6];
		for(j=8;j>=1;j--)
		{
			reg[j]=reg[j-1];
		}
		reg[0]=temp;
	}
}