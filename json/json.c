/**
 * @file json2.c
 * File Encoding UTF8
 * json对象生成库.(仅生成)
 */

#include <stdint.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include "json.h"

/// 默认不使能json库使用demo.置1使能demo例程
#ifndef JSON_LIBRARY_DEMO
#  define JSON_LIBRARY_DEMO 0
#endif

// debug print
#define RED "\033[31m"
#define _COLOR "\033[0m"
#define DP 	(fprintf(stderr,RED"*%s (%s:%d):%s\n"_COLOR,\
		__FUNCTION__,__FILE__, __LINE__ ,\
		strerror(errno)))

static oJson* init(oJson *this, int isArray);
void print_json(struct json j);
#if JSON_LIBRARY_DEMO
#warning ****** 使能 Json DEMO程序
/**
 * json Library DEMO
 * 演示json字符串生成库到使用
 */
int main(void)
{
	/* 1. 创建 json对象 */
	oJson *jsonSent= newJson();
	oJson *j3=newJson();
	/* 2. 添加内容 (按照最初添加的内容区分是对象还是数组)key=NULL 则为数组 */
	jsonAdd(j3,"asd","asd");
	/* **错误演示,企图对象中添加数组值,打印错误并忽略插入 */
	jsonAdd(j3,NULL,"asd");
	jsonAdd(jsonSent,"asd",j3->data);
	j3->clean(j3);
	jsonAdd(j3,NULL,"123");
	jsonAdd(j3,NULL,"456",IS_VALUE);
	//printf("j1.data=%s\n",j1.data);
	printf("j3.data=%s\n",j3->data);
	printf("jsonSent.data=%s\n",jsonSent->data);
	deleteJson(&j3);
	deleteJson(&j3); //忽略重复释放
	deleteJson(&jsonSent);
	return 0;
}
#endif
static oJson * init(oJson *this, int isArray)
{
	this->data = malloc(strlen("{}")+1);
	if (this->data!=NULL ) {
		if (isArray) {
			strcpy(this->data, "[]");
		} else {
			strcpy(this->data, "{}");
		}
		this->len += 2;
		this->this = this;
	} else {
		DP;
	}
	return this;
}
/**
 * 添加键/值对或者数组数值到json对象.
 * @param this 本对象
 * @param key 键,如果是数组则不需要键(传入NULL即可)
 * @param value 值
 * @param isValue bool指示是否为数组型布尔型.他们不需要引号.默认非数值型,即需要引号
 * @return
 */
oJson * jsonAdd_(oJson *this, const char*key, const char*value, int isValue)

