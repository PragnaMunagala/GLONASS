#include "settings.h"
extern Settings s1;
void initSettings()
{
	int i;
	//s1.acqThreshold = 5.9;
	s1.msToProcess=34000;
	s1.numberOfChannels=7;
	s1.skipNumberOfBytes=16000000;
	s1.fileType=2;
	s1.IF=-3500000;
	s1.samplingFreq=16000000;
	s1.codeFreqBasis=511000;
	s1.codeLength=511;
	s1.skipAcquisition=0;
	s1.acqSearchBand=14;
	s1.acqThreshold=5;
	s1.dllDampingRatio=0.7;
	s1.dllNoiseBandwidth=2;
	s1.dllCorrelatorSpacing=0.5;
	s1.pllDampingRatio=0.7;
	s1.pllNoiseBandwidth=25;
	s1.navSolPeriod=500;
	s1.elevationMask=0;
	s1.useTropCorr=1;
	s1.plotTracking=1;
	s1.c=299792458;
	s1.startOffset=68.802;
	
	for(i=1;i<=NO_OF_SATLLITES;i++)
	{
		s1.acqSatelliteList[i]=i;
	}
	s1.datatype = sizeof(char);
}