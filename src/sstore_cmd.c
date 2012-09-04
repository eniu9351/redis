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



				//printf("sstore 0, key=%s\n", c->argv[1]->ptr);

				if(1==2)	{
					return;
					}
				//1. get value
				if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.emptymultibulk)) == NULL
												|| checkType(c,o,REDIS_HASH)) {
								printf("sstoreDelandlogCommand --- o null!");
								return;
				}

				//printf("sstore 1\n");
				//	addReplyLongLong(c,deleted);

				//2. log
				bson_init( b );
				bson_append_new_oid( b , "_id" );
				//printf("sstore 2\n");

				//				replylen = addDeferredMultiBulkLength(c);
				hashTypeIterator* hi = hashTypeInitIterator(o);
				while (hashTypeNext(hi) != REDIS_ERR) {
								robj *obj;
								char *v = NULL;
								unsigned int vlen = 0;
								char *k = NULL;
								unsigned int klen = 0;
								int encoding;

								//printf("sstore iterate start\n");

								//								if (flags & REDIS_HASH_KEY) {
								encoding = hashTypeCurrent(hi,REDIS_HASH_KEY,&obj,&k,&klen);
								//if (encoding == REDIS_ENCODING_HT)
								//					addReplyBulk(c,obj);
								//	else
								//					addReplyBulkCBuffer(c,v,vlen);
								//count++;
								//								}

								printf("sstore iterate 1, encoding of key=%d, klen=%d\n", encoding, klen);
								
								//								if (flags & REDIS_HASH_VALUE) {

								if(encoding==REDIS_ENCODING_HT)	{
									k = obj->ptr;
									//printf("k=%s\n",obj->ptr);
								}
								else	{
									//printf("k=%s\n",k);
								}


								encoding = hashTypeCurrent(hi,REDIS_HASH_VALUE,&obj,&v,&vlen);

								if(encoding==REDIS_ENCODING_HT)	{
									v = obj->ptr;
									//printf("v=%s\n",obj->ptr);
								}
								else	{
									//printf("v=%s\n",v);
								}


								
								//vcopy = (char*)malloc(vlen+1);
								//strncpy(vcopy, v, vlen);
								
								//	if (encoding == REDIS_ENCODING_HT)
								//					addReplyBulk(c,obj);
								//	else
								//					addReplyBulkCBuffer(c,v,vlen);
								//count++;
								//								}
								//printf("sstore iterate 2, encoding of value=%d, vlen=%d\n", encoding, vlen);


								//printf("sstore iterate end, k=%s, v=%s\n", k, v);
								
								int bapresult = bson_append_string( b, k, v);
								if(bapresult!=BSON_OK)	{
											printf("bapresult=%d\n", bapresult);
									}

				}

				hashTypeReleaseIterator(hi);


				printf("sstore 3\n");
				//3. delete
				//dbDelete(c->db,oKey);

				bson_finish(b);
				mongo_insert(server.mongoc, "sstore.users", b, NULL);
				printf("sstore end\n");
}
