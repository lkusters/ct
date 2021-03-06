// Global size fitting in small memory model for k-mers upto KMAX
static size_t const KMAX = 10;  // max allowed value of treedepth K
static size_t const USIZE = 1 << (2 * KMAX);

// k-mer length
static size_t K = 4;  // default value
static size_t SIZE = 1 << (2 * K); // 4 ^ K

// Count table
static size_t count[USIZE][4] = {{0}};

// Symbolic constants used for printing
static char const ALPHA[4] =
{
    'A',
    'C',
    'G',
    'T'
}; // ALPHA

// Map a DNA base to an index (trivial)
static inline size_t to_idx(char const ch)
{
    switch (ch)
    {
        case 'A':
            return 0;
        case 'C':
            return 1;
        case 'G':
            return 2;
        case 'T':
            return 3;
    } // switch
    return 0;
} // to_idx

// Clear the count table
static inline void clear(void)
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        count[i][0] = 0;
        count[i][1] = 0;
        count[i][2] = 0;
        count[i][3] = 0;
    } // for
} // clear

// Print a key
static inline void printkey(size_t const keyidx)
{
    for (size_t j = 0; j < K; ++j)
    {
        fprintf(stdout, "%c", ALPHA[(keyidx >> ((K - j - 1) * 2)) & 3]);
    } // for
} // printkey

// Build a count table. Expects strings consisting of 'A', 'C', 'G' or
// 'T' only!
static inline void accumulate(char const* const string,
                              size_t const      length)
{
    size_t key = 0;
    for (size_t i = 0; i < K; ++i)
    {
        key >>= 2;
        key |= (to_idx(string[i]) << (2*(K-1)));
    } // for

    for (size_t i = K; i < length; ++i)
    {
        count[key % SIZE][to_idx(string[i])] += 1;
        key >>= 2;
        key |= (to_idx(string[i]) << (2*(K-1)));
    } // for
} // accumulate

// Check whether a character is a valid base
static inline bool is_base(char const ch)
{
    return ch == 'A' ||
           ch == 'C' ||
           ch == 'G' ||
           ch == 'T';
} // is_base

// Calculate the context key from a given start position
static inline size_t calc_key(char const* const string,
                              size_t const      length,
                              size_t const      start)
{
    if (start + K > length)
    {
        return -1;
    } // if

    size_t key = 0;
    for (size_t i = start; i < start + K; ++i)
    {
        if (!is_base(string[i]))
        {
            return -1;
        } // if
        key >>= 2;
        key |= (to_idx(string[i]) << (2*(K-1)));
    } // for
    return key;
} // calc_key

// Build the count table with Ns
static inline size_t accumulateN(char const* const string,
                                 size_t const      length)
{
    size_t i = 0;
    size_t cnt = 0;


    while (i < length)
    {
        // ignore non-bases (N)
        while (i < length && !is_base(string[i]))
        {
            i += 1;
        } // while

        // Calculate the context key from the first valid base
        size_t key = calc_key(string, length, i);
        i += K;

        // Skip invalid keys (context between Ns is too small)
        if (key == static_cast<size_t>(-1))
        {
            continue;
        } // if
        
        // Accumulate the counts
        while (i < length && is_base(string[i]))
        {
            count[key % SIZE][to_idx(string[i])] += 1;
            cnt += 1;
            key >>= 2;
            key |= (to_idx(string[i]) << (2*(K-1)));
            i += 1;
        } // while
    } // while
    return cnt;
} // accumulateN


static inline void print_count(size_t const count1[USIZE][4])
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        printkey(i);
        fprintf(stdout, ": %9ld %9ld %9ld %9ld\n", count1[i][0],
                                                   count1[i][1],
                                                   count1[i][2],
                                                   count1[i][3]);
    } // for
} // print_count

static inline void print_model(double const prob[USIZE][4])
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        printkey(i);
        fprintf(stdout, ": %9lf %9lf %9lf %9lf\n", prob[i][0],
                                                   prob[i][1],
                                                   prob[i][2],
                                                   prob[i][3]);
    } // for
} // print_model


// Save a probability table in string form to stdout
static inline size_t write_model(void)
{
    return fwrite(count[0], sizeof(count[0][0]), SIZE * 4, stdout);
} // write_model

/*
// Load a probability table in binary form from stdin
static inline void read_model(void)
{
    size_t const read = fread(count, sizeof(count[0][0]), SIZE * 4, stdin);
	size_t helpvar;
	size_t const readcheck = fread(&helpvar, sizeof(size_t), 1, stdin);

	// fprintf(stdout, "read: %zu\n", read);
	// fprintf(stdout, "SIZE: %zu\n", SIZE);
	// fprintf(stdout, "readcheck: %zu\n", readcheck);
    if (read != SIZE * 4 || (readcheck!=0))
    {
        fprintf(stderr, "fread failed\n");
        exit(EXIT_FAILURE);
    } // if
} // read_model
*/

