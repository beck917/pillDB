/* 
 * File:   t_leveldb.c
 * Author: Beck Xu <beck917@gmail.com>
 *
 * Created on 2014年5月26日, 下午5:36
 */

#include "redis.h"
#include <math.h> /* isnan(), isinf() */

void leveldbInit()
{
    char *err = NULL;
    
    server.leveldb_lru_cache = 128;
    server.leveldb_create_if_missing = 1;
    server.leveldb_write_buffer_size = 128;
    server.leveldb_max_open_files = 1024;
    server.leveldb_block_size = 32;
    server.leveldb_path = "./db/leveldb";
    
    server.leveldb_cache = leveldb_cache_create_lru(server.leveldb_lru_cache * 1048576);
    server.leveldb_options = leveldb_options_create();
    
    leveldb_options_set_create_if_missing(server.leveldb_options, server.leveldb_create_if_missing);
    leveldb_options_set_cache(server.leveldb_options, server.leveldb_cache);
    leveldb_options_set_write_buffer_size(server.leveldb_options, server.leveldb_write_buffer_size * 1048576);
    leveldb_options_set_max_open_files(server.leveldb_options, server.leveldb_max_open_files);
    leveldb_options_set_block_size(server.leveldb_options, server.leveldb_block_size * 1024);
    
    server.leveldb = leveldb_open(server.leveldb_options, server.leveldb_path, &err);
    if (err != NULL) {
        redisLog(REDIS_WARNING,"%s:%d: %s\n", __FILE__, __LINE__, err);
        leveldb_free(err);
        exit(1);
    }
    
    server.leveldb_writeoptions = leveldb_writeoptions_create();
    server.leveldb_readoptions = leveldb_readoptions_create();
    leveldb_readoptions_set_verify_checksums(server.leveldb_readoptions, 0);
    leveldb_readoptions_set_fill_cache(server.leveldb_readoptions, 1);

    leveldb_writeoptions_set_sync(server.leveldb_writeoptions, 0);
}

void getLeveldbCommand(redisClient *c) {
    char *err;
    size_t val_len;
    char *key = NULL;
    char *value = NULL;


    err = NULL;
    key = (char *) c->argv[1]->ptr;
    value = leveldb_get(server.leveldb, server.leveldb_readoptions, key, sdslen((sds) key), &val_len, &err);
    if (err != NULL) {
        addReplyError(c, err);
        leveldb_free(err);
        leveldb_free(value);

        return;
    } else if (value == NULL) {
        addReply(c, shared.nullbulk);
        return;
    }

    addReplyBulkCBuffer(c, value, val_len);

    leveldb_free(value);

    return;
}

void setLeveldbCommand(redisClient *c) {
    char *key, *value;
    char *err = NULL;

    key = (char *) c->argv[1]->ptr;
    value = (char *) c->argv[2]->ptr;
    leveldb_put(server.leveldb, server.leveldb_writeoptions, key, sdslen((sds) key), value, sdslen((sds) value), &err);
    if (err != NULL) {
        addReplyError(c, err);
        leveldb_free(err);
        return;
    }
    addReply(c, shared.ok);
    return;
}

