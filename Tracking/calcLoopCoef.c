#include<stdlib.h>
#include<math.h>

float* calcLoopCoef(char LBW,float Zeta,float k)
{
	float Wn;
	float * tau = (float *)calloc(2,sizeof(float));
	Wn = LBW*8*Zeta / (4* pow(Zeta,2) + 1);
    *tau = k / (Wn * Wn);
    *++tau = 2.0 * Zeta / Wn;
	return(--tau);
}
