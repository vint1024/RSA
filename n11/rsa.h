#ifndef __RSASTART__
#define __RSASTART__

#include <stdio.h>
#include "gmp.h"
#include <fstream>
#include <time.h>
using namespace std;

bool Prost(mpz_t &m)
{
	
	mpz_t ONE;      
	mpz_init_set_si(ONE, 1);
	
	mpz_t TWO;      
	mpz_init_set_si(TWO, 2);
	
	mpz_t ZERO;      
	mpz_init_set_si(ZERO, 0);

	mpz_t r;      
	mpz_init_set_si(r, 0);
	mpz_t t;      
	mpz_init_set_si(t, 0);
	mpz_t s;      
	mpz_init_set_si(s, 0);
	mpz_t x;     
	mpz_init_set(x,m);
	mpz_sub(x,x,ONE);
	//x = m;
	mpz_t mod;    
	mpz_init(t);
	mpz_init(mod);
	mpz_mod(mod,x,TWO);
	
	while (mpz_cmp(ZERO,mod)==0)
	{
		mpz_div(x,x,TWO);
		mpz_add(s,s,ONE);
		mpz_mod(mod,x,TWO);
	}
	mpz_set(t,x);
	mpz_set(r,s);
	while (mpz_cmp(x,ZERO)>0)
	{
		mpz_div(x,x,TWO);
		mpz_add(r,r,ONE);
	}
	mpz_sub(x,m,ONE);
	mpz_t a;
	mpz_init (a);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, rand());
	mpz_t xM2;
	mpz_init(xM2);
	mpz_sub(xM2,x,TWO);
	while (mpz_cmp(r,ZERO)>0)
	{
		mpz_urandomm(a,state,xM2);
		mpz_add(a,a,TWO);
		mpz_t y;
		mpz_init(y);
		mpz_powm(y,a,t,m);
		if ((mpz_cmp(y,ONE)!=0)&&(mpz_cmp(y,x)!=0))
		{
			mpz_t ste;
			mpz_init_set(ste,s);
			mpz_sub(ste,ste,ONE);
			while(mpz_cmp(ste,ZERO)>0)
			{
				mpz_powm(y,y,TWO,m);
				if (mpz_cmp(y,ONE)==0) return false;
				if (mpz_cmp(y,x)==0) break;
				mpz_sub(ste,ste,ONE);
			}
			if(mpz_cmp(ste,ZERO)==0) return false;
		}
		mpz_sub(r,r,ONE);
	}
	return true;
};

