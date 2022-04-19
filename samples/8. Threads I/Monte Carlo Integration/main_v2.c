		/*--------------------------------------------------------------+
		|	Multi-threaded integration using a Monte-Carlo technique	|
		|	and the Ranarray portable random number generator posted	|
		|	by Prof. Donald Knuth										|
		|	http://www-cs-faculty.stanford.edu/~uno/news02.html#rng		|
		|																|
		|	A progressive implementation seen step by step.				|
		|	Jean-Yves Hervé, 3DG URI, October 2016						|
		|																|
		|	Version 2:	First multi-threaded version.  This one is		|
		|				a bit primitive, but gets the job done.			|
		|																|
		|	Note:	I removed some of the long explanations from V1		|
		|			and I marked as //new all the changes compared to	|
		|			the previous version.  Sorry, but if you jump		|
		|			directly to the last version to "get the correct	|
		|			one," you are going to miss a lot of stuff.			|
		+--------------------------------------------------------------*/
		
//=============================================================================
//		Here I simply included the code of Prof. D. Knuth
//=============================================================================

/*    This program by D E Knuth is in the public domain and freely copyable
 *    AS LONG AS YOU MAKE ABSOLUTELY NO CHANGES!
 *    It is explained in Seminumerical Algorithms, 3rd edition, Section 3.6
 *    (or in the errata to the 2nd edition --- see
 *        http://www-cs-faculty.stanford.edu/~knuth/taocp.html
 *    in the changes to Volume 2 on pages 171 and following).              */

/*    N.B. The MODIFICATIONS introduced in the 9th printing (2002) are
      included here; there's no backwards compatibility with the original. */

/*    This version also adopts Brendan McKay's suggestion to
      accommodate naive users who forget to call ranf_start(seed).         */

/*    If you find any bugs, please report them immediately to
 *                 taocp@cs.stanford.edu
 *    (and you will be rewarded if the bug is genuine). Thanks!            */

/************ see the book for explanations and caveats! *******************/
/************ in particular, you need two's complement arithmetic **********/

#define KK 100                     /* the long lag */
#define LL  37                     /* the short lag */
#define mod_sum(x,y) (((x)+(y))-(int)((x)+(y)))   /* (x+y) mod 1.0 */

double ran_u[KK];           /* the generator state */

#ifdef __STDC__
void ranf_array(double aa[], int n)
#else
void ranf_array(aa,n)    /* put n new random fractions in aa */
  double *aa;   /* destination */
  int n;      /* array length (must be at least KK) */
#endif
{
  register int i,j;
  for (j=0;j<KK;j++) aa[j]=ran_u[j];
  for (;j<n;j++) aa[j]=mod_sum(aa[j-KK],aa[j-LL]);
  for (i=0;i<LL;i++,j++) ran_u[i]=mod_sum(aa[j-KK],aa[j-LL]);
  for (;i<KK;i++,j++) ran_u[i]=mod_sum(aa[j-KK],ran_u[i-LL]);
}

/* the following routines are adapted from exercise 3.6--15 */
/* after calling ranf_start, get new randoms by, e.g., "x=ranf_arr_next()" */

#define QUALITY 1009 /* recommended quality level for high-res use */
double ranf_arr_buf[QUALITY];
double ranf_arr_dummy=-1.0, ranf_arr_started=-1.0;
double *ranf_arr_ptr=&ranf_arr_dummy; /* the next random fraction, or -1 */

#define TT  70   /* guaranteed separation between streams */
#define is_odd(s) ((s)&1)

#ifdef __STDC__
void ranf_start(long seed)
#else
void ranf_start(seed)    /* do this before using ranf_array */
  long seed;            /* selector for different streams */
