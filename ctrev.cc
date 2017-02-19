// Expects raw sequences (only uppercase 'A', 'C', 'G' or 'T') on a
// line-by-line basis
// read binary model file, add reverse complement, and print binary to stdout

/* expect input: K INPUT
K : tree depth
INPUT : input data
*/

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "ctfunctions.h"


int main(int argc, char *argv[])
{
	#if defined(DEBUG)
		fwrite("---- DEBUG mode ---- \n", sizeof(char), strlen("---- DEBUG mode ---- \n"), stdout);
	#endif    

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s K model1 \n", argv[0]);
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

    complement(count);

#if defined(DEBUG)
    fwrite("reverse complement\n", sizeof(char), strlen("reverse complement\n"), stdout);
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

