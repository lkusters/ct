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


static inline void print_count(void)
{
    for (size_t i = 0; i < SIZE; ++i)
    {
        printkey(i);
        fprintf(stdout, ": %9ld %9ld %9ld %9ld\n", count[i][0],
                                                   count[i][1],
                                                   count[i][2],
                                                   count[i][3]);
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

// Load a probability table in binary form from stdin
static inline void read_model(void)
{
    size_t const read = fread(count, sizeof(count[0][0]), SIZE * 4, stdin);
    if (read != SIZE * 4)
    {
        fprintf(stderr, "fread failed\n");
        exit(EXIT_FAILURE);
    } // if
} // read_model

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
    fclose(file);
    if (read != SIZE * 4)
    {
		fprintf(stderr, "fread failed\n");
		exit(EXIT_FAILURE);
    } // if
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
        double const SUM = static_cast<double>(count[i][0] + 1) +
                           static_cast<double>(count[i][1] + 1) +
                           static_cast<double>(count[i][2] + 1) +
                           static_cast<double>(count[i][3] + 1);
        prob[i][0] = log2(static_cast<double>(count[i][0] + 1) / SUM);
        prob[i][1] = log2(static_cast<double>(count[i][1] + 1) / SUM);
        prob[i][2] = log2(static_cast<double>(count[i][2] + 1) / SUM);
        prob[i][3] = log2(static_cast<double>(count[i][3] + 1) / SUM);
    } // for
} // normalize

// Calculate Divergence
static inline double divergence(size_t const count[USIZE][4],
                             double const  prob[USIZE][4])
{
	double SUMS;
	SUMS = 0;
	double rate;
	rate = 0;    
	for (size_t i = 0; i < SIZE; ++i)
    {
		      
		double const SUM = static_cast<double>(count[i][0] ) +
                           static_cast<double>(count[i][1] ) +
                           static_cast<double>(count[i][2] ) +
                           static_cast<double>(count[i][3] );
		SUMS += SUM; // total SUM
		
		if (SUM > 0)
		{
			#if defined(DEBUG)
				printkey(i);
			#endif
			fwrite(" ", sizeof(char), 1, stdout);  			
			for (size_t j = 0; j < 4; ++j)
			{        	
				rate += static_cast<double>(count[i][j] ) * ( log2(static_cast<double>(count[i][j] + 1)) - log2(SUM) - prob[i][j]); // 
        	}
		}
		
    } // for
	return rate/SUMS;
} // Divergence
