#include"acqResults.h"
#include<stdio.h>

void AcqResults()
{
	int i;
	float j;
	FILE *fptestpm,*fptestcf,*fptestcp;

	fptestcf = fopen("C:\\Users\\home\\Desktop\\Results\\Cresults\\testvarcf.txt","rt");
	fptestcp = fopen("C:\\Users\\home\\Desktop\\Results\\Cresults\\testvarcp.txt","rt");
	fptestpm = fopen("C:\\Users\\home\\Desktop\\Results\\Cresults\\testvarpm.txt","rt");
	
	//printf("%u",fptestcf);
	for(i=0;i<14;i++)
	{
		fscanf(fptestcf,"%f",&j);
		acqresults[i].carrFreq = j;
		
		fscanf(fptestcp,"%f",&j);
		acqresults[i].codePhase = j;
		
		fscanf(fptestpm,"%f",&j);
		acqresults[i].peakMetric = j;
	}
	//printf("%u",fptestcf);
	fclose(fptestcf);
	fclose(fptestcp);
	fclose(fptestpm);
}
	
