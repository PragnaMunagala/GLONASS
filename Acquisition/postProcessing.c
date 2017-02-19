#include "postProcessing.h"
#include "settings.h"
#include <stdio.h>
#include <math.h>
#include "acqresults.h"
#include "acquisition.h"
#include <stdlib.h>
#include "fft.h"
extern Settings s1;
extern acqresults ar1;

void postProcessing()
{
	unsigned short i,dataAdaptCoeff;
	unsigned int samplesPerCode;
	//int *data[10],s3[10];
	char *data;
	int intdata[300];
	COMPLEX codedata[176000],* finaldata[176000];
	int a,j,b;
	char c,d;
	FILE *fp,*fp1;
	samplesPerCode = floor(s1.samplingFreq/(s1.codeFreqBasis/s1.codeLength));
	fp=fopen("C:\\Users\\home\\Desktop\\GLONASS\\GlonassMATLABcode\\FFF005.DAT","rb");
	fp1=fopen("C:\\Users\\home\\Desktop\\GLONASS\\GlonassMATLABcode\\FFF008.DAT","wb");
	if (s1.fileType==1)
		dataAdaptCoeff=1;
	else
		dataAdaptCoeff=2;
	for(i=0;i<(dataAdaptCoeff*11*samplesPerCode)/2;i++)
		finaldata[i]=&codedata[i];
	if (fp!='\0')
	{
		fseek(fp,dataAdaptCoeff*s1.skipNumberOfBytes,1);
		if ((s1.skipAcquisition == 0))
		{
			samplesPerCode = floor(s1.samplingFreq/(s1.codeFreqBasis/s1.codeLength));
			data=(char *)calloc(dataAdaptCoeff*11*samplesPerCode,sizeof(char));
			a = fread(data,s1.datatype,dataAdaptCoeff*11*samplesPerCode,fp);
			b = fwrite(data,s1.datatype,dataAdaptCoeff*11*samplesPerCode,fp1);
			/*fclose(fp);
			fclose(fp1);
			c=0,d=0;
			fp=fopen("C:\\Users\\home\\Desktop\\GLONASS\\GlonassMATLABcode\\FFF005.DAT","rb");
			fp1=fopen("C:\\Users\\home\\Desktop\\GLONASS\\GlonassMATLABcode\\FFF008.DAT","rb");
			fseek(fp,dataAdaptCoeff*s1.skipNumberOfBytes,1);
			for(i=0;i<dataAdaptCoeff*11*100;i++)
			{
				c= getc(fp);
				d=getc(fp1);
				printf("%d %d %d\n",c,d,data[i]);
			}*/

			 if (dataAdaptCoeff==2)
			 {
				 for(i=0,j=0;i<dataAdaptCoeff*11*samplesPerCode;i+=2,j++)
				 {
					 finaldata[j]->real=data[i];
					finaldata[j]->Imag=data[i+1];
				 }
			 }
			 fclose(fp);
			 fclose(fp1);
			 printf("ACQUIRING SATELLITES\n");
			 acquisition(COMPLEX codedata[]);
		}
	}
}
        
        
