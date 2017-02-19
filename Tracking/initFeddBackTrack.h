#include"showChannelStatus.h"
struct Feedback_Parameters
{
	double codeFreq;
	double remCodePhase;
	double remCarrPhase;
	double oldCodeNco;
	double oldCodeError;
	double oldCarrNco;
	double oldCarrError;
	double I_P;
	double I_E;
	double I_L;
	double Q_P;
	double Q_E;
	double Q_L;
	double pllDiscrFilt;
	double pllDiscr;
	double dllDiscrFilt;
	double dllDiscr;
	double carrFreq;
	double carrFreqBasis;
	double absoluteSample;
	short PRN;
}Feedback_parameters[7];

struct Feedback_Parameters * initFeddBackTrack(Settings s1,struct channel Ch[]);
