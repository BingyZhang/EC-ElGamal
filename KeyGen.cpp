/*
 *   p521
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

/* NIST p521 bit elliptic curve prime 2#521-1 */

char *ecp=(char *)"000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

/* elliptic curve parameter A */

char *eca=(char *)"000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC";


/* elliptic curve parameter B */

char *ecb=(char *)"00000051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00";

/* elliptic curve - point of prime order (x,y) */

char *ecx=(char *)"000000c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66";
char *ecy=(char *)"0000011839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650";

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
    a=-3; // a = p521 -3
    mip->IOBASE=16;
    b=ecb;
    p=ecp;
    ecurve(a,b,p,MR_BEST);  // means use PROJECTIVE if possible, else AFFINE coordinates
    x=ecx;
    y=ecy;
    g=ECn(x,y);
    //Generate and output pk and sk
    sk=rand(500,2);
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

