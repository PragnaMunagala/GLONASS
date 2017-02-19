typedef struct
{
	double real;
	double Imag;
} COMPLEX;
double c_abs(COMPLEX *x);
double c_power(COMPLEX *x);
double c_angle(COMPLEX *x);
void c_add(COMPLEX *x, COMPLEX * y, COMPLEX * z);
void c_sub(COMPLEX *x, COMPLEX * y, COMPLEX * z);
void c_addsignals(COMPLEX *x, COMPLEX * y, COMPLEX * z);
void c_mult(COMPLEX *x, COMPLEX * y, COMPLEX * z);
void c_multsignals(COMPLEX *x, COMPLEX * y, COMPLEX * z);
void conjugate(COMPLEX *x,COMPLEX *y);
void c_absolute(COMPLEX *x,double *y);