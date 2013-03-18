#include <stdlib.h>
u_int32_t
arc4random_uniform(u_int32_t upper_bound)
{
     return random() % upper_bound;
}
