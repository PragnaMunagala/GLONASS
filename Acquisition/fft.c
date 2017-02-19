#include "stdlib.h"
#include "math.h"
#include "fft.h"
#include <stdio.h>
#define PI 3.141592653589793
void bitrev(COMPLEX * p2X, COMPLEX *p2Y, int N)
{
	unsigned int n,p,q,l,Nbits;
	Nbits=ceil(logf(N)/logf(2.0));
	for (l=0;l<N; l++)
	{
		q=l;
		p=0;

		for (n=0;n<Nbits;n++)
		{
			p=(p<<1)+ (q & 1);
			q=q>>1;
		}
		p2Y[p].real = p2X[l].real;
		p2Y[p].Imag = p2X[l].Imag;
	}
}
void fft_ifft(COMPLEX *p2x, COMPLEX *p2Y, int N,int select)
{
	int NStages,stage,NpointFFT,Nffts,Nthfft,Nbutterflys,nthButterfly,InputsIndex[2];
	COMPLEX  *W,WW,temp,xinput[2],xoutput[2];
	int i,j;
//	FILE *fp;
	NStages=ceil(logf(N)/logf(2.0));
	bitrev(p2x, p2Y, N);
	W=(COMPLEX*) malloc(N/2*sizeof(COMPLEX));
	for(i=0;i<N/2;i++)
	{
		W[i].real=cosf(-2*select*PI*i/N);
		W[i].Imag=sinf(-2*select*PI*i/N);
	}
	
	for(stage=1;stage<=NStages;stage++)
	{
		NpointFFT=powf((double)2,(double)stage);
		Nffts=N/NpointFFT;
		for(Nthfft=0;Nthfft<Nffts;Nthfft++)
			{
				Nbutterflys=NpointFFT/2;
				for(nthButterfly=0;nthButterfly<Nbutterflys;nthButterfly++)
				{	
					j=powf((double)2, (double)(NStages-stage))*nthButterfly;
					WW.Imag=W[j].Imag;
					WW.real=W[j].real;

					InputsIndex[0]=Nthfft*NpointFFT+nthButterfly+0;
					InputsIndex[1]=Nthfft*NpointFFT+nthButterfly+NpointFFT/2;
					xinput[0].real = p2Y[InputsIndex[0]].real;
					xinput[0].Imag = p2Y[InputsIndex[0]].Imag;

					xinput[1].real = p2Y[InputsIndex[1]].real;
					xinput[1].Imag = p2Y[InputsIndex[1]].Imag;
					c_mult(&p2Y[InputsIndex[1]],&WW,&xinput[1]);

					c_add(&xinput[0],&xinput[1],&xoutput[0]);
					c_sub(&xinput[0],&xinput[1],&xoutput[1]);

					
					
					for(i=0;i<2;i++)
					{
						p2Y[InputsIndex[i]].real = xoutput[i].real;
						p2Y[InputsIndex[i]].Imag = xoutput[i].Imag;
					}
					
				}
		}
	}
	if(select==-1)
		for(i=0;i<N;i++)
		{
			p2Y[i].Imag/=N;
			p2Y[i].real/=N;
		}
	free(W);

}