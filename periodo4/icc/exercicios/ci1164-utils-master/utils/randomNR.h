#ifndef _RANDOMNR_H_
#define _RANDOMNR_H_

// all the system #include's we'll ever need
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX(a, b)  (b > a ? b : a)
#define MIN(a, b)  (b < a ? b : a)
#define SIGN(a, b) (b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a))

// basic type names (redefine if your bit lengths don't match)

typedef int Int; // 32 bit integer
typedef unsigned int Uint;

typedef long long int Llong; // 64 bit integer
typedef unsigned long long int Ullong;

typedef char Char; // 8 bit integer
typedef unsigned char Uchar;

typedef double Doub; // default floating type
typedef long double Ldoub;

// typedef complex<double> Complex; // default complex type

typedef unsigned char Bool;

// NaN: uncomment one of the following 3 methods of defining a global NaN
// you can test by verifying that (NaN != NaN) is true

//Uint proto_nan[2]={0xffffffff, 0x7fffffff};
//double NaN = *( double* )proto_nan;

//Doub NaN = sqrt(-1.);

// Functions
// Período do gerador: 3.138e57 (aprox.)
// Fonte: Press, W.H.; "Numerical Recipes, 3rd ed.", pp.342)

// Define a semente
void nrSeed(Ullong j);
// Retorna int-64 bits entre 0 e 2⁶⁴ - 1
Ullong nrRandom64();
// Retorna int-64 bits entre 0 e 1.0
Doub nrDrandom();
// Retorna int-32 bits entre 0 e 2³² - 1
Uint nrRandom32();

#endif /* _RANDOMNR_H_ */
