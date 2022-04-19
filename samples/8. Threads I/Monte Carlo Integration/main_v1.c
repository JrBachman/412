		/*--------------------------------------------------------------+
		|	Multi-threaded integration using a Monte-Carlo technique	|
		|	and the Ranarray portable random number generator posted	|
		|	by Prof. Donald Knuth										|
		|	http://www-cs-faculty.stanford.edu/~uno/news02.html#rng		|
		|																|
		|	A progressive implementation seen step by step.				|
		|	Jean-Yves Hervé, 3DG URI, October 2016						|
		|																|
		|	Version 1:	We start with a single-threaded implementation	|
		|				just to make sure that we understand the		|
		|				basic algorithm.								|
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

//-------------------------
// shared data
//-------------------------

//	Number of random points that we will use.  As explained in class, I used
//	a #define rather than a regular variable because, at first blush,
//	prof. Knuth's code looked very "old style" to me and I assumed that in order
//	to build it I would have to revert to a C89 or even ANSI-C version of the
//	language in my compiler settings.  As we saw in class, that was not the case,
//	but keep that in mind when you use in your program some legacy code or
//	third-party library:  You may not be able to use the latest features of the
//	language.
#define NUM_POINTS  100000

//	This array will store the array of pseudo-random x and y coordinates,
//	so 2*NUM_POINTS double values
double* xy;

//	This counter counts how many random points fall under the graph of
//	the function to integrate.  The integral of the function will be
//	equal to count / NUM_POINTS
unsigned int counter = 0;

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


int main(int argc, char* argv[])
{
	printf("Monte Carlo integration -- Version 1\n\n");
	
	//	As we saw in class, if the array is large, it cannot be allocated
	//	on the stack, using autmatic allocation.  So we have to use
	//	dynamic allocation, using malloc or calloc, to store the array
	//	on the heap.  Here I use malloc because the array is going to be
	//	filled with random values anyway, so the (small) time required to
	//	clear before allocating, in calloc, is not justified.
	xy = (double*) malloc(2*NUM_POINTS*sizeof(double));

	//	Here, we could initialize the random generator by feeding it a fixed seed value.
	//	This is something that you may want to do if you are testing/debugging
	//	a simulation program and want to gauge over and over the effects of
	//	small adjustments on the same pseudo-random dataset
	//ranf_start(310952);
	
	//	A common way to seed the generator is to use CPU clock data.
	//	That one is typically random itself, so that you will get a different behavior
	//	every time you run the program.  Note that the value returned by clock()
	//	is the number of clock ticks since the program was launched, and the duration
	//	of a clock tick, which you can compute from CLOCKS_PER_SEC, is platform-specific.
	clock_t seed = clock();
	ranf_start(seed);
	
	//	Generate the random values.  After re-reading the comments on the code on Prof. Knuth's
	//	web page, it becomes clear why he runs the number gneration twice in his main function:
	//	A bug identifed by a user of an earlier version of the library can be solved if we...
	//	<Quote Knuth>
	//		(1) Discard the first 2000 numbers produced by ran_array after it has been started.
	//		(2) Improve the algorithm used by ran_start to prime the pump. With either (1) or (2),
	//		no statistical bias is detected in the resulting numbers by Marsaglia's famous
	//		"Die Hard" battery of tests.
	//		Brent and Gimeno recommended using BOTH fixes (1) and (2), to be doubly confident
	//		that residual nonrandomness isn't lurking.
	//	</Quote Knuth>
	//
	//	So, run the generator on 2000+ points first
	ranf_array(xy, 2021);

	//	and now generate the values we are going to use
	ranf_array(xy, 2*NUM_POINTS);
	
	//	This is the integration code proper.  This code will move into my
	//	thread function in Version 2
	for (int i=0; i<NUM_POINTS; i++)
	{
		//	"Generate" a random point (x, y) in [0, 1]x[0, 1]
		//	Here, get the coordinate from the pre-generated array.  The x coordinates
		//	are at even indices and the y coordinates at odd indices
		double x = xy[2*i];
		double y = xy[2*i + 1];
		
		//	If the point is below the graph of f, increment the counter
		if (y <= f(x))
			counter++;
	}
	
	//	End of the computations, report the result.
	printf("Integral = %f\n\n", (1.0 * counter / NUM_POINTS));
	
	//	Here I free the arrays that were allocated dynamically in the function.
	//	I don't really *have* to do it, since the entire partition will get wiped
	//	out when the process terminates, but
	//		1. It's good practice, while you're still learning C to force
	//			yourself to always have the concern: "I allocated memory in this
	//			function; will I still need it after I leave the function."
	//		2. If you use a memory leak tracker to optimize your code, it will
	//			probably report this as a memory leak.
	free(xy);
	
	return 0;
}
