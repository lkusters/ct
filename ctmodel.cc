// Expects raw sequences (only uppercase 'A', 'C', 'G' or 'T') on a
// line-by-line basis
// constructs the model

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

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s K\n", argv[0]);
        return EXIT_FAILURE;
    } // if

    K = atoi(argv[1]);
    SIZE = 1 << (2 * K); // 4 ^ K

    if (K > KMAX)
    {
        fprintf(stderr, "K = %ld is too large\n", K);
        return EXIT_FAILURE;
    } // if

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
            //clear();
            char *here = string; // strchr(string, '|');
            if (here != 0)
            {
                size_t const len = strlen(here) - 1;
                accumulateN(here, len);  // add to model
            } // if

        } // if
        free(string);
    } // while


#if defined(DEBUG)
    fwrite("Construct model\n", sizeof(char), strlen("Construct model\n"), stdout);
    fwrite("tree depth: ", sizeof(char), strlen("tree depth: "), stdout);
    fwrite(argv[1], sizeof(char), strlen(argv[1]), stdout);
    //fwrite(argv[2], sizeof(char), strlen(argv[2]), stdout);
    //fwrite(argv[3], sizeof(char), strlen(argv[3]), stdout);
    fwrite("\n", sizeof(char), 1, stdout);
    print_count();
    
#endif
    write_model();
} // main

