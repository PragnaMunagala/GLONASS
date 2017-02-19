#define NO_OF_SATLLITES 14
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
	/*double dllDampingRatio;
	double dllNoiseBandwidth;
	double dllCorrelatorSpacing;
	double pllDampingRatio;
	double pllNoiseBandwidth;
	unsigned int navSolPeriod;
	unsigned int elevationMask;
	unsigned int useTropCorr;
	unsigned int plotTracking;
	double c;
	double startOffset;*/
	unsigned short acqSatelliteList[13];
	char datatype;
}Settings;
void initSettings();