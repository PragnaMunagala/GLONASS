#include<stdio.h>
//#include "initFeddBackTrack.h"
//#include"initFixedParameters.h"
//#include"trackIter.h"
#include"generateCACode.h"
#include"initTrackResults.h"
#include<stdlib.h>

Settings s1;
short channelNr,**CAcode,g=0;
long z=0;

void main()
{
	int subFrameStart[7],j,i,s=0,loopCnt;
	double codePeriods;
	char dataAdaptCoeff,channelNr,a,svnum;
	short *Cacode;
	FILE *fid,*fptest,*fptest1,*fptest2,*fptest3,*fptest4,*fptest5,*fptest6,*fptest7,*fptest8,*fptest9,*fptest10,*fptest11,*fptest12;
	struct Feedback_Parameters *FEEDBACK_PARAMETERS,FeedBackParameters[7];
	struct FixedParameters *FixedParameters;
	struct trackResults *trackResults;

	//codePeriods = s1.msToProcess;
	Cacode = (short *)calloc(513,sizeof(short));
	//fp = fopen("C:\\Users\\home\\Desktop\\PROJECT\\GlonassSecondFiles\\Acquisition\\FFF005.DAT","rb");
	CAcode = (short **)malloc(14 * sizeof(short *));
	for(i = 0; i < 14; i++)
	{
		CAcode[i] = (short *) malloc(511 * sizeof(short));
	}
	
	printf("Starting processing...");
	
	fptest12 = fopen("C:\\Users\\home\\Documents\\MATLAB\\PRN.txt","wt");
	//printf("%u",fid);

	fid = fopen("C:\\Users\\home\\Desktop\\PROJECT\\GlonassSecondFiles\\Acquisition\\FFF005.DAT","rb");

	//printf("%u",fid);
	
	//Initialize the multiplier to adjust for the data type
	if (s1.fileType==1)
		dataAdaptCoeff=1;
	else
		dataAdaptCoeff=2;

	AcqResults();
	initSettings();
    
	Cacode = generateCAcode(Cacode);

	for(i=0;i<14;i++)
	{
		for(j=0;j<511;j++)
		{
			CAcode[i][j] = Cacode[j];
		}
	}
	
	// Initialize channels and prepare for the run
	// Start further processing only if a GNSS signal was acquired (the
	// field FREQUENCY will be set to 0 for all not acquired signals)
	
	for(i=0;i<14;i++)
	{
      if (acqresults[i].carrFreq != 0)
		{
			a = 1;
		}
	}
	if(a==1)
	{
		preRun(acqresults,s1);
		showChannelStatus(Channel, s1);
	}
	else
	{
		// No satellites to track, exit
		printf("No GNSS signals detected, signal processing finished.\n");
	}
    
	//for (channelNr=0;channelNr<s1.numberOfChannels;channelNr++)
		//	subFrameStart[channelNr] = 0;
		
	FEEDBACK_PARAMETERS = initFeddBackTrack(s1,Channel);
	
	trackResults = initTrackResults(s1);
	FixedParameters = initFixedParameters(s1);

	fptest = fopen("C:\\Users\\home\\Documents\\MATLAB\\I_E.txt","wt");
	fptest1 = fopen("C:\\Users\\home\\Documents\\MATLAB\\I_L.txt","wt");
	fptest2 = fopen("C:\\Users\\home\\Documents\\MATLAB\\I_P.txt","wt");
	fptest3 = fopen("C:\\Users\\home\\Documents\\MATLAB\\Q_E.txt","wt");
	fptest4 = fopen("C:\\Users\\home\\Documents\\MATLAB\\Q_L.txt","wt");
	fptest5 = fopen("C:\\Users\\home\\Documents\\MATLAB\\Q_P.txt","wt");
	fptest6 = fopen("C:\\Users\\home\\Documents\\MATLAB\\pllDiscr.txt","wt");
	fptest7 = fopen("C:\\Users\\home\\Documents\\MATLAB\\pllDiscrFilt.txt","wt");
	fptest8 = fopen("C:\\Users\\home\\Documents\\MATLAB\\dllDiscr.txt","wt");
	fptest9 = fopen("C:\\Users\\home\\Documents\\MATLAB\\dllDiscrFilt.txt","wt");
	fptest10 = fopen("C:\\Users\\home\\Documents\\MATLAB\\absoluteSample.txt","wt");
	fptest11= fopen("C:\\Users\\home\\Documents\\MATLAB\\status.txt","wt");
	


	loopCnt = 0;
	//while (!feof(fid))  
	//{
		
		//if(loopCnt>=34000)
				//exit;
		for(i=0;i<s1.msToProcess;i++)
		{
			loopCnt = loopCnt+1;

			//if(loopCnt>=34000)
				//exit;
			if (loopCnt%100==0)
				printf("%d\n",loopCnt);
		

			for (channelNr = 0;channelNr<s1.numberOfChannels;channelNr++)
				{
					if (Channel[channelNr].PRN != 0)
					{
						trackResults[channelNr].PRN  = Channel[channelNr].PRN;
						
						//fprintf(fptest12,"%d\n",trackResults[channelNr].PRN);

						//fseek(fid,dataAdaptCoeff*(settings.skipNumberOfBytes + channel(channelNr).codePhase-1),'bof');
						FEEDBACK_PARAMETERS[channelNr].PRN = Channel[channelNr].PRN;
						FeedBackParameters[channelNr] = FEEDBACK_PARAMETERS[channelNr];
						//printf("%d %u\n",s++,fid);
						//if(z == 2496)
							//printf("");
						FeedBackParameters[channelNr] = TrackIter(fid,FeedBackParameters[channelNr],FixedParameters[channelNr],s1,Channel);
						FEEDBACK_PARAMETERS[channelNr] = FeedBackParameters[channelNr];
						trackResults[channelNr].I_E[loopCnt]    =   FeedBackParameters[channelNr].I_E;
						trackResults[channelNr].I_L[loopCnt]    =   FeedBackParameters[channelNr].I_L;
						trackResults[channelNr].I_P[loopCnt]    =   FeedBackParameters[channelNr].I_P;
						trackResults[channelNr].Q_E[loopCnt]    =   FeedBackParameters[channelNr].Q_E;
						trackResults[channelNr].Q_L[loopCnt]    =   FeedBackParameters[channelNr].Q_L;
						trackResults[channelNr].Q_P[loopCnt]    =   FeedBackParameters[channelNr].Q_P;
						trackResults[channelNr].pllDiscr[loopCnt]       =   FeedBackParameters[channelNr].pllDiscr;
						trackResults[channelNr].pllDiscrFilt[loopCnt]   =   FeedBackParameters[channelNr].pllDiscrFilt;
						trackResults[channelNr].dllDiscr[loopCnt]       =   FeedBackParameters[channelNr].dllDiscr;
						trackResults[channelNr].dllDiscrFilt[loopCnt]   =   FeedBackParameters[channelNr].dllDiscrFilt;
						trackResults[channelNr].absoluteSample[loopCnt] =   FeedBackParameters[channelNr].absoluteSample/dataAdaptCoeff;
						trackResults[channelNr].status                  =   Channel[channelNr].status;
						
						fprintf(fptest,"%f\n",trackResults[channelNr].I_E[loopCnt]);
						fprintf(fptest1,"%f\n",trackResults[channelNr].I_L[loopCnt]);
						fprintf(fptest2,"%f\n",trackResults[channelNr].I_P[loopCnt]);
						fprintf(fptest3,"%f\n",trackResults[channelNr].Q_E[loopCnt]);
						fprintf(fptest4,"%f\n",trackResults[channelNr].Q_L[loopCnt]);
						fprintf(fptest5,"%f\n",trackResults[channelNr].Q_P[loopCnt]);
						fprintf(fptest6,"%f\n",trackResults[channelNr].pllDiscr[loopCnt]);
						fprintf(fptest7,"%f\n",trackResults[channelNr].pllDiscrFilt[loopCnt]);
						fprintf(fptest8,"%f\n",trackResults[channelNr].dllDiscr[loopCnt]);
						fprintf(fptest9,"%f\n",trackResults[channelNr].dllDiscrFilt[loopCnt]);
						fprintf(fptest10,"%d\n",trackResults[channelNr].absoluteSample);
						fprintf(fptest11,"%c\n",trackResults[channelNr].status);
						
					}
				}
		}
	//}

	fclose(fptest);	
	 fclose(fid);
	
	/*fprintf('   Saving Acq & Tracking results to file trackingResults.dat\n')
	save('trackingResults.dat', 'trackResults', 'settings', 'acqResults', 'channel');
	
	// Plot all results ===================================================
	fprintf('   Ploting results...\n');
	if settings.plotTracking
		plotTracking(1:settings.numberOfChannels, trackResults, settings);
		printf("Post processing of the signal is over.\n");*/
}
