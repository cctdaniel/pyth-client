#include <stdio.h>
#include <stdlib.h>

void
sort_gen( int n ) {

# if 0

  /* In register variant (PSEUDO OPS ~ 9+4n+3n(n-1))
     Assumes switch ~ 3 PSEUDO OPS (LDA,LD,JMP) -> 3 switch statements / 9 pseudo ops
     Assumes load   ~ 2 PSEUDO OPS (LDA,LD)     -> n loads             / 2n pseudo ops
     Assumes store  ~ "
     Assumes cswap  ~ 6 PSEUDO OPS (CMP,MOV,TESTEQ,CMOV,TESTNEQ,CMOV)  / 6*0.5*n*(n-1) pseudo ops */

//printf( "static inline key_t *              /* returns (sorted) x */\n" );
//printf( "sort_network_stable( key_t * x,    /* indexed [0,n) */\n" );
//printf( "                     ulong   n ) { /* assumes n in [0,%i) */\n", n );
  printf( "  int c;\n" );
  printf( "  key_t t" );
  for( int i=0; i<n; i++ ) printf( ", x%i", i );
  printf( ";\n" );
  printf( "\n" );
  printf( "  switch( n ) {\n" );
  for( int i=n; i; i-- ) printf( "  case %2iUL: x%i = x[%i]; /* fall through */\n", i, i-1, i-1 );
  printf( "  default: break;\n" );
  printf( "  }\n" );
  printf( "\n" );
  printf( "# define _(i,j) c = BEFORE( x##j, x##i ); t = x##i; x##i = c ? x##j : x##i; x##j = c ? t : x##j\n" );
  printf( "  switch( n ) {\n" );
  for( int i=n-1; i; i-- ) {
    printf( "  case %2iUL:", i+1 );
    for( int j=0; j<i; j++ ) printf( " _(%2i,%2i);", j,j+1 );
    printf( " /* fall through */\n" );
  }
  printf( "  default: break;\n" );
  printf( "  }\n" );
  printf( "# undef _\n" );
  printf( "\n" );
  printf( "  switch( n ) {\n" );
  for( int i=n; i; i-- ) printf( "  case %2iUL: x[%i] = x%i; /* fall through */\n", i, i-1, i-1 );
  printf( "  default: break;\n" );
  printf( "  }\n" );

//printf( "\n" );
//printf( "  return x;\n" );
//printf( "}\n" );

# else

  /* Memory variant (PSEUDO OPS ~ 3+4.5n(n-1))
     Assumes switch ~ 3 PSEUDO OPS (LDA,LD,JMP) -> 3 pseudo ops
     Assumes cswap  ~ 9 PSEUDO OPS (LDA,LDA,LD,LD,CMP,CMOV,CMOV,ST,ST) / 9*0.5*n*(n-1) pseudo ops */

//printf( "static inline key_t *              /* returns (sorted) x */\n" );
//printf( "sort_network_stable( key_t * x,    /* indexed [0,n) */\n" );
//printf( "                     ulong   n ) { /* assumes n in [0,%i) */\n", n );

  printf( "  do { /* BEGIN AUTOGENERATED CODE (n=%2i) *****************************/\n", n );
  printf( "    /* This network of comparators and fallthroughs implement a sorting network representation\n" );
  printf( "       of an insertion sort. Each case acts as a sort pass with the fallthrough falling through\n" );
  printf( "       to smaller ranges of the input. */\n");
  printf( "#   define SORT_STABLE_CE(i,j) u = x[(SORT_IDX_T)i]; v = x[(SORT_IDX_T)j]; c = SORT_BEFORE( v, u ); x[(SORT_IDX_T)i] = c ? v : u; x[(SORT_IDX_T)j] = c ? u : v\n" );
  printf( "    int        c;\n" );
  printf( "    SORT_KEY_T u;\n" );
  printf( "    SORT_KEY_T v;\n" );
  printf( "    switch( n ) {\n" );
  for( int i=n-1; i>=0; i-- ) {
    printf( "    case (SORT_IDX_T)%2i:", i+1 );
    for( int j=0; j<i; j++ ) printf( " SORT_STABLE_CE(%2i,%2i);", j,j+1 );
    printf( " /* fall through */\n" );
  }
  printf( "    case (SORT_IDX_T) 0: return x;\n" );
  printf( "    default: break;\n" );
  printf( "    }\n" );
  printf( "#   undef SORT_STABLE_CE\n" );
  printf( "  } while(0); /* END AUTOGENERATED CODE *******************************/\n" );

//printf( "\n" );
//printf( "  return x;\n" );
//printf( "}\n" );

# endif

}

int
main( int     argc,
      char ** argv ) {
  if( argc!=2 ) { fprintf( stderr, "Usage: %s [max_base_case]\n", argv[0] ); return 1; }
  int n = atoi( argv[1] );
  if( n<1 ) { fprintf( stderr, "n (%i) must be positive\n", n ); return 1; }
  sort_gen( n );
  return 0;
}

