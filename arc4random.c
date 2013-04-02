#include <stdlib.h>
int
arc4random_uniform(int upper_bound)
{
     return random() % upper_bound;
}
