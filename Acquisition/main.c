#include<stdio.h>
#include<conio.h>
#include"settings.h"
#include"acqresults.h"
#include"generateCAcode.h"
#include <stdlib.h>
#include "fft.h"
#include<math.h>

#define PI 3.141592653589793

unsigned int samplesPerCode;
int FFTLength;
Settings s1;
acqresults prnresults[15];

void makeCAtable(short *p2caCode, COMPLEX * p2LocalcaCode);

void main()
{
	int i,FFTLength,j,a,k,samplesPerCodeChip,excludeRangeIndex1,excludeRangeIndex2;
	FILE *fptest,*fptest2,*fp,*fptest3,*fptest4;
	short CAcode[511];
	COMPLEX *p2temp,*p2LocalcaCode,*p2caCodeFFT,*p2caCode;
	int *p2frqBins;
	double **p2results;
	int numberOfFrqBins,frequencyBinIndex,codePhase,*p2codePhaseRange;
	double p2sincarrier,p2coscarrier,*p2acqRes1,*p2acqRes2,max1,max2,p2maxres1[141],peaksize,*p2maxres2;
	char PRN = 0,dataAdaptCoeff;
	double phasePoints[80000],ts;
	COMPLEX *p2signal1, *p2signal2,*I1,*Q1,*I2,*Q2,*Z1,*Z2,*p2IQfreqDom1,*p2IQfreqDom2,*p2convCodeIQ1,*p2convCodeIQ2,*p2caCodeFreqDom,*p2ifft1,*p2ifft2;
	char *data,*p2data;
	int temp1,temp2,temp3;
	printf("Acquiring Satellites\n");

	//opening the GLONASS signal file
	fp=fopen("C:\\Users\\home\\Desktop\\PROJECT\\DATA\\GLONASS_Fs16MHZ_If3.5MHZ.DAT","rb");
	
	//Initialising the variables of the structure Settings 
	initSettings();

	//To generate the C/A code
	generateCAcode(CAcode);
	
	samplesPerCode = floor(s1.samplingFreq /(s1.codeFreqBasis /s1.codeLength));
	FFTLength=pow(2,ceil(logf(5*samplesPerCode)/logf(2)));
	
	p2LocalcaCode = (COMPLEX *) calloc(FFTLength, sizeof(COMPLEX));
	p2caCodeFFT = (COMPLEX *) calloc(FFTLength, sizeof(COMPLEX));
	p2caCode= (COMPLEX *) calloc(samplesPerCode, sizeof(COMPLEX));
	
	if (s1.fileType==1)
		dataAdaptCoeff=1;
	else
		dataAdaptCoeff=2;

	//to skip the required number of bytes in the signal to be read
	fseek(fp,dataAdaptCoeff*s1.skipNumberOfBytes,1);

	data = (char *) calloc(11*samplesPerCode*2*dataAdaptCoeff,sizeof(char));
	p2signal1 = (COMPLEX *) calloc(5*samplesPerCode, sizeof(COMPLEX));
	p2signal2 = (COMPLEX *) calloc(5*samplesPerCode, sizeof(COMPLEX));
	p2caCodeFreqDom = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));

	numberOfFrqBins = (s1.acqSearchBand * 10) + 1;
	ts = 1/s1.samplingFreq;
	

	//reading the data from the signal file
	if (dataAdaptCoeff==2)
	{
		a=fread(data,1,11*dataAdaptCoeff*samplesPerCode,fp);
	}

	p2data = data;

	for(j=0;j<5*samplesPerCode;j++)
	{
		p2signal1[j].real = (double) *p2data++;
		p2signal1[j].Imag = -(double) *p2data++;
	}

	for(j=0;j<5*samplesPerCode;j++)
	{
		p2signal2[j].real = (double) *p2data++;
		p2signal2[j].Imag = -(double) *p2data++;
	}
	/*fptest3=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar3.txt","wt"); 
		
		
			for(i=0;i<5*samplesPerCode;i++)
			{
				fprintf(fptest3,"%f\t%f\n",p2signal1[i].real,p2signal1[i].Imag);
			}

		
		fclose(fptest3);*/
	
	//to generate the CAtable for the C/A code
	makeCAtable(CAcode,p2caCode);


	//to generate the CAtable 
	for(i=0,j=0;(j<=samplesPerCode) && (i<=samplesPerCode*5);j++,i++)
	{
		if(j==samplesPerCode)
			j = 0;
		p2LocalcaCode[i] = p2caCode[j];
	}

	for(i=5*samplesPerCode;i<FFTLength;i++)
	{
		p2LocalcaCode[i].real = 0;
		p2LocalcaCode[i].Imag = 0;
	}

	fft_ifft(p2LocalcaCode,p2caCodeFFT,FFTLength,1);
	
	conjugate(p2caCodeFFT,p2caCodeFreqDom);

	for(PRN = 1 ; PRN <= NO_OF_SATLLITES; PRN++)
	{
		p2frqBins = (int *) calloc(numberOfFrqBins,sizeof(int));
		I1 = (COMPLEX *)calloc(5*samplesPerCode,sizeof(COMPLEX));
		Q1 = (COMPLEX *)calloc(5*samplesPerCode,sizeof(COMPLEX));
		I2 = (COMPLEX *)calloc(5*samplesPerCode,sizeof(COMPLEX));
		Q2 = (COMPLEX *)calloc(5*samplesPerCode,sizeof(COMPLEX));
		Z1 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		Z2 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2IQfreqDom1 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2IQfreqDom2 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2convCodeIQ1 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2convCodeIQ2 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2ifft1 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2ifft2 = (COMPLEX *)calloc(FFTLength,sizeof(COMPLEX));
		p2acqRes1 = (double *)calloc(FFTLength,sizeof(double));
		p2acqRes2 = (double *)calloc(FFTLength,sizeof(double));
		p2codePhaseRange = (int *)calloc((samplesPerCode),sizeof(int));
		p2maxres2 = (double *)calloc(samplesPerCode,sizeof(double));
	
		p2results = (double **)malloc(141 * sizeof(double *));
		for(i = 0; i < numberOfFrqBins; i++)
		{
			p2results[i] = (double *) malloc(samplesPerCode * sizeof(double));
		
		}

		for(k=0;k<numberOfFrqBins;k++)
		{
			p2frqBins[k] = (s1.IF+PRN*0.5625e6)-(s1.acqSearchBand/2)*1000+100*(k);
			for(i=0;i<5*samplesPerCode;i++)
			{
					phasePoints[i] = 2*PI*i*ts;
					p2sincarrier = sin(p2frqBins[k] * phasePoints[i]);
					p2coscarrier = cos(p2frqBins[k] * phasePoints[i]);
					I1[i].real = p2sincarrier*p2signal1[i].real;
					I1[i].Imag = p2sincarrier*p2signal1[i].Imag;
					Q1[i].real = p2coscarrier*p2signal1[i].real;
					Q1[i].Imag = p2coscarrier*p2signal1[i].Imag;
					I2[i].real = p2sincarrier*p2signal2[i].real;
					I2[i].Imag = p2sincarrier*p2signal2[i].Imag;
					Q2[i].real = p2coscarrier*p2signal2[i].real;
					Q2[i].Imag = p2coscarrier*p2signal2[i].Imag;
			}
			
			c_addsignals(I1,Q1,Z1);
			c_addsignals(I2,Q2,Z2);

			for(i=5*samplesPerCode;i<FFTLength;i++)
			{
				Z1[i].real = 0;
				Z1[i].Imag = 0;
				Z2[i].real = 0;
				Z2[i].Imag = 0;
			}

			fft_ifft(Z1,p2IQfreqDom1,FFTLength,1);
			fft_ifft(Z2,p2IQfreqDom2,FFTLength,1); 
			
			/*fptest3=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar3.txt","wt"); 
		
		
			for(i=0;i<FFTLength;i++)
			{
				fprintf(fptest3,"%f\t%f\n",p2IQfreqDom1[i].real,p2IQfreqDom1[i].Imag);
			}

		
		fclose(fptest3);*/

			c_multsignals(p2IQfreqDom1,p2caCodeFreqDom,p2convCodeIQ1);
			c_multsignals(p2IQfreqDom2,p2caCodeFreqDom,p2convCodeIQ2);

			fft_ifft(p2convCodeIQ1, p2ifft1,FFTLength,-1);
			fft_ifft(p2convCodeIQ2, p2ifft2,FFTLength,-1);
			
			c_absolute(p2ifft1,p2acqRes1);
			c_absolute(p2ifft2,p2acqRes2);

			max1 = p2acqRes1[0];
			max2 = p2acqRes2[0];
			for(i=0;i<samplesPerCode;i++)
			{
				if(max1 < p2acqRes1[i])
					max1 = p2acqRes1[i];
				if(max2 < p2acqRes2[i])
					max2 = p2acqRes2[i];
			}
	
			if(max1 > max2)
			{
				for(j=0;j<samplesPerCode;j++)
					p2results[k][j] = p2acqRes1[j];
			}
			else
				for(j=0;j<samplesPerCode;j++)
					p2results[k][j] = p2acqRes2[j];
		}

	/*fptest3=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar3.txt","wt"); 
		for(k=0;k<141;k++)
		{
			for(i=0;i<samplesPerCode;i++)
			{
				fprintf(fptest3,"%f\t",p2results[k][i]);
			}
			fprintf(fptest3,"\n");
		}
		fclose(fptest3);*/
	
		for(i=0,peaksize=0;i<(numberOfFrqBins);i++)
		{
			for(j=samplesPerCode,p2maxres1[i] = 0;j>0;j--)
			{
				if(p2results[i][j-1] > p2maxres1[i])
					p2maxres1[i] = p2results[i][j-1];		
			}
			if( ( peaksize < p2maxres1[i]))
			{
				peaksize = p2maxres1[i];
				frequencyBinIndex = i+1;
			}
		}
		
		for(j=0,peaksize=0;j<(samplesPerCode);j++)
		{
			for(i=numberOfFrqBins,p2maxres2[i] = 0;i>0;i--)
			{
				if(p2results[i-1][j] > p2maxres2[j])
					p2maxres2[j] = p2results[i-1][j];		
			}
			if( ( peaksize < p2maxres2[j]))
			{
				peaksize = p2maxres2[j];
				codePhase = j+1;
			}
		}
	
		samplesPerCodeChip = floor(s1.samplingFreq/s1.codeFreqBasis);
		excludeRangeIndex1 = codePhase - samplesPerCodeChip;
		excludeRangeIndex2 = codePhase + samplesPerCodeChip;
    
		if (excludeRangeIndex1 < 2)
		{
		   for(i = excludeRangeIndex2;i<(samplesPerCode + excludeRangeIndex1);i++)
				p2codePhaseRange[i] = i;
		}
            
		else if (excludeRangeIndex2 >= samplesPerCode)
		{
			for(i=(excludeRangeIndex2 - samplesPerCode);i<=excludeRangeIndex1;i++)
				p2codePhaseRange[i] = i;
		}
		
		else
		{
			for(i=1,j=1;(i<=samplesPerCode) ;i++)
			{
				if((i<=excludeRangeIndex1)||(i>=excludeRangeIndex2))
				{
						p2codePhaseRange[j] = i;
						j++;
				}
			}
			temp2 = j - 1;
		}

		for(i=1,j=1,temp1 = p2results[frequencyBinIndex-1][j];i<=temp2;i++,j=p2codePhaseRange[i])
		{
			if((i>1) && (temp1 < p2results[frequencyBinIndex-1][j]))
			{
				temp1 = p2results[frequencyBinIndex-1][j];
				temp3 = j;
			}
		}
	
		prnresults[PRN].peakMetric = peaksize/temp1;
		if ( prnresults[PRN].peakMetric > s1.acqThreshold)
		{
			printf("%d\n",PRN);
			prnresults[PRN].carrfreq = p2frqBins[frequencyBinIndex-1];
			prnresults[PRN].codePhase = codePhase;
		}
		
		free(p2frqBins);
		free(I1);
		free(Q1);
		free(I2);
		free(Q2);
		free(Z1);
		free(Z2);
		free(p2IQfreqDom1);
		free(p2IQfreqDom2);
		free(p2convCodeIQ1);
		free(p2convCodeIQ2);
		free(p2ifft1);
		free(p2ifft2);
		free(p2acqRes1);
		free(p2acqRes2);
		free(p2codePhaseRange);
		free(p2maxres2);
		free(p2results);
	}
}



	
