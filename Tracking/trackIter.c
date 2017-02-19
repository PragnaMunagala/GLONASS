#include"trackIter.h"
#include"complex.h"
//#include"initFixedParameters.h"
#include<math.h>
#include<stdlib.h>
long double PI=3.141592653589793;

extern Settings s1;
extern short **CAcode;
extern z;

struct Feedback_Parameters TrackIter(FILE *fp,struct Feedback_Parameters FB,struct FixedParameters FP,Settings s1,struct channel ch[])
{
	double codePhaseStep,blksize,k,*tcode,*tcode2,*earlyCode,*lateCode,*promptCode,*time,*iBasebandSignal,I_E = 0,Q_E = 0,I_P = 0,Q_P = 0,I_L = 0,Q_L = 0,codeNco,carrNco,codeError,carrError;
	FILE *fptest5;
	long double *qBasebandSignal,*trigarg;
	float samplesRead; 
	short i,j,*caCode;
	char dataAdaptCoeff,*data,*rawsignal1,*rawsignal2,*p2data;
	COMPLEX *rawsignal;
	
	caCode = (short *)calloc(513,sizeof(short));
	tcode = (double *)calloc(16050,sizeof(double));
	tcode2 = (double *)calloc(16050,sizeof(double));
	earlyCode = (double *)calloc(16050,sizeof(double));
	lateCode = (double *)calloc(16050,sizeof(double));
	promptCode = (double *)calloc(16050,sizeof(double));
	time = (double *)calloc(16050,sizeof(double));
	trigarg = (long double *)calloc(16050,sizeof(long double));
	//rawsignal1 = (char *)calloc(16050,sizeof(char));
	//rawsignal2 = (char *)calloc(16050,sizeof(char));
	qBasebandSignal = (long double *)calloc(16050,sizeof(long double));
	iBasebandSignal = (double *)calloc(16050,sizeof(double));
	data = (char *)calloc(32100,sizeof(char));

	rawsignal = (COMPLEX *)calloc(16500,sizeof(COMPLEX));
	if(rawsignal==NULL)
		printf("insufficient memory");
	//fp = fopen("C:\\Users\\home\\Desktop\\PROJECT\\GlonassSecondFiles\\Acquisition\\FFF005.DAT","rb");

	i = FB.PRN;

	for(j = 1;j < 512;j++)
	{
		caCode[j] = CAcode[i][j-1];
	}
    
	//Then make it possible to do early and late versions
    
	caCode[0] = CAcode[i][510];
	caCode[512] = CAcode[i][0];

	codePhaseStep = FB.codeFreq / s1.samplingFreq;

   // Update the phasestep based on code freq (variable) and
   //sampling freq uency (fixed)

	blksize = ceil((s1.codeLength-FB.remCodePhase) / codePhaseStep);

	if (s1.fileType==1)
		dataAdaptCoeff=1;
	else
		dataAdaptCoeff=2;
	//printf(" loop %u",fp);


	// Read in the appropriate number of samples to process this interation
	fseek(fp,FB.absoluteSample,0);

	//printf(" fseek  %u",fp);
	
	samplesRead = fread(data,1,dataAdaptCoeff*blksize,fp);

	p2data = data;
	
	//FB.absoluteSample = samplesRead;
	//printf("%f",FB.absoluteSample);
	FB.absoluteSample = ftell(fp);

