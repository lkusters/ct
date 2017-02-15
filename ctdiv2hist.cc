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

static double SUMS = 0;
static size_t count = 0;
static double min = 10;
static double max = -10;

int main(int argc, char *argv[])
{

    if (argc < 1)
    {
        fprintf(stderr, "usage: %s avg datastream\n", argv[0]);
        return EXIT_FAILURE;
    } // if

	char *string = 0;
    size_t length = 0;
	double avg;
	ssize_t read = 0;
    // this is POSIX compliant (MS Windows is not)
    read = getline(&string, &length, stdin);
    if (read > 0)
	{
			char *here = string; 
            if (here != 0)
            {
                sscanf(string, "%lf", &avg);
			}
	}
	free(string);
    while (read != -1)
    {
        char *string = 0;
        size_t length = 0;
		double div;
        // this is POSIX compliant (MS Windows is not)
        read = getline(&string, &length, stdin);
        if (read > 0)
        {
            
			char *here = string; 
            if (here != 0)
            {
                sscanf(string, "%lf", &div);
				count += 1;
				SUMS += (div-avg)*(div-avg);
				if (div < min )
				{
					min = div;
				} // if
				if (div > max)
				{
					max = div;
				}

            } // if

        } // if
        free(string);
    } // while
	double const var = static_cast<double>(SUMS)/
                           static_cast<double>(count-1);

	fprintf(stdout, "%lf\n", min);
	fprintf(stdout, "%lf\n", max);
	fprintf(stdout, "%lf\n", avg);
	fprintf(stdout, "%lf\n", var);
}
