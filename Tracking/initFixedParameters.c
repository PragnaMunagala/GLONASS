#include"initFixedParameters.h"
#include"calcLoopCoef.h"

struct FixedParameters *initFixedParameters(Settings s1)
{
	//float tau1Code,tau2Code,tau1Carr,tau2Carr;
	float *taucode,*taucarr;
	char i;
	for(i=0;i<s1.numberOfChannels;i++)
	{
		fixedParameters[i].earlyLateSpc = s1.dllCorrelatorSpacing;

		//Summation interval
		fixedParameters[i].PDIcode = 0.001;

		//Calculate filter coefficient values
		taucode = calcLoopCoef(s1.dllNoiseBandwidth,s1.dllDampingRatio,1.0);
		fixedParameters[i].tau1code = *taucode;
		fixedParameters[i].tau2code = *++taucode;
	
		//--- PLL variables --------------------------------------------------------
	
		//Summation interval
		fixedParameters[i].PDIcarr = 0.001;

		//Calculate filter coefficient values
		taucarr = calcLoopCoef(s1.pllNoiseBandwidth,s1.pllDampingRatio,0.25);
		fixedParameters[i].tau1carr = *taucarr;
		fixedParameters[i].tau2carr = *++taucarr;  
	}
	return fixedParameters;
}