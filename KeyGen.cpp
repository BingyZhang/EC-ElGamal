/*
 *   Example program demonstrates 1024 bit Diffie-Hellman, El Gamal and RSA
 *   and 168 bit Elliptic Curve Diffie-Hellman 
 *
 *   Requires: big.cpp ecn.cpp
 */

#include <fstream>
#include <iostream>
#include "ecn.h"
#include "big.h"
#include <ctime>

//using namespace std;

#define  PKFILE "EC_PK.txt"
#define  SKFILE "EC_SK.txt"

/* NIST p192 bit elliptic curve prime 2#192-2#64-1 */

char *ecp=(char *)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF";

/* elliptic curve parameter B */

char *ecb=(char *)"64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1";

/* elliptic curve - point of prime order (x,y) */

char *ecx=(char *)"188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012";
char *ecy=(char *)"07192B95FFC8DA78631011ED6B24CDD573F977A11E794811";

#ifndef MR_NOFULLWIDTH
Miracl precision(50,0);
#else 
Miracl precision(50,MAXBASE);
#endif

// If MR_STATIC is defined in mirdef.h, it is assumed to be 100

//Miracl precision(120,(1<<26));

int main(int argc, char *argv[])
{
    int iy;
    ofstream fout;
    time_t seed;
    Big sk,a,b,p,x,y;
    ECn g,h;
    miracl *mip=&precision;

    time(&seed);
    irand((long)seed);   /* change parameter for different values */

    cout << "Generating EC-ElGamal Public and Private Key Pairs...." << endl;
    a=-3;
    mip->IOBASE=16;
    b=ecb;
    p=ecp;
    ecurve(a,b,p,MR_BEST);  // means use PROJECTIVE if possible, else AFFINE coordinates
    x=ecx;
    y=ecy;
    g=ECn(x,y);
    //Generate and output pk and sk
    sk=rand(160,2);
    mip->IOBASE=64;
    fout.open(SKFILE);
    fout<<sk;
    fout.close();
    h = sk*g;
    iy=h.get(x); //<x,y> is compressed form of public key
    fout.open(PKFILE);
    fout<<x<<endl;
    fout<<iy;
    fout.close();





    return 0;
}

