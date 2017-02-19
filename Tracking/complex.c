#include "complex.h"
#include "math.h"
//#include "settings.h"
//extern unsigned int samplesPerCode;
//extern Settings s1;
//extern int FFTLength;

double c_abs(COMPLEX *x)
{
	double z;
	z= sqrt(x->real * x->real + x->Imag * x->Imag);
	return z;
}

double c_power(COMPLEX *x)
{
	double z;
	z= x->real * x->real + x->Imag * x->Imag;
	return z;
}
double c_angle(COMPLEX *x)
{
	double z;
	z = atan(x->Imag/x->real);
	return z;
}
void c_add(COMPLEX *x, COMPLEX * y, COMPLEX * z)
{
	z->real = x->real + y->real;
	z->Imag = x->Imag + y->Imag;
}
void c_sub(COMPLEX *x, COMPLEX * y, COMPLEX * z)
{
	z->real = x->real - y->real;
	z->Imag = x->Imag - y->Imag;
}

/*void c_addsignals(COMPLEX *x, COMPLEX * y, COMPLEX * z)
{
	int i;	
	samplesPerCode = floor(s1.samplingFreq /(s1.codeFreqBasis /s1.codeLength));
	for(i=0;i<5*samplesPerCode;i++)
	{
		z[i].real = x[i].real - y[i].Imag;
		z[i].Imag = x[i].Imag + y[i].real;
	}
}

void c_mult(COMPLEX *x, COMPLEX * y, COMPLEX * z)
{
	z->real = x->real * y->real - x->Imag * y->Imag;
	z->Imag = x->real * y->Imag + x->Imag * y->real;
}


void conjugate(COMPLEX *x,COMPLEX *y)
{
	int i;
	FFTLength=pow(2,ceil(logf(5*samplesPerCode)/logf(2)));
	for(i=0;i<FFTLength;i++)
	{
		y[i].real=x[i].real;
		y[i].Imag=-(x[i].Imag);
	}
}

void c_multsignals(COMPLEX *x, COMPLEX * y, COMPLEX * z)
{
	int i;
	FFTLength=pow(2,ceil(logf(5*samplesPerCode)/logf(2)));
	for(i=0;i<FFTLength;i++)
	{
		z[i].real = x[i].real * y[i].real - x[i].Imag * y[i].Imag;
		z[i].Imag = x[i].real * y[i].Imag + x[i].Imag * y[i].real;
	}
}

void c_absolute(COMPLEX *x,double *y)
{
	int i;
	FFTLength=pow(2,ceil(logf(5*samplesPerCode)/logf(2)));
	for(i=0;i<FFTLength;i++)
	{
		y[i]= (x[i].real * x[i].real + x[i].Imag * x[i].Imag);
	}
}*/
