// Expects raw sequences (only uppercase 'A', 'C', 'G' or 'T') on a
// line-by-line basis
// prints the model (counts) to stdout

/* expect input: K INPUT
K : tree depth
INPUT : input data
*/

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "ctfunctions.h"

static double modelprob[USIZE][4] = {{0.0}};


int main(int argc, char *argv[])
{
	#if defined(DEBUG)
		fwrite("---- DEBUG mode ---- \n", sizeof(char), strlen("---- DEBUG mode ---- \n"), stdout);
	#endif    
	char str[15];

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s K model\n", argv[0]);
        return EXIT_FAILURE;
    } // if

    K = atoi(argv[1]);
    SIZE = 1 << (2 * K); // 4 ^ K

    if (K > 10)
    {
        fprintf(stderr, "K = %ld is too large\n", K);
        return EXIT_FAILURE;
    } // if

	read_count_file(argv[2], count);
	normalize(count,modelprob);

#if defined(DEBUG)
    fwrite("Print counts\n", sizeof(char), strlen("Print counts\n"), stdout);
    fwrite("tree depth: ", sizeof(char), strlen("tree depth: "), stdout);
    fwrite(argv[1], sizeof(char), strlen(argv[1]), stdout);
    //fwrite(argv[2], sizeof(char), strlen(argv[2]), stdout);
    //fwrite(argv[3], sizeof(char), strlen(argv[3]), stdout);
    fwrite("\n", sizeof(char), 1, stdout);
#endif
	fwrite("counts\n", sizeof(char), strlen("counts\n"), stdout);
	print_count(count);
	fwrite("probs\n", sizeof(char), strlen("probs\n"), stdout);
	print_model(modelprob);
    
} // main