// Load a count table in binary form from a file
static inline void read_count_file(char *filename, size_t ccount[USIZE][4])
{
    FILE* file = fopen(filename, "rb");
    if (file == 0)
    {
		fprintf(stderr, "fopen failed\n");
		exit(EXIT_FAILURE);
    } // if

    size_t const read = fread(ccount, sizeof(ccount[0][0]), SIZE * 4, file);
	size_t helpvar;
	size_t const readcheck = fread(&helpvar, sizeof(size_t), 1, file);
    
	// fprintf(stdout, "read: %zu\n", read);
	// fprintf(stdout, "SIZE: %zu\n", SIZE);
	// fprintf(stdout, "readcheck: %zu\n", readcheck);
	
    if ((read != SIZE * 4) || (readcheck!=0))
    {
		fprintf(stderr, "fread failed\n");
		exit(EXIT_FAILURE);
    } // if
	fclose(file);
} // read_model

// add reverse complement to the input model counts
static inline void complement(size_t ccount[USIZE][4])
{
    
    for (size_t i = 0; i < SIZE; ++i)
    {
        size_t key = 0;
		size_t const idx = (~i) & 3;

		#if defined(DEBUG)
			fwrite("symbol", sizeof(char), strlen("symbol"), stdout);
			fprintf(stdout, "%c", ALPHA[idx]);
			fwrite("\n", sizeof(char), 1, stdout);
	    #endif
		for (size_t k = 1; k < K; ++k)
		{
			key |= ( (~(i >> (2 * k))) & 3 ) ;
			key <<= 2;
		} // for
		
		#if defined(DEBUG)
			fwrite("kkeys: ", sizeof(char), strlen("kkeys: "), stdout);
		#endif
        for (size_t j = 0; j < 4; ++j)
        {
            size_t const kkey = key | ((~j)&3);
			#if defined(DEBUG)
				printkey(kkey);
				fwrite(" ", sizeof(char), 1, stdout);
				fprintf(stdout, "%9ld", ccount[i][j]);
				fwrite("+", sizeof(char), 1, stdout);
				fprintf(stdout, "%9ld", ccount[kkey][idx]);
				fwrite(" ", sizeof(char), 1, stdout);
	    	#endif

	    	if (kkey >= i)
            {
                ccount[i][j] += ccount[kkey][idx];
                ccount[kkey][idx] = ccount[i][j];
            } // if
        } // for
		#if defined(DEBUG)
			fwrite("\n", sizeof(char), 1, stdout);
		#endif
    } // for
} // complement

// Calculate the log2 probabilities from a count table
static inline void normalize(size_t const count[USIZE][4],
                             double       prob[USIZE][4])
{
	    
	for (size_t i = 0; i < SIZE; ++i)
    {
        double const SUM = static_cast<double>(count[i][0]) + .5 +
                           static_cast<double>(count[i][1]) + .5 +
                           static_cast<double>(count[i][2]) + .5 +
                           static_cast<double>(count[i][3]) + .5;

        prob[i][0] = log2((static_cast<double>(count[i][0]) + .5) / SUM);
        prob[i][1] = log2((static_cast<double>(count[i][1]) + .5) / SUM);
        prob[i][2] = log2((static_cast<double>(count[i][2]) + .5) / SUM);
        prob[i][3] = log2((static_cast<double>(count[i][3]) + .5) / SUM);
    } // for
} // normalize

// Calculate Divergence
static inline double divergence(size_t const count1[USIZE][4],
                             size_t const  count2[USIZE][4])
{
	double SUMS;
	SUMS = 0;
	double div;
	div = 0;    
	for (size_t i = 0; i < SIZE; ++i)
    {
		      
		double const SUM1 = static_cast<double>(count1[i][0] ) +.5 +
                           static_cast<double>(count1[i][1] ) +.5 +
                           static_cast<double>(count1[i][2] ) +.5 +
                           static_cast<double>(count1[i][3] ) +.5;
		SUMS += SUM1-2; // total SUM
		
		if ((SUM1-2) > 0) // context has occurred in test sequence
		{
			#if defined(DEBUG) 
				printkey(i);
				fwrite(" ", sizeof(char), 1, stdout);  			
			#endif
		double const SUM2 = static_cast<double>(count2[i][0] ) +.5 +
                        static_cast<double>(count2[i][1] ) +.5 +
                        static_cast<double>(count2[i][2] ) +.5 +
                        static_cast<double>(count2[i][3] ) +.5;
		for (size_t j = 0; j < 4; ++j)
			{        	
				double const prob1 = (static_cast<double>(count1[i][j]) + .5) / SUM1;				
				double const lprob1 = log2((static_cast<double>(count1[i][j]) + .5) / SUM1);
				double const lprob2 = log2((static_cast<double>(count2[i][j]) + .5) / SUM2);				
						
				div += (SUM1-2)* prob1 * ( lprob1 - lprob2); // 
				#if defined(DEBUG)
					fprintf(stdout, "%lf", (SUM1-2) );
					fwrite("*", sizeof(char), 1, stdout);
					fprintf(stdout, "%lf", (prob1) );
					fwrite("*(", sizeof(char), 2, stdout);
 					fprintf(stdout, "%lf", lprob1);
					fwrite("-", sizeof(char), 1, stdout);
					fprintf(stdout, "%lf", lprob2);
					fwrite(")  ", sizeof(char), 3, stdout);
				#endif
        	}
			#if defined(DEBUG)
				fwrite("\n", sizeof(char), 1, stdout);  			
			#endif
		}
		
		
    } // for
	return div/SUMS;
} // Divergence

