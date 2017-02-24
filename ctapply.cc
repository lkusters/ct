// Expects raw sequences (only uppercase 'A', 'C', 'G' or 'T') on a
// line-by-line basis
// apply model to input data

/* expect input: K M INPUT
K : tree depth
M : model
INPUT : input data
*/

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "ctfunctions.h"

static size_t DIV = 0;

static size_t model[USIZE][4] = {{0}};

int main(int argc, char *argv[])
{
	#if defined(DEBUG)
		fwrite("---- DEBUG mode ---- \n", sizeof(char), strlen("---- DEBUG mode ---- \n"), stdout);
	#endif    
    if (argc < 4)
    {
        fprintf(stderr, "usage: %s K DIV Model data\nK: model depth\n\
DIV:\n\
0: Divergence D(seq||model) -> 1/N SUM n(c) SUM P(a|c) log( P(a|c) / M(a|c) )\n\
1: Divergence estimate D(seq||model) -> 1/N SUM n(a|c) log( P(a|c) / M(a|c) )\n\
2: Divergence D(model||seq)\n\
3: Divergence estimate D(model||seq)\n", argv[0]);
        return EXIT_FAILURE;
    } // if

    K = atoi(argv[1]);
    SIZE = 1 << (2 * K); // 4 ^ K

    if (K > KMAX)
    {
        fprintf(stderr, "K = %ld is too large\n", K);
        return EXIT_FAILURE;
    } // if
	DIV = atoi(argv[2]);
	
	read_count_file(argv[3], model);

    // code for loading multiple strings
    ssize_t read = 0;
    while (read != -1)
    {
        char *string = 0;
        size_t length = 0;
        // this is POSIX compliant (MS Windows is not)
        read = getline(&string, &length, stdin);
        if (read > 0)
        {
            clear();// clear count table
            char *here = string; // strchr(string, '|');
            if (here != 0)
            {
                size_t const len = strlen(here) - 1;
				double rate;
                accumulateN(here, len);  // add to model
				//complement(count);
				// apply algorithm
				switch(DIV) {
				
					case 0 :
						rate = divergence(count, model);
						break;

					case 1 :
						rate = divergenceEst(count, model);
						break;

					case 2 :
						rate = divergence(model, count);
						break;

					case 3 :
						rate = divergenceEst(model, count);
						break;

					default :
						fprintf(stderr, "DIV = %ld is invalid\n", DIV);
						return EXIT_FAILURE;
				}

				#if defined(DEBUG)
					fprintf(stdout, "%lf", rate);
					fwrite(" ", sizeof(char), 1, stdout);
				#else
					fprintf(stdout,"%lf\n", rate);
				#endif
            } // if

        } // if
        free(string);
    } // while

#if defined(DEBUG)
    fwrite("Apply model\n", sizeof(char), strlen("Apply model\n"), stdout);
    fwrite("tree depth: ", sizeof(char), strlen("tree depth: "), stdout);
    fwrite(argv[1], sizeof(char), strlen(argv[1]), stdout);
    fwrite("\n", sizeof(char), 1, stdout);
    print_count(model);
	print_count(count);
#endif
} // main

