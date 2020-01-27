/* $Id: //WIFI_SOC/release/SDK_4_1_0_0/source/user/snort-2.8.5.2/src/mempool.c#1 $ */
/*
** Copyright (C) 2002-2009 Sourcefire, Inc.
** Copyright (C) 2002 Martin Roesch <roesch@sourcefire.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/*
 * This is used to allocate a list of fixed size objects in a static
 * memory pool aside from the concerns of alloc/free
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "mempool.h"
#include "util.h"
#include "debug.h"

static INLINE void mempool_free_pools(MemPool *mempool)
{
    if (mempool == NULL)
        return;

    if (mempool->datapool != NULL)
    {
        free(mempool->datapool);
        mempool->datapool = NULL;
    }

    if (mempool->listpool != NULL)
    {
        free(mempool->listpool);
        mempool->listpool = NULL;
    }

    if (mempool->bucketpool != NULL)
    {
        free(mempool->bucketpool);
        mempool->bucketpool = NULL;
    }
}

/* Function: int mempool_init(MemPool *mempool,
 *                            PoolCount num_objects, size_t obj_size)
 * 
 * Purpose: initialize a mempool object and allocate memory for it
 * Args: mempool - pointer to a MemPool struct
 *       num_objects - number of items in this pool
 *       obj_size    - size of the items
 * 
 * Returns: 0 on success, 1 on failure
 */ 

int mempool_init(MemPool *mempool, PoolCount num_objects, size_t obj_size)
{
    PoolCount i;
    
    if(mempool == NULL)
        return 1;

    if(num_objects < 1)
        return 1;

    if(obj_size < 1)
        return 1;

    mempool->obj_size = obj_size;
    
    /* this is the basis pool that represents all the *data pointers
       in the list */
    mempool->datapool = calloc(num_objects, obj_size);
    if(mempool->datapool == NULL)
        return 1;

    mempool->listpool = calloc(num_objects, sizeof(SDListItem));
    if(mempool->listpool == NULL)
    {
        /* well, that sucked, lets clean up */
        ErrorMessage("%s(%d) mempool_init(): listpool is null\n",
                     __FILE__, __LINE__);
        mempool_free_pools(mempool);
        return 1;
    }

    mempool->bucketpool = calloc(num_objects, sizeof(MemBucket));
    if(mempool->bucketpool == NULL)
    {
        ErrorMessage("%s(%d) mempool_init(): bucketpool is null\n",
                     __FILE__, __LINE__);
        mempool_free_pools(mempool);
        return 1;
    }

    /* sets up the 2 memory lists */
    if(sf_sdlist_init(&mempool->used_list, NULL))
    {
        ErrorMessage("%s(%d) mempool_init(): Failed to initialize used list\n",
                     __FILE__, __LINE__);
        mempool_free_pools(mempool);
        return 1;
    }

    if(sf_sdlist_init(&mempool->free_list, NULL))
    {
        ErrorMessage("%s(%d) mempool_init(): Failed to initialize free list\n",
                     __FILE__, __LINE__);
        mempool_free_pools(mempool);
        return 1;
    }


    for(i=0; i<num_objects; i++)
    {
        SDListItem *itemp;
        MemBucket *bp;

        bp = &mempool->bucketpool[i];
        itemp = &mempool->listpool[i];
        
        /* each bucket knows where it resides in the list */
        bp->key = itemp;

#ifdef TEST_MEMPOOL        
        printf("listpool: %p itemp: %p diff: %u\n",
               mempool->listpool, itemp,
               (((char *) itemp) -
                ((char *) mempool->listpool)));
#endif /* TEST_MEMPOOL */
               
        bp->data = ((char *) mempool->datapool) + (i * mempool->obj_size);
        
#ifdef TEST_MEMPOOL        
        printf("datapool: %p bp.data: %p diff: %u\n",
               mempool->datapool,
               mempool->datapool + (i * mempool->obj_size),
               (((char *) bp->data) -
                ((char *) mempool->datapool)));
#endif /* TEST_MEMPOOL */
        

        if(sf_sdlist_append(&mempool->free_list,                           
                            &mempool->bucketpool[i],
                            &mempool->listpool[i]))
        {
            ErrorMessage("%s(%d) mempool_init(): Failed to add to free list\n",
                         __FILE__, __LINE__);
            mempool_free_pools(mempool);
            return 1;
        }

        mempool->free++;
    }

    mempool->used = 0;
    mempool->total = num_objects;
    
    return 0;
}

/* Function: int mempool_clean(MemPool *mempool) 
 * 
 * Purpose: return all memory to free list
 * Args: mempool - pointer to a MemPool struct
 * 
 * Returns: 0 on success, -1 on failure
 */ 
