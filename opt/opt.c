/* 最简单 getopt用法
 * 编译: gcc opt.c
 * 测试: a.out -h
 * 	 a.out -vhsd
 * 	 a.out -p 8000
 * 	 a.out -jS -p 8000 -L abc -46
 **/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char*argv[])
{
	char c; //得到的短选项 如 -d -s 的 d 和 s
	//一次获得所有选项直到返回结束.
	for(;;){
		//获得选项
		c= getopt(argc,argv,"hvsdF46jS" //没有参数的选项
				//有参数的选项,如-p 8000
				"r:R:f:p:n:N:i:t:c:a:u:g:l:L:m:y:b:k:e:x:C:P:~:"
			 );
		if(-1 == c){
			printf("返回结束,c=%02d\n",c);
			break;
		}
		//识别到一个选项则打印出来,一般这里switch开始分流执行.
		//对于有参数的选项如 -p 8000,optarg为char* 指向字符串"8000" 
		printf("识别到选项 -%c (%d) %s \n",c,c,optarg);
	}
	return 0;
}