void rsa(char *inFname, char *outFname)
{
	unsigned long long timetot=time(NULL);
	mpz_t ONE;
	mpz_init_set_ui(ONE,1);
	mpz_t p, q, constrand;
	mpz_init_set_ui(constrand,10);
	
	printf("creation constrand  - ");
	
	for (int i = 0; i<501; ++i)
		mpz_mul_ui(constrand,constrand,10);
	mpz_sub_ui(constrand,constrand,1);
	
	printf("OK\n");
	
	mpz_init (p);
	mpz_init (q);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	//srand(time(NULL));
	gmp_randseed_ui(state, rand());
	mpz_urandomm(p,state,constrand);
	
	printf("find p  - ");
	
	mpz_nextprime(p,p);
	
	printf("OK\n");
	//srand(time(NULL));
	gmp_randseed_ui(state, rand());
	mpz_urandomm(q,state,constrand);
	
	printf("find q  - ");
	
	mpz_nextprime(q,q);
	
	printf("OK\n");
	
	mpz_t n;
	mpz_init (n);
	mpz_mul(n,p,q);
	mpz_t d;
	mpz_init(d);
	mpz_sub(q,q,ONE);
	mpz_sub(p,p,ONE);
	mpz_t tmp;
	mpz_init(tmp);
	mpz_mul(tmp,p,q);
	mpz_add(q,q,ONE);
	mpz_add(p,p,ONE);
	mpz_t ev;
	mpz_init(ev);
	
	printf("find d  - ");
	//srand(time(NULL));
	gmp_randseed_ui(state, rand());
	mpz_urandomm(d,state,constrand);
	mpz_gcd(ev,d,tmp);
	while(mpz_cmp(ev,ONE)!=0)
	{
		mpz_add(d,d,ONE);
		mpz_gcd(ev,d,tmp);
	}	
	
	printf("OK\n");
	printf("find e  - ");
	
	mpz_t e,g;
	mpz_init(e);
	mpz_init_set_ui(g,1);
	mpz_gcdext(g,e,NULL,d,tmp);
	printf("OK\n");
	if (mpz_probab_prime_p(q,10)>0) printf("q is prime TRUE\n");
	else printf("q is prime FALSE\n");
	//mpz_out_str(stdout,10,q);
	if (mpz_probab_prime_p(p,10)>0) printf("p is prime TRUE\n");
	else printf("p is prime FALSE\n");
	//mpz_out_str(stdout,10,p);
	mpz_gcd(g,d,tmp);
	if (mpz_cmp(g,ONE)==0) printf("gcd( d , (p-1) * (q-1) ) == 1 TRUE\n");
	else printf("gcd(d,(p-1)*(q-1))!=1 FALSE\n");
	//mpz_out_str(stdout,10,d);
	mpz_mul(g,e,d);
	mpz_mod(g,g,tmp);
	if (mpz_cmp(g,ONE)==0) printf("(e * d) mod ( (q-1) * (p-q) ) == 1 TRUE\n");
	else printf("(e * d) mod ( (q-1) * (p-q) ) != 1 FALSE\n");
	//mpz_out_str(stdout,10,e);
	//cout << endl;
	//mpz_out_str(stdout,10,n);
	cout << "\nencoding is started";
	unsigned long long timecod = time(NULL);

	ifstream in(inFname, std::ios_base::binary);

	FILE *outfile, *keyF, *keyFo;
	
	char keyFname[1024] = "secret_key_";
	strcat(keyFname,outFname);
	
	outfile = fopen(outFname,"wb");
	keyF = fopen(keyFname,"wb");
	
	cout<<endl<< endl << keyFname << endl;

	mpz_out_raw(keyF,d);
	mpz_out_raw(keyF,n);

	keyFname[0] = '\0';
	strcat(keyFname,"open_key_");
	strcat(keyFname,outFname);
	keyFo = fopen(keyFname,"wb");
	cout << keyFname << endl;
	cout << outFname<<endl;
	mpz_out_str(keyFo,10,e);
	fwrite("\r\n",sizeof(char),2,keyFo);
	mpz_out_str(keyFo,10,n);
	//fwrite(" ",sizeof(char),1,keyFo);
	//mpz_out_str(keyFo,10,d);
	
	unsigned char tmp2;
	//unsigned long prov = 0;
	while(!in.eof())
	{
		tmp2=in.get();
		//cout << tmp2 << " ";
		mpz_set_ui(tmp,tmp2);
		mpz_powm(tmp,tmp,e,n);
		mpz_out_str(outfile,10,tmp);
		//tmp2 = mpz_get_ui(tmp);
		//prov = mpz_get_ui(tmp);
		//cout << prov << " " << int(tmp2)<<endl;
		fwrite(" ",sizeof(char),1,outfile);
	}
	mpz_out_str(outfile,10,n);
	cout<<"\nencoding is complete\n"<<endl;
	cout << "Time encoding "<<timecod-time(NULL)<<" sec."<<"\nTotal time "<< timetot-time(NULL)<<" sec.\n";
};
void rsa(char *inFname, char *outFname, char *keyFname)
{
	unsigned long long timeencod = time(NULL);
	cout << "decoding is started" << endl;
	mpz_t tmp, d, n;
	mpz_init(tmp);
	mpz_init(d);
	mpz_init(n);

	ofstream out(outFname, std::ios_base::binary);
	FILE *infile, *keyF;
	infile = fopen(inFname,"rb");
	keyF = fopen(keyFname,"rb");
	
	mpz_inp_raw(d,keyF);
	mpz_inp_raw(n,keyF);
	cout << "d: " << endl;
	mpz_out_str(stdout,10,d);
	
	cout << endl;
	cout << "n: " << endl;
	mpz_out_str(stdout,10,n);
	cout<<endl;
	unsigned char tmp2;
	unsigned long prov = 0;
	mpz_inp_str(tmp,infile,10);
	while(mpz_cmp(tmp,n)!=0)
	{
		mpz_powm(tmp,tmp,d,n);
		tmp2 = mpz_get_ui(tmp);
		//prov = mpz_get_ui(tmp);
		cout.width(3);
		cout <<int(tmp2)<<" - "<< tmp2<<endl;
		if (tmp2 == 255) {break;}
		out.put(tmp2);
		mpz_inp_str(tmp,infile,10);
	}
	cout << endl<< "decoding is completed" << endl;
	cout << "Time decoding "<<timeencod-time(NULL)<<" sec.\n";
}

#endif