#endif
{
  register int t,s,j;
  double u[KK+KK-1];
  double ulp=(1.0/(1L<<30))/(1L<<22);               /* 2 to the -52 */
  double ss=2.0*ulp*((seed&0x3fffffff)+2);

  for (j=0;j<KK;j++) {
    u[j]=ss;                                /* bootstrap the buffer */
    ss+=ss; if (ss>=1.0) ss-=1.0-2*ulp;  /* cyclic shift of 51 bits */
  }
  u[1]+=ulp;                     /* make u[1] (and only u[1]) "odd" */
  for (s=seed&0x3fffffff,t=TT-1; t; ) {
    for (j=KK-1;j>0;j--)
      u[j+j]=u[j],u[j+j-1]=0.0;                         /* "square" */
    for (j=KK+KK-2;j>=KK;j--) {
      u[j-(KK-LL)]=mod_sum(u[j-(KK-LL)],u[j]);
      u[j-KK]=mod_sum(u[j-KK],u[j]);
    }
    if (is_odd(s)) {                             /* "multiply by z" */
      for (j=KK;j>0;j--) u[j]=u[j-1];
      u[0]=u[KK];                    /* shift the buffer cyclically */
      u[LL]=mod_sum(u[LL],u[KK]);
    }
    if (s) s>>=1; else t--;
  }
  for (j=0;j<LL;j++) ran_u[j+KK-LL]=u[j];
  for (;j<KK;j++) ran_u[j-LL]=u[j];
  for (j=0;j<10;j++) ranf_array(u,KK+KK-1);  /* warm things up */
  ranf_arr_ptr=&ranf_arr_started;
}

#define ranf_arr_next() (*ranf_arr_ptr>=0? *ranf_arr_ptr++: ranf_arr_cycle())
double ranf_arr_cycle()
{
  if (ranf_arr_ptr==&ranf_arr_dummy)
    ranf_start(314159L); /* the user forgot to initialize */
  ranf_array(ranf_arr_buf,QUALITY);
  ranf_arr_buf[KK]=-1;
  ranf_arr_ptr=ranf_arr_buf+1;
  return ranf_arr_buf[0];
}

//	Commented out original main function.
//	Hopefully that does not count as a change, otherwise the
//	"library" would be pretty useless.
//	jyh. 2016-10-14

//#include <stdio.h>
//int main()
//{
//  register int m; double a[2009];  a rudimentary test
//  ranf_start(310952);
//  for (m=0;m<2009;m++) ranf_array(a,1009);
//  printf("%.20f\n", ran_u[0]);             0.36410514377569680455
//      beware of buggy printf routines that do not give full accuracy here!
//  ranf_start(310952);
//  for (m=0;m<1009;m++) ranf_array(a,2009);
//  printf("%.20f\n", ran_u[0]);             0.36410514377569680455
//  return 0;
//}


//=============================================================================
//		Our code starts here
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

//-------------------------
// shared data
//-------------------------

//	Number of threads
#define NUM_THREADS			10

//	Number of random points used by each thread
#define POINTS_PER_THREAD	10000

//	Total number of random points that we will use.
#define NUM_POINTS			(NUM_THREADS * POINTS_PER_THREAD)

//	This array will store the array of pseudo-random x and y coordinates,
//	so 2*NUM_POINTS double values
double* xy;

//new
//	Now we must use a separate counter for each thread.  The global
//	counter can be a local variable to the main function
////	This counter counts how many random points fall under the graph of
////	the function to integrate.  The integral of the function will be
////	equal to count / NUM_POINTS
//unsigned int counter = 0;

//	The function that I want to compute the integral of, over the
//	interval [0, 1].  Note that for the algorithm to work (in its
//	current implementation, the function *must* take its values in
//	the interval [0, 1].  So, for example, if the function was
//	f(x) = 2 x^2 instead, the values in the range (1, 2]
//	(for 1/√2 < x <= 1) would get "clipped."
double f(double x)
{
	double x2 = x*x;
	
	return 1 - 0.5*x -0.75*x2 + 0.25*x*x2;
}

