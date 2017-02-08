// Expects raw sequences (only uppercase 'A', 'C', 'G' or 'T') on a
// line-by-line basis
// combines 2 models, add reverse complement, and print binary to stdout

/* expect input: K INPUT
K : tree depth
INPUT : input data
*/

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "ctfunctions.h"

// Count table for loading 2nd
static size_t count2[USIZE][4] = {{0}};

int main(int argc, char *argv[])
{
	#if defined(DEBUG)
		fwrite("---- DEBUG mode ---- \n", sizeof(char), strlen("---- DEBUG mode ---- \n"), stdout);
	#endif    

    if (argc < 4)
    {
        fprintf(stderr, "usage: %s K model1 model2\n", argv[0]);
        return EXIT_FAILURE;
    } // if

    K = atoi(argv[1]);
    SIZE = 1 << (2 * K); // 4 ^ K

    if (K > KMAX)
    {
        fprintf(stderr, "K = %ld is too large\n", K);
        return EXIT_FAILURE;
    } // if

	read_count_file(argv[2], count);
	read_count_file(argv[3], count2);
	// and combine the two models
	for (size_t i = 0; i < SIZE; ++i)
    {
        count[i][0] += count2[i][0] ;
        count[i][1] += count2[i][1] ;
        count[i][2] += count2[i][2] ;
        count[i][3] += count2[i][3] ;
    } // for	    

    complement(count);

#if defined(DEBUG)
    fwrite("Combine models & reverse complement\n", sizeof(char), strlen("Combine models & reverse complement\n"), stdout);
    fwrite("tree depth: ", sizeof(char), strlen("tree depth: "), stdout);
    fwrite(argv[1], sizeof(char), strlen(argv[1]), stdout);
    //fwrite(argv[2], sizeof(char), strlen(argv[2]), stdout);
    //fwrite(argv[3], sizeof(char), strlen(argv[3]), stdout);
    fwrite("\n", sizeof(char), 1, stdout);
    print_count();
#else
	write_model();
#endif
    
} // main

