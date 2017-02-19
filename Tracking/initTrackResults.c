#include"initTrackResults.h"

struct trackResults *initTrackResults(Settings s1)
{
	int i,j;
	for(i=0;i<s1.numberOfChannels;i++)
	{
		trackresults[i].status         = '-';  //No tracked signal, or lost lock

		trackresults[i].PRN = 0;
		for(j=0;j<s1.msToProcess;j++)
		{
			//The absolute sample in the record of the C/A code start:
			trackresults[i].absoluteSample[j] = 0;

			//Freq of the C/A code:
			trackresults[i].codeFreq[j]       = 0;

			//Frequency of the tracked carrier wave:
			trackresults[i].carrFreq[j]       = 0;

			// Outputs from the correlators (In-phase):
			trackresults[i].I_P[j]            = 0;
			trackresults[i].I_E[j]            = 0;
			trackresults[i].I_L[j]            = 0;

			// Outputs from the correlators (Quadrature-phase):
			trackresults[i].Q_E[j]            = 0;
			trackresults[i].Q_P[j]            = 0;
			trackresults[i].Q_L[j]            = 0;

			// Loop discriminators
			trackresults[i].dllDiscr[j]       = 0;
			trackresults[i].dllDiscrFilt[j]   = 0;
			trackresults[i].pllDiscr[j]       = 0;
			trackresults[i].pllDiscrFilt[j]   = 0;
		}
	}
	return trackresults;
}