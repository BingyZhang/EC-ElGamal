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
#define  CFILE "EC_cipher.txt"
#define  PFILE "EC_plain.txt"

#define MAX 30000

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
    	int iy,i;
	char *line = new char[512];//buffer needs char instead of const char
    	ofstream fout;
    	ifstream fin;
	char *cipher_file;
    	time_t seed;
    	Big tempB,a,b,p,x,y,sk;
    	ECn g,h,c1,c2,tempE;
    	miracl *mip=&precision;

    	time(&seed);
    	irand((long)seed);   /* change parameter for different values */

	if(argc ==2){
                cipher_file=argv[1];
        }
	else{
		cipher_file=(char *)CFILE;
	}



	cout << "Decrypting EC-ElGamal...." << endl;
    	a=-3;
    	mip->IOBASE=16;
    	b=ecb;
    	p=ecp;
    	ecurve(a,b,p,MR_BEST);  // means use PROJECTIVE if possible, else AFFINE coordinates
    	x=ecx;
    	y=ecy;
    	g=ECn(x,y);
    	//Read sk
    	mip->IOBASE=64;
    	fin.open(SKFILE);
    	fin.getline(line,512);
        sk = line;
    	fin.close();
	
	//Read cipher and decrypt
        fin.open(cipher_file);
	fout.open(PFILE);
	while(fin.getline(line,512)){
	        x = line;
	        fin.getline(line,512);
        	iy = atoi(line);
       		c1 = ECn(x,iy); //decompress
        	fin.getline(line,512);
        	x = line;
        	fin.getline(line,512);
        	iy = atoi(line);
        	c2 = ECn(x,iy); //decompress
	        //decrypt
        	tempE = -sk*c1;
        	c2 +=tempE;
        	//brute force
        	tempE = -g;
        	for(i=0;i<=MAX;i++){
                	tempE +=g;
                	if(tempE == c2){
                        	break;
                	}
        	}
        	//output message
        	fout<<i<<endl;
	}
        fin.close();
        fout.close();

	//free buffer
	delete [] line;
	return 0;
}

