#include"initFeddBackTrack.h"
#include<stdio.h>
//#include"showChannelStatus.h"

struct Feedback_Parameters * initFeddBackTrack(Settings s1,struct channel Ch[])
{
	struct Feedback_Parameters Track[7];
	char dataAdaptCoeff,channelNr,i;
	FILE *fptest5;
	for(i=0;i<s1.numberOfChannels;i++)
	{
		Track[i].codeFreq = s1.codeFreqBasis;
		
		//define residual code phase (in chips)
		Track[i].remCodePhase  = 0.0;
		
		//define residual carrier phase
		Track[i].remCarrPhase  = 0.0;

		//code tracking loop parameters
		Track[i].oldCodeNco   = 0.0;
		Track[i].oldCodeError = 0.0;

		//carrier/Costas loop parameters
		Track[i].oldCarrNco   = 0.0;
		Track[i].oldCarrError = 0.0;
		Track[i].I_P = 0;
		Track[i].I_E = 0;
		Track[i].I_L = 0;
		Track[i].Q_P = 0;
		Track[i].Q_E = 0;
		Track[i].Q_L = 0;
		Track[i].pllDiscrFilt  = 0;
		Track[i].pllDiscr      = 0;
		Track[i].dllDiscrFilt  = 0;
		Track[i].dllDiscr      = 0;
	}

	if (s1.fileType==1)
		dataAdaptCoeff=1;
	else
		dataAdaptCoeff=2;
		
	//define carrier frequency which is used over whole tracking period
	for(channelNr=0;channelNr<s1.numberOfChannels;channelNr++)
	{
		Track[channelNr].carrFreq      = Ch[channelNr].acquiredFreq;
		Track[channelNr].carrFreqBasis = Ch[channelNr].acquiredFreq;
		Track[channelNr].absoluteSample= dataAdaptCoeff*(s1.skipNumberOfBytes + (Ch[channelNr].codePhase+1)-1);
	}

	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
				for(i=0;i<6;i++)
				{
					fprintf(fptest5,"%f\t%f\t%f\n",Track[i].carrFreq,Track[i].carrFreqBasis,Track[i].absoluteSample);
				}
				fclose(fptest5);*/
	
	for(i=0;i<s1.numberOfChannels;i++)
	{	
		Feedback_parameters[i] = Track[i];
	}
	return(&Feedback_parameters[0]);
}

