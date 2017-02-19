#include "acqresults.h"
extern acqresults ar1;
void acqResults()
{
	char i;
	for(i=0;i<14;i++)
	{
		ar1.carrfreq[i]=0;
		ar1.codePhase[i]=0;
		ar1.peakMetric[i]=0;
	}
}