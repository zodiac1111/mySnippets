/**
 * @file clog.c
 *
 */
#include <stdlib.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <time.h>
#include "clog.h"
/// 日志文件描述符
FILE* clog_fd;
char clog_tmpbuf[128];
int clog_start(void)
{
#if CLOG_PRINT_TO_FILE == 1
	char filename[128];
	char app_name[256] = { 0 };
	//需要proc文件系统支持
	int count = readlink("/proc/self/exe", app_name, 256);
	if (count<0||count>256) {
		perror("readlink");
	}
	sprintf(filename, "log-%s.%ld", app_name, syscall(SYS_gettid));
	clog_fd = fopen(filename, "a");
	if (clog_fd==NULL ) {
		perror("open log file");
	}
	setvbuf(clog_fd, (char *) NULL, _IONBF, 0);
	printf("logfilename=%s\n", filename);
#else
#  warning "不输出到文件.clog_start 将被忽略"
#endif
	return 0;
}
int clog_end(void)
{
#if CLOG_PRINT_TO_FILE == 1
	fclose(clog_fd);
#else
#  warning "不输出到文件.clog_end 将被忽略"
#endif
	return 0;
}
char clog_str_time[32] = "yyyy-MM-dd HH:mm:ss";
/**
 * 使用(文件内)全局变量构造当前时刻字符串
 * @retval 上面格式的时间字符串
 */
char * clog_time(void)
{
	time_t t;
	struct tm tm;
	t = time(NULL );
	localtime_r(&t, &tm);
	strftime(clog_str_time, 32, "[%F %T]", &tm);
//	sprintf(clog_str_time, "%04d-%02d-%02d %02d:%02d:%02d",
//	        tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
//	        tm.tm_hour, tm.tm_min, tm.tm_sec);
	return clog_str_time;
}
