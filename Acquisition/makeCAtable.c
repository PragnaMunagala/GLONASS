//#include "makeCAtable.h"
#include "settings.h"
#include "generateCAcode.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "complex.h"
extern Settings s1;
void makeCAtable(short *p2caCode, COMPLEX * p2LocalcaCode);
void makeCAtable(short *p2caCode, COMPLEX * p2LocalcaCode)
{
	
	short * codeValueIndex,x;
	unsigned short i,j;
	double ts,tc;
	int FFTLength, samplesPerCode;
	samplesPerCode = floor(s1.samplingFreq /(s1.codeFreqBasis /s1.codeLength));
	//FFTLength=pow(2,ceil(logf(samplesPerCode)/logf(2)));
	codeValueIndex = (short *) calloc(samplesPerCode,sizeof(short));
	ts = 1/s1.samplingFreq;   
	tc = 1/s1.codeFreqBasis;
	for(i=0;i<samplesPerCode;i++)
	{
		codeValueIndex[i] = ceil(i*s1.codeFreqBasis/s1.samplingFreq);
		if(i==0)
			codeValueIndex[i] = 1; 
	}

	for(i=0;i<samplesPerCode;i++)
	{
		if (codeValueIndex[i]==0)
			i=i;
		p2LocalcaCode[i].real =(double)p2caCode[codeValueIndex[i]-1];
		p2LocalcaCode[i].Imag = 0;
	}
	/*for(i=samplesPerCode;i<FFTLength;i++)
	{
		p2LocalcaCode[i].real = 0;
		p2LocalcaCode[i].Imag = 0;
	}*/
}