	if (dataAdaptCoeff==2)
	{
		for(i = 0;i < (samplesRead/2);i++)
		{
			rawsignal[i].real = (double) *p2data++;
			rawsignal[i].Imag = (double) *p2data++;
		}

		//if(z==2497)
			//printf("%d",i);


		/*for(i = 0;i < (samplesRead/2);i++)
		{
			rawsignal[i].real = (double)rawsignal1[i];
			rawsignal[i].Imag = (double)rawsignal2[i];
		}*/
	}
	//free(data);
	
	
	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<(samplesRead/2);i++)
	{
		fprintf(fptest5,"%f\t%f\n",rawsignal[i].real,rawsignal[i].Imag);
	}
	fclose(fptest5);*/
	
	if (samplesRead != dataAdaptCoeff*blksize)
	{
		printf("Not able to read the specified number of samples  for tracking, exiting");
		fclose(fp);
		return FB;
	}
	
	for(k = (FB.remCodePhase-FP.earlyLateSpc),j=0;k <=(((blksize-1)*codePhaseStep+FB.remCodePhase+FP.earlyLateSpc)+1) && j<16000;k+=codePhaseStep,j++)
	{
				
		tcode[j] = k;
		tcode2[j] = ceil(tcode[j]) + 1;
		earlyCode[j] = caCode[(int)(tcode2[j]-1)];
	}
	
	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<16000;i++)
	{
		fprintf(fptest5,"%f\n",earlyCode[i]);
	}
	fclose(fptest5);*/

	for(k = (FB.remCodePhase+FP.earlyLateSpc),j=0;(k <= ((blksize-1)*codePhaseStep+FB.remCodePhase+FP.earlyLateSpc)+1) && j<16000;k+=codePhaseStep,j++)
	{
		tcode[j] = k;
		tcode2[j] = ceil(tcode[j]) + 1;
		lateCode[j] = caCode[(int)(tcode2[j]-1)];
	}

	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<16000;i++)
	{
		fprintf(fptest5,"%f\n",lateCode[i]);
	}
	fclose(fptest5);*/

	for(k = (FB.remCodePhase),j=0;(k <= (((blksize-1)*codePhaseStep+FB.remCodePhase)+1)) ;k+=codePhaseStep,j++)
	{
		tcode[j] = k;
		tcode2[j] = ceil(tcode[j]) + 1;
		promptCode[j] = caCode[(int)(tcode2[j]-1)];
	}
	
	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<16000;i++)
	{
		fprintf(fptest5,"%f\n",promptCode[i]);
	}
	fclose(fptest5);*/

	FB.remCodePhase = tcode[(int)(blksize-1)] + codePhaseStep - 511.0;

	//Generate the carrier frequency to mix the signal to baseband -----------
	// Get the argument to sin/cos functions
	
	
	for(k = 0;k < blksize;k++)
	{
		time[(int)k] = k/s1.samplingFreq;
		trigarg[(int)k] = ((FB.carrFreq* 2.0 * PI) * time[(int)k]) + FB.remCarrPhase;
		
		// Generate the six standard accumulated values ---------------------------
		// First mix to baseband
		qBasebandSignal[(int)k] = (cos(trigarg[(int)k])*rawsignal[(int)k].real ) - (sin(trigarg[(int)k])*rawsignal[(int)k].Imag) ;
		iBasebandSignal[(int)k] = (cos(trigarg[(int)k])*rawsignal[(int)k].Imag ) + (sin(trigarg[(int)k])*rawsignal[(int)k].real) ;
		
		//Now get early, late, and prompt values for each
		I_E = I_E + (earlyCode[(int)k] * iBasebandSignal[(int)k]);
		Q_E = Q_E + (earlyCode[(int)k] * qBasebandSignal[(int)k]);
		I_P = I_P + (promptCode[(int)k] * iBasebandSignal[(int)k]);
		Q_P = Q_P + (promptCode[(int)k] * qBasebandSignal[(int)k]);
		I_L = I_L + (lateCode[(int)k] * iBasebandSignal[(int)k]);
		Q_L = Q_L + (lateCode[(int)k] * qBasebandSignal[(int)k]);
	}
	if(rawsignal !=NULL)
	free(rawsignal);
	
	time[16000] = k/s1.samplingFreq;
	trigarg[16000] = ((FB.carrFreq * 2.0 * PI) * time[16000]) + FB.remCarrPhase;
	//printf("%20.16f\n",PI);

	/*fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<=blksize;i++)
	{
		fprintf(fptest5,"%20.10f\n",(time[i]));
	}
	fclose(fptest5);
	
	fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<=blksize;i++)
	{
		fprintf(fptest5,"%20.12f\n",(trigarg[i]));
	}
	fclose(fptest5);
	
	fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<blksize;i++)
	{
		fprintf(fptest5,"%20.10f\n",qBasebandSignal[i]);
	}
	fclose(fptest5);
	
	fptest5=fopen("C:\\Users\\home\\Documents\\MATLAB\\testvar5.txt","wt"); 
	for(i=0;i<blksize;i++)
	{
		fprintf(fptest5,"%20.10f\n",iBasebandSignal[i]);
	}
	fclose(fptest5);*/
	
	FB.remCarrPhase = (trigarg[(int)(blksize)]-(int) (trigarg[(int)(blksize)]/((2 * PI)))*(2 * PI));
	
	// Find PLL error and update carrier NCO ----------------------------------

	//Implement carrier loop discriminator (phase detector)
	carrError = atan(Q_P / I_P) / (2.0 * PI);

	//Implement carrier loop filter and generate NCO command
	carrNco = FB.oldCarrNco + (FP.tau2carr/FP.tau1carr) * (carrError - FB.oldCarrError) + carrError * (FP.PDIcarr/FP.tau1carr);
	FB.oldCarrNco   = carrNco;
	FB.oldCarrError = carrError;

	//Modify carrier freq based on NCO command
	FB.carrFreq = FB.carrFreqBasis + carrNco;

	// Find DLL error and update code NCO -------------------------------------
	codeError = (sqrt(I_E * I_E + Q_E * Q_E) - sqrt(I_L * I_L + Q_L * Q_L)) / (sqrt(I_E * I_E + Q_E * Q_E) + sqrt(I_L * I_L + Q_L * Q_L));

	// Implement code loop filter and generate NCO command
	codeNco = FB.oldCodeNco + (FP.tau2code/FP.tau1code) * (codeError - FB.oldCodeError) + codeError * (FP.PDIcode/FP.tau1code);
	FB.oldCodeNco   = codeNco;
	FB.oldCodeError = codeError;

	FB.dllDiscr = FB.oldCodeError;
	FB.dllDiscrFilt = FB.oldCodeNco;
	FB.pllDiscr = FB.oldCarrError;
	FB.pllDiscrFilt = FB.oldCarrNco;
	
	//printf("%f\n",FB.remCodePhase);
	// Modify code freq based on NCO command
	FB.codeFreq = s1.codeFreqBasis - codeNco;
	FB.I_P = I_P;
	FB.I_E = I_E;
	FB.I_L = I_L;
	FB.Q_P = Q_P;
	FB.Q_E = Q_E;
	FB.Q_L = Q_L;

	free(caCode);
	free(tcode);
	free(tcode2);
	free(earlyCode);
	free(lateCode);
	free(promptCode); 
	free(time);
	free(trigarg);
	/*free(rawsignal1);
	free(rawsignal2);*/
	free(qBasebandSignal);
	free(iBasebandSignal);
	free(data);
	//free(rawsignal);
	//fclose(fp);
	return FB;
}