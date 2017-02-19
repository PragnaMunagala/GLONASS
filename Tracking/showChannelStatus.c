#include "showChannelStatus.h"
#include<stdio.h>


void showChannelStatus(struct channel ch[],Settings s1)
{
	int channelNr;
	float a;
	printf("\n*=========*=====*===============*===========*=============*======*\n");
	printf("| Channel | PRN |   Frequency   |  Doppler  | Code Offset | Status |\n");
    printf("*=========*=====*===============*===========*=============*========*\n");

	for(channelNr=0; channelNr<s1.numberOfChannels;channelNr++)
	{
		a = ch[channelNr].acquiredFreq - (s1.IF + (((ch[channelNr].PRN)+1)*(562500)));
		if (ch[channelNr].status != '-')
			printf("|      %2d | %3d |  %12.0f |   %5.0f   |    %6f   |     %1c  |\n",channelNr,ch[channelNr].PRN,ch[channelNr].acquiredFreq,a,ch[channelNr].codePhase,ch[channelNr].status);
		else
			printf("|      %2d | --- |  ------------ |   -----   |    ------   |   Off  |\n",channelNr);
	}
	printf("*=========*=====*===============*===========*=============*========*\n\n");
}

