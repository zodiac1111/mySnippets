/**
 * @file clog.h c语言实现的日志模块.
 * CLOG_LEVEL: 设定日志等级,参考printk
 * DBG/INFO/NOTICE/WARN/ERROR
 *
 */

#ifndef _CLOG_H_
#define _CLOG_H_
#include <stdio.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <time.h>
/**
 * 调试等级[0-7],越小严重,越大越详细.参考kprint日志等级.
 * 3 - 打印错误
 * 4 - 打印错误,警告
 * 5 - 打印错误,警告,提示
 * 6 - 打印错误,警告,提示,信息
 * 7 - 打印错误,警告,提示,信息,调试信息
 * 打印 [时刻]==线程号== 函数名 (文件:行) 错误信息[错误号] [自定义消息]
 * 其中线程号是LWP的号:
 * http://stackoverflow.com/questions/1759794/how-to-print-pthread-t
 * 如果没有错误则没有错误信息.
 */
#define CLOG_LEVEL_DEBUG 7
#define CLOG_LEVEL_INFO 6
#define CLOG_LEVEL_NOTICE 5
#define CLOG_LEVEL_WARN 4
#define CLOG_LEVEL_ERROR 3
#ifndef  CLOG_LEVEL
#  define CLOG_LEVEL CLOG_LEVEL_DEBUG /// 默认的日志等级.可以被覆盖
#endif

extern char clog_str_time[];
extern char * clog_time(void);
extern FILE* clog_fd;
extern int clog_start(void);
extern int clog_end(void);
/**
 * 可选的错误数据目的,可输出到文件或者屏幕(默认)
 * 如果需要输出至文件,只要在需要使用的文件的#include "clog.h" 语句**前面**定义
 * #define CLOG_PRINT_TO_FILE 1
 * 来覆盖输出目的.
 */
#ifndef CLOG_PRINT_TO_FILE
#  define CLOG_PRINT_TO_FILE 0
#endif
#if CLOG_PRINT_TO_FILE == 0
#  define DC_LOG_FD stderr ///<日志输出文件描述符,简单的输出到标准错误输出
#  define RED "\033[31m"
#  define GREEN "\033[32m"
#  define YELLOW "\033[33m"
#  define BLUE "\033[34m"
#  define PURPLE "\033[35m"
#  define _COLOR "\033[0m" /*一次性关闭所有颜色和特效 类似xml的标签闭合.不需嵌套*/
#  define CLOG_TIME clog_time()
#else
#  define DC_LOG_FD clog_fd ///<也可以根据要求打印到文件.(必要的时候)
#  define RED ""
#  define GREEN ""
#  define YELLOW ""
#  define BLUE ""
#  define PURPLE ""
#  define _COLOR "" /*一次性关闭所有颜色和特效 类似xml的标签闭合.不需嵌套*/
#  define CLOG_TIME  clog_time()
#endif
///日志输出头.线程号(任务号) + [时间]
#define CLOG_PREFIX "%s==%lu== "
#if CLOG_LEVEL >= 7
#  define DP (fprintf(DC_LOG_FD,\
		CLOG_PREFIX PURPLE"%s (%s:%d):%s" _COLOR "\n",\
		CLOG_TIME,syscall(SYS_gettid),\
		__FUNCTION__,__FILE__, __LINE__ ,\
		strerror(errno)))
#  define DBG(fmt, ...) \
		fprintf(DC_LOG_FD,\
		CLOG_PREFIX PURPLE"%s (%s:%d) "_COLOR fmt "\n",\
		CLOG_TIME,syscall(SYS_gettid),__FUNCTION__,__FILE__, __LINE__ , \
		##__VA_ARGS__ )
#else
#  define DP
#  define DBG(fmt, ...)
#endif

#if CLOG_LEVEL >= 6
#  define INFO(fmt, ...)  \
		fprintf(DC_LOG_FD,\
		CLOG_PREFIX GREEN"%s (%s:%d) "_COLOR fmt "\n",\
		CLOG_TIME,syscall(SYS_gettid),__FUNCTION__,__FILE__, __LINE__ , \
		##__VA_ARGS__ )
#else
#  define INFO(fmt, ...)
#endif

#if CLOG_LEVEL >= 5
#  define NOTICE(fmt, ...) fprintf(DC_LOG_FD,\
		CLOG_PREFIX PURPLE"%s (%s:%d) "_COLOR fmt "\n",\
		CLOG_TIME,syscall(SYS_gettid),__FUNCTION__,__FILE__, __LINE__ , \
		##__VA_ARGS__ )
#else
#  define NOTICE(fmt, ...)
#endif

#if CLOG_LEVEL >= 4
#define WARN(fmt, ...) \
		fprintf(DC_LOG_FD,\
		CLOG_PREFIX YELLOW"%s (%s:%d) "_COLOR fmt "\n",\
		CLOG_TIME,syscall(SYS_gettid),__FUNCTION__,__FILE__, __LINE__ ,\
		##__VA_ARGS__ )
#else
#  define WARN(fmt, ...)
#endif

#if CLOG_LEVEL >= 3
#  define ERROR(fmt, ...) \
		fprintf(DC_LOG_FD,\
		CLOG_PREFIX RED"%s (%s:%d):%s[%d] "_COLOR fmt "\n",\
		CLOG_TIME,syscall(SYS_gettid),__FUNCTION__,__FILE__, __LINE__ ,\
		strerror(errno) ,errno, ##__VA_ARGS__ )
#else
#  define ERROR(fmt, ...)
#endif

#endif /* _CLOG_H_ */
