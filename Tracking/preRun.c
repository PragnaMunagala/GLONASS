//#include "settings.h"
#include "preRun.h"
#include<stdio.h>

void preRun(struct acqResults acqresults[],Settings s1)
{
	unsigned int noOfVisSat,min;
	int i,a,j,PRNindexes[14],b,c;
	double peakMetric[14],swap,junk[14],carrFreq[14],codePhase[14];
	FILE *fptest5;

	for(i=0;i<NO_OF_SATLLITES;i++)
	{
		Channel[i].acquiredFreq = 0;
		Channel[i].codePhase = 0;
		Channel[i].PRN = 0;
		Channel[i].status = '-';
	}

	for(i=0;i<14;i++)
		junk[i] = acqresults[i].peakMetric;

	 for (i = 0 ; i < 14; i++)
	 {
		for (j = 0 ; j < 14 - i - 1; j++)
		{
			if (junk[j] < junk[j+1]) /* For decreasing order use < */
			{
				swap       = junk[j];
				junk[j]   = junk[j+1];
				junk[j+1] = swap;

			}
		}
	 }

	 /*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
				for(i=0;i<14;i++)
				{
					fprintf(fptest5,"%f\n",junk[i]);
				}
				fclose(fptest5);*/

	for(i=0;i<14;i++)
	{
		 for(j=0;j<14;j++)
		 {
			 if(junk[i] == acqresults[j].peakMetric)
				 PRNindexes[i] = j;
		 }
	}

	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
				for(i=0;i<14;i++)
				{
					fprintf(fptest5,"%d\n",PRNindexes[i]);
				}
				fclose(fptest5);*/
	noOfVisSat = 0;
	for(i=0;i<14;i++)
	{
		if(acqresults[i].carrFreq!=0)
			noOfVisSat++;	
	}

	if(s1.numberOfChannels < noOfVisSat)
		min = s1.numberOfChannels;
	else
		min = noOfVisSat;

	for(i=0;i<min;i++)
	{
		Channel[i].PRN          = PRNindexes[i];
		Channel[i].acquiredFreq = acqresults[PRNindexes[i]].carrFreq;
		Channel[i].codePhase    = acqresults[PRNindexes[i]].codePhase;
		Channel[i].status       = 'T';
	}

	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
				for(i=0;i<7;i++)
				{
					fprintf(fptest5,"%d\t%f\t%f\n",Channel[i].PRN,Channel[i].acquiredFreq,Channel[i].codePhase);
				}
				fclose(fptest5);*/

}


