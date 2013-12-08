# 隐含规则使用的编译器,一般编译时候显式的也使用这个,如下面的$(CC)
CC=gcc

#隐含规则使用的预处理选项
CPPFLAGS= -I..

#隐含规则使用的编译(complie)器选项
CFLAGS=-Wall -g

#连接器选项,不是隐含的,需要显式指出
LDFLAGS= -lm

#终极目标
all:main.o
	$(CC) $^ -o $@ $(LDFLAGS)
# 预处理和编译汇编在一起执行了,不用分步
#main.o:main.i
#main.i:main.c main.h
main.o:main.c main.h

# 执行效果应该是这样的:
# gcc -Wall -g -I..  -c -o main.o main.c  #这里gcc是隐含规则使用了CC变量
# gcc main.o -o all -lm   #这里gcc是$(CC)变量显式的使用
