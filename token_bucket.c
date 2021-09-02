#include <sys/time.h>
#include <stdbool.h>
#include <stddef.h>
#include "token_bucket.h"

/*
struct token_bucket
{
        unsigned int rate;			// bit/s
        unsigned int bucket_size;	// bit

        unsigned int full_time;		// ms
        volatile unsigned long int old;
};
*/

bool token_bucket_init(struct token_bucket *tb, unsigned int rate, unsigned int bucket_size)
{
        tb->rate = rate;
        tb->bucket_size = bucket_size;

        tb->full_time = bucket_size * 1000 / rate;	// ms
        struct timeval old_t;
        gettimeofday(&old_t, NULL);
		//Make it full at the beginning
        tb->old = old_t.tv_sec*1000 + old_t.tv_usec/1000 - tb->full_time;
}

bool token_bucket_consume(struct token_bucket *tb, unsigned int tokens)
{
        struct timeval now_t;
        unsigned long int old, now;
        unsigned long int rtime;
        unsigned long int consume_time = tokens * 1000 / tb->rate;     //ms

        for(;;)
        {
                gettimeofday(&now_t, NULL);
                now = now_t.tv_sec*1000 + now_t.tv_usec/1000;
                old = tb->old;
                rtime = (old > (now - tb->full_time)) ? old : (now - tb->full_time);
                rtime += consume_time;
                if(rtime > now)
                        return false;
                if(__sync_bool_compare_and_swap(&tb->old, old, rtime))
                        return true;
        }
}
