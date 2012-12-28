#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <glib.h>
#include "acdc.h"




unsigned int get_random_lifetime(MContext *mc, 
                                 unsigned int min, 
                                 unsigned int max);


GRand *init_rand(unsigned int seed);
void free_rand(GRand *rand);


#endif
