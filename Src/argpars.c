#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Parses an input structured in this way: ./main -a AAA -c CCC -b BBB ... -fe ( or -f -e ) where a,b,c wants a string while fg don't, g and h instead want an integer */


/***********************************    FUNCTIONS. TYPES AND MACROS THAT NEEDS TO BE DEFINED BASED ON THE APPLICATION    **************************************

 Modify this structure based on the type of parameters, their length ecc.. char[15] options that needs a parameter, while int the options with flags*/

#define PARAM1 'a'
#define PARAM2 'b'
#define PARAM3 'c'
#define FLAG1 'd'
#define FLAG2 'e'
#define FLAG3 'f'
#define VAL1 'g'
#define VAL2 'h'
#define MAXPARAM 6
#define MAXSTRLEN 20

typedef struct{
	char a[MAXSTRLEN];
	char b[MAXSTRLEN];
	char c[MAXSTRLEN];
	int  d;
	int  e;
	int  f;
	int  g;
	int  h;
}optStruct;

//Return 1 if parameter is a flag, 2 if it needs a string argument, 3 if a int argument, -1 if the option is unknown
int parseOption(char opt)
{
	if(opt == PARAM1 || opt == PARAM2 || opt == PARAM3)
	{
		return 2;
	}else if (opt == FLAG1 || opt == FLAG2 || opt == FLAG3)
	{
		return 1;
	}else if (opt == VAL1 || opt == VAL2)
	{
		return 3;
	}else
	{
		return -1;
	}
}

//Set the corresponding flag in the struct
void setFlag(char flag, optStruct* options)
{
	switch(flag)
	{
		case FLAG1:
			options->e = 1;
			break;
		case FLAG2:
			options->f = 1;
			break;
		case FLAG3:
			options->g = 1;
			break;
	}
}

void setStrPar(char flag, optStruct* options, char* strPar, int n_bytes)
{
	switch(flag)
	{
		case PARAM1:
			memcpy((void*)options->a,(void*)strPar,n_bytes);
			break;
		case PARAM2:
			memcpy((void*)options->b,(void*)strPar,n_bytes);
			break;
		case PARAM3:
			memcpy((void*)options->c,(void*)strPar,n_bytes);
			break;
	}
}

void setNumPar(char flag, optStruct* options, int val)
{
	switch(flag)
	{
		case VAL1:
			options->g = val;
			break;
		case VAL2:
			options->h = val;
			break;
	}
}

void initOpt(optStruct* options)
{
	memset(options->a,0,MAXSTRLEN);
	memset(options->b,0,MAXSTRLEN);
	memset(options->c,0,MAXSTRLEN);
	options->d=0;
	options->e=0;
	options->f=0;
	options->g=0;
	options->h=0;
}

void printOpt(optStruct* options)
{
	printf("%s \n   \
		    %s \n    \  
		    %s \n    \
		    %d \n    \
		    %d \n    \
		    %d \n    \
		    %d \n    \
		    %d \n    \
		    %d \n",
		    options->a,
		    options->b,
		    options->c,
		    options->d,
		    options->e,
		    options->f,
		    options->g,
		    options->h );
}

/* GENERAL FUNCTIONS */

int optIndexSearch(int argc, char** argv, int* optIndex)
{
	int p =0;
	for(int i = 0;i<argc;i++)
	{
		if(*argv[i] == '-')
		{
			optIndex[p] = i;
			p++;
		}
	}
	return p;
}

void fillOptStruct(char** argv, int* optIndex, int nOpt, optStruct* options)
{
	char currentOpt;
	char* parameter;
	int numPar,j,k;
	for(int i=0;i<nOpt;i++)
	{
		//If the string is -t, optIndex[i] gives the position of the i-th "-" while *(argv[optIndex[i]]+1) gives the paramter/flag
		switch( parseOption(*(argv[optIndex[i]]+1)) )
		{
			case 1: /* Flag */
				//Check if -f or -fd or -efd ecc..
				j=0;
				//This works because +1 excludes the "-" and the j cycles on the other flags, if more than one
				currentOpt = *(argv[optIndex[i]]+1+j);
				while( currentOpt != 0  )
				{
					setFlag(currentOpt, options);
					j++;
					currentOpt = *(argv[optIndex[i]]+1+j);
				}
				break;
			case 2: /* Parameter */
				//If at argv[i]+1 there is "a" at argv[i+1] there is the start of the string that contains the parameter 
				currentOpt = *(argv[optIndex[i]]+1);
				parameter = argv[optIndex[i]+1];
				k=0;
				//Count length of the parameter string
				while( *(parameter+k) != 0 )
				{
					k++;
					if(k > MAXSTRLEN)
					{
						fprintf(stderr,"Max length for string parameter (%d) not respected, returning\n",MAXSTRLEN);
						return;
					}
				}
				setStrPar(currentOpt, options, parameter, k);
				break;
			case 3: /*Numerical parameter*/
				//If at argv[i]+1 there is h, at argv[i+1] there is the string representing the number
				currentOpt = *(argv[optIndex[i]]+1);
				numPar = atoi(   (argv[optIndex[i]]+1)   );
				setNumPar(currentOpt, options, numPar);
				break;
			case -1: /* Error */
				fprintf(stderr,"Option %c not recognized\n",*(argv[optIndex[i]]+1));
				return;
				break;
		}
	}
}


int main(int argc, char** argv)
{
	int optIndex[MAXPARAM];
	int nOpt;
	optStruct options;
	initOpt(&options);

	nOpt = optIndexSearch(argc,argv,optIndex);
	fillOptStruct(argv, optIndex, nOpt, &options);

	printOpt(&options);
	return 0;

}
