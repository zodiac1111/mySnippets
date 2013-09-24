#include <stdio.h>
#include "staticlib.h"
int main(void)
{
	printf("静态库链接示例: %d\n",foo(41));
	return 0;
}
