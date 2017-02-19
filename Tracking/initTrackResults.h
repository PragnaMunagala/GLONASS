#include"trackIter.h"

struct trackResults
{
	char status;
	//Settings s1;
	int absoluteSample[MsToProcess];
	double codeFreq[MsToProcess];
	double carrFreq[MsToProcess];
	double I_P[MsToProcess];
	double I_E[MsToProcess];
	double I_L[MsToProcess];
	double Q_E[MsToProcess];
	double Q_P[MsToProcess];
	double Q_L[MsToProcess];
	double dllDiscr[MsToProcess];
	double dllDiscrFilt[MsToProcess];
	double pllDiscr[MsToProcess];
	double pllDiscrFilt[MsToProcess];
	short PRN;
}trackresults[7];

struct trackResults *initTrackResults(Settings s1);