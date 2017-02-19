//#include "acquisition.h"
#include "settings.h"
#include "makeCAtable.h"
#include "fft.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define PI 3.141592653589793
extern Settings s1;
#include "acqresults.h"
extern int * caCodesTable;
extern acqresults ar1;

 void acquisition(COMPLEX * longsignal)
{
	//acqresults ar2={{0},{0},{0}};
	unsigned int samplesPerCode,numberOfFrqBins,sizeOfResults,samplesPerCodeChip,excludeRangeIndex1,excludeRangeIndex2,codePhaseRange,frequencyBinIndex;
	COMPLEX S1,S2,* signal1,* signal2;
	int *p2longsignal=(int *)longsignal;
	int * frqBins,i,j,* frqBinIndex;
	float ts,phasePoints[80000],max1,max2,* acqRes1,* acqRes2,peaksize,maxres1[140],results[140][16000],codePhase;
	unsigned short PRN;
	COMPLEX *sinCarr,*cosCarr,* I1,* Q1,* I2,* Q2,* Z1,* Z2,* IQfreqDom1,* IQfreqDom2,* cacodetable,* CACODEFFT,* caCodeFreqDom,* convCodeIQ1,* convCodeIQ2,* IFFTconvCodeIQ1,* IFFTconvCodeIQ2;
	signal1=&S1;
	signal2=&S2;
	samplesPerCode = floor(s1.samplingFreq /(s1.codeFreqBasis /s1.codeLength));
	
	for(i=0;i<5*samplesPerCode;i++)
	{
		signal1[i]=longsignal[i];
	}


	for(i=5*samplesPerCode;i<10*samplesPerCode;i++)
	{
		signal2[i]=longsignal[i];
	}


	ts = 1/s1.samplingFreq; 
	for(i=0;i<=(5*samplesPerCode-1);i++)
	{
		phasePoints[i] = 2*PI*i*ts;
	}

	/*for(i=0,j=0;(j<=samplesPerCode) && (i<=samplesPerCode*5);j++,i++)
	{
		if(j==samplesPerCode)
			j=0;
		cacodetable->real=caCodesTable[j];
		cacodetable++;
	}*/

	numberOfFrqBins = (s1.acqSearchBand * 10) + 1;  //floor not needed
	//sizeOfResults = samplesPerCode-numberOfFrqBins;
	//results = (float *)calloc(numberOfFrqBins*samplesPerCode,sizeof(int));
	frqBins = (int *)calloc(numberOfFrqBins,sizeof(int));
	frqBinIndex = (int *)calloc(numberOfFrqBins,sizeof(int));
	sinCarr = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	cosCarr = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	I1 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	Q1 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	I2 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	Q2 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	Z1 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	Z2 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	IQfreqDom1 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	IQfreqDom2 = (COMPLEX *)calloc(numberOfFrqBins,sizeof(int));
	cacodetable=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	caCodeFreqDom=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	CACODEFFT=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	convCodeIQ1=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	convCodeIQ2=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	IFFTconvCodeIQ1=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	IFFTconvCodeIQ2=(COMPLEX *)calloc(5*samplesPerCode,sizeof(int));
	acqRes1=(float *)calloc(5*samplesPerCode,sizeof(int));
	acqRes2=(float *)calloc(5*samplesPerCode,sizeof(int));
	for(j=1;PRN<=s1.acqSatelliteList[j];j++)
	{
		PRN=j;
		fft_ifft(cacodetable,CACODEFFT,5*samplesPerCode,1);
		for(i=0;i<5*samplesPerCode;i++)
		{
			caCodeFreqDom[i] = conjugate(CACODEFFT[i]);
		}

		for(i=1;i<=numberOfFrqBins;i++)
		{
			frqBinIndex[i]=i;
			frqBins[frqBinIndex[i]] = (s1.IF+PRN*0.5625e6)-(s1.acqSearchBand/2)*1000+100*(frqBinIndex[i]-1);
			sinCarr->real = sin(frqBins[frqBinIndex[i]] * phasePoints[i]);
			cosCarr->real = cos(frqBins[frqBinIndex[i]] * phasePoints[i]);
			c_mult(sinCarr,signal1,I1);
			c_mult(cosCarr,signal1,Q1);
			c_mult(sinCarr,signal2,I2);
			c_mult(cosCarr,signal2,Q2);
			c_add(I1,Q1,Z1);
			c_add(I2,Q2,Z2);
			fft_ifft(Z1,IQfreqDom1,numberOfFrqBins,1);
	   		fft_ifft(Z2,IQfreqDom2,numberOfFrqBins,1); 
			c_mult(IQfreqDom1,caCodeFreqDom,convCodeIQ1);
			c_mult(IQfreqDom2,caCodeFreqDom,convCodeIQ2);
			fft_ifft(convCodeIQ1,IFFTconvCodeIQ1,numberOfFrqBins,-1);
			fft_ifft(convCodeIQ2,IFFTconvCodeIQ2,numberOfFrqBins,-1);
			for(i=0;i<numberOfFrqBins;i++)
			{
				*acqRes1++=pow(c_abs(IFFTconvCodeIQ1++),2);
				*acqRes2++=pow(c_abs(IFFTconvCodeIQ2++),2);
			}
			max1=acqRes1[0];
			max2=acqRes2[0];
			for(i=0;i<numberOfFrqBins;i++)
			{
				if(max1<acqRes1[i])
					max1=acqRes1[i];
			}
			for(i=0;i<numberOfFrqBins;i++)
			{
				if(max2<acqRes2[i])
					max2=acqRes2[i];
			}
			if(max1>max2)
			{
				for(j=0;j<samplesPerCode;j++)
					results[frqBinIndex[i]][j]=acqRes1[i];
			}
			else
				for(j=0;j<samplesPerCode;j++)
					results[frqBinIndex[i]][j]=acqRes2[i];
		}
	}
    
	for(i=0;i<numberOfFrqBins;i++)
	{
		for(j=0;j<samplesPerCode;j++)
		{
			if(results[i][j]<results[i][j+1])
				maxres1[i]=results[i][j+1];		
		 }
		if(i && (maxres1[i] < maxres1[i-1]))
		{
			peaksize = maxres1[i-1];
			frequencyBinIndex = i-1;
		}
	}
	  
	for(j=0;j<samplesPerCode;j++)
	{
		for(i=0;i<numberOfFrqBins;i++)
		{
			if(results[i][j]<results[i+1][j])
				maxres1[j]=results[i+1][j];		
		 }
		if(j && (maxres1[j] < maxres1[j-1]))
		{
			peaksize = maxres1[j-1];
			codePhase = j-1;
		}
	}
	
	samplesPerCodeChip = floor(s1.samplingFreq/s1.codeFreqBasis);
    excludeRangeIndex1 = codePhase - samplesPerCodeChip;
    excludeRangeIndex2 = codePhase + samplesPerCodeChip;
    
    if (excludeRangeIndex1 < 2)
	{
       for(i=excludeRangeIndex2;i<=(samplesPerCode + excludeRangeIndex1);i++)
		codePhaseRange = i;
	}
            
    else if (excludeRangeIndex2 >= samplesPerCode)
	{
		for(i=(excludeRangeIndex2 - samplesPerCode);i<=excludeRangeIndex1;i++)
        codePhaseRange = i;
	}
   /* else
        codePhaseRange = [1:excludeRangeIndex1, ...
            excludeRangeIndex2 : samplesPerCode];
   
	secondPeakSize = max(results[frequencyBinIndex][codePhaseRange]);
    
    
    ar1.peakMetric[PRN] = peaksize/secondPeakSize;
    
    
    if ((peaksize/secondPeakSize) > s1.acqThreshold)
	{        
        printf("%02d", PRN);
        ar1.carrfreq[PRN]  = frqBins[frqBinIndex];
        ar1.codePhase[PRN] = codePhase;
	}*/
        
	//return(ar2);
}