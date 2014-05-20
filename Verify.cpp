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
#define  CFILE "EC_cipher.txt"
#define  PFILE "EC_plain.txt"


/* NIST p521 bit elliptic curve prime 2#521-1 */

char *ecp=(char *)"000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

/* group order q */

char *ecq=(char *)"000001fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffa51868783bf2f966b7fcc0148f709a5d03bb5c9b8899c47aebb6fb71e91386409";

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
    int iy,i;
	char *line = new char[512];//buffer needs char instead of const char
    ofstream fout;
    ifstream fin;
    time_t seed;
    Big tempB,a,b,p,x,y,plain,rand;
    ECn g,h,c1,c2,tempE,a1,a2;
    miracl *mip=&precision;
    time(&seed);
    irand((long)seed);   /* change parameter for different values */





    a=-3;
    mip->IOBASE=16;    b=ecb;
    p=ecp;
    ecurve(a,b,p,MR_BEST);  //means use PROJECTIVE if possible, else AFFINE coordinates
    x=ecx;
    y=ecy;
    g=ECn(x,y);
    //Read PK
    mip->IOBASE=64;
    fin.open(PKFILE);
    fin.getline(line,512);
    x = line;
    fin.getline(line,512);
    iy = atoi(line);
    fin.close();
	h = ECn(x,iy); //decompress pk

	
	//Read cipher and plaintext, and verify
    fin.open(CFILE);
	fin.getline(line,512);
    x = line;
    fin.getline(line,512);
    iy = atoi(line);
    c1 = ECn(x,iy); //decompress
    fin.getline(line,512);
    x = line;
    fin.getline(line,512);
    iy = atoi(line);
    c2 = ECn(x,iy); //decompress
    fin.close();
    
    fin.open(PFILE);
	fin.getline(line,512);
    plain = line;
    fin.getline(line,512);
    rand = line;
    fin.close();
    
    //check
    a1 = rand*g;
    tempE = rand*h;
    a2 = plain*g;
    a2+=tempE;
    if(c1 == a1 && c2 == a2 ){
        cout<<"1"<<endl;
    }
    else{
        cout<<"0"<<endl;
    }
    

	//free buffer
	delete [] line;
	return 0;
}

