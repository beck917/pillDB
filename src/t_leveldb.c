/* 
 * File:   t_leveldb.c
 * Author: Beck Xu <beck917@gmail.com>
 *
 * Created on 2014年5月26日, 下午5:36
 */

#include "redis.h"
#include <math.h> /* isnan(), isinf() */

void initLevelDB() 
{
    
    server.leveldb_cache = leveldb_cache_create_lru(server.leveldb_lru_cache * 1048576);
    server.leveldb_options = leveldb_options_create();
    
    leveldb_options_set_create_if_missing(server.leveldb_options, server.leveldb_create_if_missing);
    leveldb_options_set_cache(server.leveldb_options, server.leveldb_cache);
    leveldb_options_set_write_buffer_size(server.leveldb_options, server.leveldb_write_buffer_size * 1048576);
    leveldb_options_set_max_open_files(server.leveldb_options, server.leveldb_max_open_files);
    leveldb_options_set_block_size(server.ds_options, server.leveldb_block_size * 1024);
    
    server.leveldb = leveldb_open(server.leveldb_options, server.leveldb_path, &err);
    if (err != NULL) {
        leveldb_free(err);
        exit(1);
    }
}

