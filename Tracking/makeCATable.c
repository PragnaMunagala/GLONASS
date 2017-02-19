#include "makeCAtable.h"
#include "initFixedParameters.h"
#include"generateCACode.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
extern Settings s1;
//void makeCAtable(short *p2caCode, COMPLEX * p2LocalcaCode);
void makeCAtable(struct Feedback_Parameters fb)
{
	float samplesPerCode,ts,tc;
	short *caCodesTable,PRN;
	samplesPerCode = ceil(s1.samplingFreq /(s1.codeFreqBasis / s1.codeLength));
	caCodesTable = (short *)calloc(samplesPerCode,sizeof(short));

	  //--- Find time constants --------------------------------------------------
	  ts = 1/s1.samplingFreq;   // Sampling period in sec
	  tc = 1/s1.codeFreqBasis;  // C/A chip period in sec

	  //=== For all satellite PRN-s ...
	  for(PRN = 0;PRN < 14;PRN++)
	  {
		//--- Generate CA code for given PRN -----------------------------------
		caCode = generateCAcode(PRN);
 
		%%=== Digitizing =======================================================
    
		%%--- Make index array to read C/A code values -------------------------
		%% The length of the index array depends on the sampling frequency -
		%% number of samples per millisecond (because one C/A code period is one
		%% millisecond).
		codeValueIndex = ceil((ts * (1:samplesPerCode)) / tc);
    
		%%--- Correct the last index (due to number rounding issues) -----------
		codeValueIndex(end) = 511;     %$ means end
		%%--- Make the digitized version of the C/A code -----------------------
		%% The "upsampled" code is made by selecting values form the CA code
		%% chip array (caCode) for the time instances of each sample.
		caCodesTable(PRN, :) = caCode(codeValueIndex);
	  end %% for PRN = 1:32
}