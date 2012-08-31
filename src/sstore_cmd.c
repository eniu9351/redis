#include "redis.h"


#include <stdio.h>
#include <mongo.h>
//#include <bson.h>


void sstoreDelandlogCommand(redisClient *c)
{
	/*
	int deleted = 0, j;
	for (j = 1; j < c->argc; j++) {
			if (dbDelete(c->db,c->argv[j])) {
					signalModifiedKey(c->db,c->argv[j]);
					server.dirty++;
					deleted++;
			}
	}
	*/

				robj *o;
				bson b[1];
				char* vcopy;

				printf("sstore 0\n");
				//1. get value
				if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.emptymultibulk)) == NULL
												|| checkType(c,o,REDIS_HASH)) {
								printf("sstoreDelandlogCommand --- o null!");
								return;
				}

				printf("sstore 1\n");
				//	addReplyLongLong(c,deleted);

				//2. log
				bson_init( b );
				bson_append_new_oid( b , "_id" );
				printf("sstore 2\n");

				//				replylen = addDeferredMultiBulkLength(c);
				hashTypeIterator* hi = hashTypeInitIterator(o);
				while (hashTypeNext(hi) != REDIS_ERR) {
								robj *obj;
								char *v = NULL;
								unsigned int vlen = 0;
								char *k = NULL;
								unsigned int klen = 0;
								int encoding;

								//								if (flags & REDIS_HASH_KEY) {
								encoding = hashTypeCurrent(hi,REDIS_HASH_KEY,&obj,&k,&klen);
								//if (encoding == REDIS_ENCODING_HT)
								//					addReplyBulk(c,obj);
								//	else
								//					addReplyBulkCBuffer(c,v,vlen);
								//count++;
								//								}
								//								if (flags & REDIS_HASH_VALUE) {
								encoding = hashTypeCurrent(hi,REDIS_HASH_VALUE,&obj,&v,&vlen);
								printf("k=%s\n",k);
								vcopy = (char*)malloc(vlen+1);
								strncpy(vcopy, v, vlen);
								printf("v=%s\n",v);
								//	if (encoding == REDIS_ENCODING_HT)
								//					addReplyBulk(c,obj);
								//	else
								//					addReplyBulkCBuffer(c,v,vlen);
								//count++;
								//								}
								bson_append_string( b, k, vcopy);
				}

				printf("sstore 3\n");
				//3. delete
				//dbDelete(c->db,oKey);

				bson_finish(b);
				mongo_insert(server.mongoc, "sstore.users", b, NULL);
				printf("sstore end\n");
}
