/**
 * @file json2.h
 * File Encoding UTF8
 * json字串生成库.
 */

#ifndef JSON2_H_
#define JSON2_H_
#include <errno.h>
#define DEBUG_JSON_CLEAR 0 /* 调试选项 */

#define IS_VALUE 1
#define IS_STRING 0
// 追加函数,最后一个参数默认为字符串(IS_STRING)
#define jsonAdd(objJson,key,val,...) \
	jsonAdd_(objJson,key,val, (IS_STRING,##__VA_ARGS__))



/* json 对象 */
typedef struct json {
	struct json * this;
	char * data;
	int len;
	struct json* (*init)(struct json *this, int isArray);
	struct json* (*add)
	(struct json* this, const char*key, const char*value, int isValue);
	int (*free)(struct json **this);
	struct json* (*clean)(struct json* this);
} oJson;

oJson* newJson(void);
oJson* jsonAdd_(oJson *this, const char*key, const char*value, int isValue);
oJson* clean(oJson*this);
int deleteJson(oJson**this);
int jsonFree(oJson*this);

#endif /* JSON2_H_ */
