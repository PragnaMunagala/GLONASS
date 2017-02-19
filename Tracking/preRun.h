#include "settings.h"
#include"acqResults.h"
extern Settings s1;
struct channel
{
	short PRN;
	double acquiredFreq;
	float codePhase;
	char status;
}Channel[7];

void preRun(struct acqResults acqresults[],Settings s1);