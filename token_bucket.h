#ifndef _TOKEN_BUCKET_H_
#define _TOKEN_BUCKET_H_

#include <stdbool.h>

struct token_bucket
{
        unsigned int rate;				// bit/s
        unsigned int bucket_size;		// bit

        unsigned int full_time;			// ms
        volatile unsigned long int old; // ms
};

bool token_bucket_init(struct token_bucket *tb, unsigned int rate, unsigned int bucket_size);
bool token_bucket_consume(struct token_bucket *tb, unsigned int tokens);

#endif