int mempool_clean(MemPool *mempool)
{
    unsigned int i;
    int ret;

    if (mempool == NULL)
        return -1;

    /* clean used list */
    ret = sf_sdlist_delete(&mempool->used_list);
    if (ret != 0)
        return -1;
    mempool->used = 0;

    /* clean free list */
    ret = sf_sdlist_delete(&mempool->free_list);
    if (ret != 0)
        return -1;
    mempool->free = 0;

    /* add everything back to free list */
    for (i = 0; i < mempool->total; i++)
    {
        ret = sf_sdlist_append(&mempool->free_list, &mempool->bucketpool[i],
                               &mempool->listpool[i]);
        if (ret == -1)
            return -1;

        mempool->free++;
    }

    return 0;
}

/* Function: int mempool_destroy(MemPool *mempool) 
 * 
 * Purpose: destroy a set of mempool objects
 * Args: mempool - pointer to a MemPool struct
 * 
 * Returns: 0 on success, 1 on failure
 */ 
int mempool_destroy(MemPool *mempool)
{
    if(mempool == NULL)
        return 1;

    mempool_free_pools(mempool);

    /* TBD - callback to free up every stray pointer */
    memset(mempool, 0, sizeof(MemPool));
    
    return 0;    
}


/* Function: MemBucket *mempool_alloc(MemPool *mempool);
 * 
 * Purpose: allocate a new object from the mempool
 * Args: mempool - pointer to a MemPool struct
 * 
 * Returns: a pointer to the mempool object on success, NULL on failure
 */ 
MemBucket *mempool_alloc(MemPool *mempool)
{
    SDListItem *li = NULL;
    MemBucket *b;
    
    if(mempool == NULL)
    {
        return NULL;
    }

    if(mempool->free <= 0)
    {
        return NULL;
    }

    /* get one item off the free_list,
       put one item on the usedlist
     */

    li = mempool->free_list.head;

    if((li == NULL) || sf_sdlist_remove(&mempool->free_list, li))
    {
        printf("Failure on sf_sdlist_remove\n");
        return NULL;
    }

    mempool->free--;
    mempool->used++;

    if(sf_sdlist_append(&mempool->used_list, li->data, li))
    {
        printf("Failure on sf_sdlist_append\n");
        return NULL;
    }

    /* TBD -- make configurable */
    b = li->data;
    bzero(b->data, mempool->obj_size);
    
    return b;
}

void mempool_free(MemPool *mempool, MemBucket *obj)
{       
    if(sf_sdlist_remove(&mempool->used_list, obj->key))
    {
        printf("failure on remove from used_list");
        return;
    }
    
    mempool->used--;

    /* put the address of the membucket back in the list */
    if(sf_sdlist_append(&mempool->free_list, obj, obj->key))
    {
        printf("failure on add to free_list");
        return;
    }

    mempool->free++;    
    return;
}

#ifdef TEST_MEMPOOL

#define SIZE 36
int main(void)
{
    MemPool test;
    MemBucket *bucks[SIZE];
    MemBucket *bucket = NULL;
    int i;
    long long a = 1;

    //char *stuffs[4] = { "eenie", "meenie", "minie", "moe" };
    char *stuffs2[36] =
        {  "1eenie", "2meenie", "3minie", " 4moe",
           "1xxxxx", "2yyyyyy", "3zzzzz", " 4qqqq",
           "1eenie", "2meenie", "3minie", " 4moe",
           "1eenie", "2meenie", "3minie", " 4moe",
           "1eenie", "2meenie", "3minie", " 4moe",
           "1eenie", "2meenie", "3minie", " 4moe",
           "1eenie", "2meenie", "3minie", " 4moe",
           "1eenie", "2meenie", "3minie", " 4moe",
           "1eenie", "2meenie", "3minie", " 4moe"
        };
    
    if(mempool_init(&test, 36, 256))
    {
        printf("error in mempool initialization\n");
    }

    for(i = 0; i < 36; i++)
    {
        if((bucks[i] = mempool_alloc(&test)) == NULL)
        {
            printf("error in mempool_alloc: i=%d\n", i);
            continue;
        }

        bucket = bucks[i];

        bucket->data = strncpy(bucket->data, stuffs2[i], 256);
        printf("bucket->key: %p\n", bucket->key);
        printf("bucket->data: %s\n", (char *) bucket->data);
    }

    for(i = 0; i < 2; i++)
    {
        mempool_free(&test, bucks[i]);
        bucks[i] = NULL;
    }
    
    for(i = 0; i < 14; i++)
    {
        if((bucks[i] = mempool_alloc(&test)) == NULL)
        {
            printf("error in mempool_alloc: i=%d\n", i);
            continue;
        }

        bucket = bucks[i];

        bucket->data = strncpy(bucket->data, stuffs2[i], 256);
        printf("bucket->key: %p\n", bucket->key);
        printf("bucket->data: %s\n", (char *) bucket->data);
    }

    printf("free: %u, used: %u\n", test.free, test.used);

    
    return 0;
}
#endif /* TEST_MEMPOOL */