// Calculate Divergence estimate
static inline double divergenceEst(size_t const count1[USIZE][4],
                             size_t const  count2[USIZE][4])
{
	double SUMS;
	SUMS = 0;
	double div;
	div = 0;    
	for (size_t i = 0; i < SIZE; ++i)
    {
		      
		double const SUM1 = static_cast<double>(count1[i][0] ) +.5 +
                           static_cast<double>(count1[i][1] ) +.5 +
                           static_cast<double>(count1[i][2] ) +.5 +
                           static_cast<double>(count1[i][3] ) +.5;
		SUMS += SUM1-2; // total SUM
		
		if ((SUM1-2) > 0) // context has occurred in test sequence
		{
			#if defined(DEBUG) 
				printkey(i);
				fwrite(" ", sizeof(char), 1, stdout);  			
			#endif
		double const SUM2 = static_cast<double>(count2[i][0] ) +.5 +
                            static_cast<double>(count2[i][1] ) +.5 +
                            static_cast<double>(count2[i][2] ) +.5 +
                            static_cast<double>(count2[i][3] ) +.5;

		for (size_t j = 0; j < 4; ++j)
			{        	
				double const lprob1 = log2((static_cast<double>(count1[i][j]) + .5) / SUM1);
				double const lprob2 = log2((static_cast<double>(count2[i][j]) + .5) / SUM2);
												
				div += (count1[i][j])* ( lprob1 - lprob2); // 
				#if defined(DEBUG)
					fprintf(stdout, "%9ld", (count1[i][j]) );
					fwrite("*(", sizeof(char), 2, stdout);
 					fprintf(stdout, "%lf", lprob1);
					fwrite("-", sizeof(char), 1, stdout);
					fprintf(stdout, "%lf", lprob2);
					fwrite(")  ", sizeof(char), 3, stdout);
				#endif
        	}
			#if defined(DEBUG)
				fwrite("\n", sizeof(char), 1, stdout);  			
			#endif
		}
		
		
    } // for
	return div/SUMS;
} // Divergence estimate
// Calculate compression rates R(X|MX) R(X|MY) R(Y|MX) R(Y|MY)
static inline void calcrates(size_t const count1[USIZE][4],
                             size_t const  count2[USIZE][4],
							double rates[4])
{
	double SUMS1;
	SUMS1 = 0;
	double SUMS2;
	SUMS2 = 0;
	rates[0]=0;rates[1]=0;rates[2]=0;rates[3]=0;
   
	for (size_t i = 0; i < SIZE; ++i)
    {
		      
		double const SUM1 = static_cast<double>(count1[i][0] ) +.5 +
                           static_cast<double>(count1[i][1] ) +.5 +
                           static_cast<double>(count1[i][2] ) +.5 +
                           static_cast<double>(count1[i][3] ) +.5;
		SUMS1 += SUM1-2; // total SUM

		double const SUM2 = static_cast<double>(count2[i][0] ) +.5 +
                            static_cast<double>(count2[i][1] ) +.5 +
                            static_cast<double>(count2[i][2] ) +.5 +
                            static_cast<double>(count2[i][3] ) +.5;
		SUMS2 += SUM2-2; // total SUM

		
		for (size_t j = 0; j < 4; ++j)
			{        	
								
				double const lprob1 = log2((static_cast<double>(count1[i][j]) + .5) / SUM1);
				double const lprob2 = log2((static_cast<double>(count2[i][j]) + .5) / SUM2);
				// R(X|MX)
				rates[0] -= count1[i][j]*lprob1;
				// R(X|MY)
				rates[1] -= count1[i][j]*lprob2;
				// R(Y|MX)
				rates[2] -= count2[i][j]*lprob1;
				// R(Y|MY)
				rates[3] -= count2[i][j]*lprob2;
												
        	} // for i

    } // for j
	#if defined(DEBUG)
	fprintf(stdout, "%lf %lf %lf\n", rates[0], SUMS1,rates[0] / SUMS1);
	fprintf(stdout, "%lf %lf %lf\n", rates[1], SUMS1,rates[1] / SUMS1);
	fprintf(stdout, "%lf %lf %lf\n", rates[2], SUMS2,rates[2] / SUMS2);
	fprintf(stdout, "%lf %lf %lf\n", rates[3], SUMS2,rates[3] / SUMS2);
	#endif
	rates[0] = rates[0] / SUMS1;rates[1] = rates[1] / SUMS1;
	rates[2] = rates[2] / SUMS2;rates[3] = rates[3] / SUMS2;

} // calcrates()
