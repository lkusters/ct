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

static double modelprob[USIZE][4] = {{0}};
static size_t model[USIZE][4] = {{0}};

int main(int argc, char *argv[])
{
	#if defined(DEBUG)
		fwrite("---- DEBUG mode ---- \n", sizeof(char), strlen("---- DEBUG mode ---- \n"), stdout);
	#endif    
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s K Model data\n", argv[0]);
        return EXIT_FAILURE;
    } // if

    K = atoi(argv[1]);
    SIZE = 1 << (2 * K); // 4 ^ K

    if (K > KMAX)
    {
        fprintf(stderr, "K = %ld is too large\n", K);
        return EXIT_FAILURE;
    } // if

	
	read_count_file(argv[2], model);
	// convert to probabilities
	//normalize(model, modelprob);

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
                accumulateN(here, len);  // add to model
				//complement(count);
				// apply algorithm
				//double const rate = divergence(count, model);
				double const rate = divergenceEst(count, model);

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
    print_model(modelprob);
#endif
} // main