//new
//	All is new, or modified in this function.  Look at it all.
//
//	This is the thread main function.  This is where we moved the
//	code doing the counting of points below the graph
void* threadFunc(void* argument)
{
	//	In this version, the parameter received is simply
	//	a pointer to the thread index.  Here we get that index.
	int tIndex = *((int *) argument);

	//	This thread is going to access only part of the array
	//	of xy coordinates.  We define the base address for this thread
	double* myXY = xy + 2*POINTS_PER_THREAD*tIndex;
	
	//	This will count the number of points below the graph
	int counter = 0;

	//	This is the integration code proper.  This code will move into my
	//	thread function in Version 2
	for (int i=0; i<POINTS_PER_THREAD; i++)
	{
		//	"Generate" a random point (x, y) in [0, 1]x[0, 1]
		//	Here, get the coordinate from the pre-generated array.  The x coordinates
		//	are at even indices and the y coordinates at odd indices
		double x = myXY[2*i];
		double y = myXY[2*i + 1];
		
		//	If the point is below the graph of f, increment the counter
		if (y <= f(x))
			counter++;

	}
	
	//	So, now, how do I return the counter value?  I should have thought about it before :-)
	//	Now, I have no choice but to allocate a 1-sized "array" of int to store that value,
	//	and return the pointer
	int* returnVal = malloc(sizeof(int));
	*returnVal = counter;
	
	return returnVal;
}

int main(int argc, char* argv[])
{
	printf("Monte Carlo integration -- Version 2\n\n");
	

	//	Allocate array of pseudo-random coordinates
	xy = (double*) malloc(2*NUM_POINTS*sizeof(double));

	//	Version for repeatable pseudo-random dataset
	//ranf_start(310952);
	//	pseudo-random seeding
	clock_t seed = clock();
	ranf_start(seed);
	
	//	Generate the random values.  See comments on Prof Knuth's page for
	//	why this is done this way
	ranf_array(xy, 2021);
	ranf_array(xy, 2*NUM_POINTS);
	
	//new
	//	We can use automatic allocation for the array that will store the
	//	threadID info. Again, the pthread_t data type is implementation dependent.
	//	It could a simple unsigned int (or long) or a pointer to a struct.
	pthread_t threadID[NUM_THREADS];

	//new
	//	I create my threads using default attributes, so all the threads will
	//	be joinable.
	for (int k=0; k<NUM_THREADS; k++)
	{
		//	If you pass NULL as second parameter, then this set
		//	defaults parameters for the thread, and that includes
		//	a setting for the thread to be "joinable", i.e. not "detached"
		//	In this version I only pass to the thread its own index
		//	in the program (again, this is *not* the thread id)
		int createErr = pthread_create(threadID + k, NULL, threadFunc, &k);
		
		//	Note that the above code is somewhat suspect, based on what we saw
		//	in class yesterday:  We are sending a pointer to our loop index
		//	to the thread creation function, but you don't know for sure when
		//	the thread function will dereference the pointer.  This may well happen
		//	*after* the loop index k has been incremented.  Making a call to usleep
		//	to make this occurrence less likely is only a hack, not a real fix
		//	(remember: don't make asumptions about speed or number).  We will fix
		//	the problem in V. 3
		usleep(2000);
		
		if (createErr != 0)
		{
			printf ("could not pthread_create thread %d. %d/%s\n",
					 k, createErr, strerror(createErr));
			exit (EXIT_FAILURE);
		}
	}
	
	//new
	//	All my threads are running and now all I have to do is wait for
	//	them to finish and join.
	
	//	the counter where I am going to collate the "votes" of all
	//	the counting threads
	long globalCounter = 0;
	
	for (int k = 0; k<NUM_THREADS; k++)
	{
		//	See Thread 1 code smaple for explanation.
		int* w;
		pthread_join(threadID[k], (void**) &w);
		
		printf( "Thread %d joined. It reported a count of %d out of %d, for a partial estimate of %f\n",
				k, w[0], POINTS_PER_THREAD, ((1.0 * w[0])/POINTS_PER_THREAD));
		
		globalCounter += w[0];

		printf( "Global count is now %ld out of %d, for an estimate of %f\n",
				globalCounter, ((k+1)*POINTS_PER_THREAD), (globalCounter/((k+1.0)*POINTS_PER_THREAD)));
		
		free(w);
	}
	
	
	//	End of the computations, report the result.
	printf("\nIntegral = %f\n\n", (1.0 * globalCounter /NUM_POINTS));
	
	//	Here I free the arrays that were allocated dynamically in the function.
	free(xy);
	
	return 0;
}