{
	// 如果是空对象,则按照第一次到类型初始化为数据或者对象
	if (this->len==0||this->data==NULL ) {
		this->init(this, key==NULL );
	}
	if (this->data==NULL ) {
		fprintf(stderr, "jsonAdd-null dobj");
		DP;
		return NULL ;
	}
	int isArray = (((this->data))[0]=='[') ? 1 : 0;/*是否是向数组内添加.*/
	if (key==NULL &&!isArray) { /*添加数组元素不需要name.设置成空.*/
		fprintf(stderr, "jsonAdd-null name");
		DP;
		return NULL ;
	}
	if (value==NULL ) {
		fprintf(stderr, "jsonAdd-null value");
		DP;
		return NULL ;
	}
	int dlen = this->len; /* 追加前对象到长度, 用于计算追加到尾部到位置 */
	int isObj = (value[0]=='{'||value[0]=='['||isValue) ? 1 : 0;
	int isFirst = ((dlen==2) ? 1 : 0); /*原始对象为空则不需要加逗号.*/
	char end;/*区分是像对象还是向数组添加名/值对.*/
	end = ((this->data))[0]+2;
	int add_len = (isFirst ? 0 : 1) /*可能存在的逗号.如果空对象,则新增的名/值对不需要逗号*/
	+(isArray ? 0 : strlen("\"\":"))/* 名/值对的可格式 "名": */
	+(isObj ? 0 : 2) /* 如果不是对象,且不是数值型则要加一对引号 */
	+(isArray ? 0 : strlen(key)) /* 数组不需要name字段. */
	+strlen(value)/* 名/值对的长度 */
	;
	this->len += add_len;
	(this->data) = realloc(this->data, this->len+1);/* \0 */
	if ((this->data)==NULL ) {
		perror("jsonAddValue-realloc");
		DP;
		return (this);
	}
	/**
	 * @note 用三个bit分别表示,若gcc版本过低不能识别0b开头的二进制表达方式.
	 * 换成0x开头的十六进制表示就可以了.这里用二进制表示更形象 XD /
	 * 较低版本的gcc不支持二进制
	 */
	/* int cs=isArray*0b100+isFirst*0b10+isObj*0b1; *//*不使用gcc二进制的扩展 */
	int cs = isArray*0x04+isFirst*0x02+isObj*0x01;
	switch (cs) {
	/* 添加到不是数组的对象中 */
	case 0x00/*0b000*/: /*不是首个元素(需要逗号),不是对象(需要引号).*/
		if (isValue) { /* 如果是数值不需要引号 */
			sprintf((this->data)+dlen-1, ",\"%s\":%s%c", key, value, end);
		} else {
			sprintf((this->data)+dlen-1, ",\"%s\":\"%s\"%c", key, value, end);
		}
		break;
	case 0x01/*0b001*/: /*不是首个元素(需要逗号),是对象(不需要引号)*/
		sprintf((this->data)+dlen-1, ",\"%s\":%s%c", key, value, end);
		break;
	case 0x02/*0b010*/: /*是首个元素(不需要逗号),不是对象(需要引号).*/
		if (isValue) { /* 如果是数值不需要引号 */
			sprintf((this->data)+dlen-1, "\"%s\":%s%c", key, value, end);
		} else {
			sprintf((this->data)+dlen-1, "\"%s\":\"%s\"%c", key, value, end);
		}
		break;
	case 0x03/*0b011*/: /*是首个元素(不需要逗号),是对象(不需要引号).*/
		sprintf((this->data)+dlen-1, "\"%s\":%s%c", key, value, end);
		break;
		/*添加到一个数组对象中,不需要name*/
	case 0x04/*0b100*/: /*不是首个元素(需要逗号),不是对象(需要引号).*/
		if (isValue) { /* 如果是数值不需要引号 */
			sprintf((this->data)+dlen-1, ",%s%c", value, end);
		} else {
			sprintf((this->data)+dlen-1, ",\"%s\"%c", value, end);
		}
		break;
	case 0x05/*0b101*/: /*不是首个元素(需要逗号),是对象(不需要引号)*/
		sprintf((this->data)+dlen-1, ",%s%c", value, end);
		break;
	case 0x06/*0b110*/: /*是首个元素(不需要逗号),不是对象(需要引号).*/
		if (isValue) { /* 如果是数值不需要引号 */
			sprintf((this->data)+dlen-1, "%s%c", value, end);
		} else {
			sprintf((this->data)+dlen-1, "\"%s\"%c", value, end);
		}
		break;
	case 0x07/*0b111*/: /*是首个元素(不需要逗号),是对象(不需要引号).*/
		sprintf((this->data)+dlen-1, "%s%c", value, end);
		break;
	default:
		fprintf(stderr, "in switch case cs=%d\n", cs);
		DP;
		break;
	}
	return this;
}
/* 清空但不释放对象 */
oJson* clean(oJson* this)
{
	this->this = NULL;
	this->len = 0;
	return this;
}
int jsonFree(oJson *this)
{
	if (this->data!=NULL ) {
		free(this->data);
		this->data = NULL;
		this->len = 0;
		return 0;
	}
	return -1;
}
int deleteJson(oJson **this)
{
	if (*this!=NULL ) {
		if ((*this)->data!=NULL ) {
			free((*this)->data);
			(*this)->data = NULL;
			(*this)->len = 0;
			//printf("释放数据指针\n");
		}
		free(*this);
		(*this) = NULL;
		//printf("释放结构体指针\n");
	}
	return -1;
}
void print_json(oJson j)
{
	int i;
	for (i = 0; i<j.len; i++) {
		printf("%c", j.data[i]);
	}
}
#define is_Value(x) 							\
	(__builtin_types_compatible_p (__typeof__ (x), int)) ?1: 	\
	(__builtin_types_compatible_p (__typeof__ (x), float))?1: 	\
	(__builtin_types_compatible_p (__typeof__ (x), long))?1: 	\
	(__builtin_types_compatible_p (__typeof__ (x), double))?1:0
oJson * newJson(void)
{
	oJson *j = (oJson *) malloc(sizeof(oJson));
	j->data = NULL;
	j->len = 0;
	j->init = init;
	j->add = jsonAdd_;
	j->free = deleteJson;
	j->clean = clean;
	return j;
}

