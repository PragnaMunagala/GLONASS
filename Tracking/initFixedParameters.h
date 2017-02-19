#include "initFeddBackTrack.h"
struct FixedParameters
{
	float earlyLateSpc;
	float PDIcode;
	float tau1code;
	float tau2code;
	float PDIcarr;
	float tau1carr;
	float tau2carr;
}fixedParameters[7];

struct FixedParameters *initFixedParameters(Settings s1);