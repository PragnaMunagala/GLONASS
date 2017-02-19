#define NO_OF_SATLLITES 14
#define MsToProcess 34000

typedef struct
{
	unsigned int  msToProcess;
	unsigned int numberOfChannels;
	unsigned long skipNumberOfBytes;
	unsigned short fileType;
	double IF;
	double samplingFreq;
	double codeFreqBasis;
	unsigned int codeLength;
	unsigned short skipAcquisition;
	unsigned short acqSearchBand;
	double acqThreshold;
	char acqCohIntegration;
	float dllDampingRatio;
	char dllNoiseBandwidth;
	float dllCorrelatorSpacing;
	float pllDampingRatio;
	char pllNoiseBandwidth;
    int navSolPeriod;
	char elevationMask;
	char useTropCorr;
	char plotTracking;
	double c;
	float startOffset;
	unsigned short acqSatelliteList[13];
	char datatype;
}Settings;
void initSettings();