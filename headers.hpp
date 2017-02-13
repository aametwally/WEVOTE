#ifndef WEVOTE_HEADERS
#define WEVOTE_HEADERS

#include "stdio.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <omp.h>
#include <set>
#include <sstream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sysexits.h>
#include <unistd.h>
#include <vector>
#include <math.h>
#include <getopt.h>

using namespace std;

typedef struct {
	string seqID;
	vector<uint32_t> annotation;
	uint32_t resolvedTaxon;
	uint32_t numToolsAgreed;
	uint32_t numToolsReported;
	uint32_t numToolsUsed;
	double score;
} readsInfo;


typedef struct {
	uint32_t taxonID;
	string rank;
	string name;
	uint32_t occurrence;
	double abundance;
} taxon;


#endif
