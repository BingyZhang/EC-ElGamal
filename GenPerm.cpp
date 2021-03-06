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
//#include <random>

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



// It returns a random permutation of 0..n-1
/*
int * Rperm(int n) {
    int *a = new int[n];
    int k;
    std::random_device rd;
    std::mt19937 gen(rd());
    for (k = 0; k < n; k++)
	a[k] = k;
    for (k = n-1; k > 0; k--) {
	std::uniform_int_distribution<int> dis(0, k);
	int j = dis(gen);
	int temp = a[j];
	a[j] = a[k];
	a[k] = temp;
    }
    return a;
}
*/

//fake permutation for debug
int * ID(int n) {
    int *a = new int[n];
    int k;
    for (k = 0; k < n; k++)
        a[k] = k;
    return a;
}


int main(int argc, char *argv[])
{
    int iy,i,j, n=2, max = 1000;
    int *plist;
    char *line = new char[512];//buffer needs char instead of const char
    ofstream fout;
    ofstream pout;
    ifstream fin;
    time_t seed;
    Big tempB,a,b,p,x,y,r;
    ECn g,h,c1,c2,tempE;
    miracl *mip=&precision;

    time(&seed);
    irand((long)seed);   /* change parameter for different values */
    if(argc >=2){
        n=atoi(argv[1]);
    }
    
    if(argc ==3){
        max=atoi(argv[2]);
    }
	
	//cout << "Generating EC-ElGamal "<<n<<" X "<<n<< " Permutation Matrix...." << endl;
    a=-3;
    mip->IOBASE=16;
    b=ecb;
    p=ecp;
    ecurve(a,b,p,MR_BEST);  // means use PROJECTIVE if possible, else AFFINE coordinates
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
    //Generate random permutation
	plist = ID(n);
	//print plist
	for(i=0;i<n;i++){
		cout<<plist[i];
		if(i<n-1)cout<<",";
		else cout<<endl;
	}


	//encrypt
	fout.open(CFILE);
    pout.open(PFILE);
	for(i = 0;i<n;i++){
        tempB = max;
        tempB = pow(tempB,plist[i]);
        r=rand(500,2);
        //write the plain and random
        pout<<tempB<<endl<<r<<endl;
        c1 = r*g;
        c2 = tempB*g;
        tempE = r*h;
        c2+=tempE; //avoid temp variables
        iy = c1.get(x);
        fout<<x<<endl;
        fout<<iy<<endl;
        iy = c2.get(x);
        fout<<x<<endl;
        fout<<iy<<endl;
	}
	fout.close();
    pout.close();


    //free buffer
    delete [] line;
	delete [] plist;
	return 0;
}

