####  makefile 例子:多目标,编译方式统一的  ##########
EXES= foo bar baz
$(EXES):
	cc -o $@  $@.c  #其中 $@指代一个目标


###################################### 

## 简单通用例子, ##
.PHONY: all install clean distclean

all:
	$(CC) 1.c -o a.out
install:
	cp a.out $(PREFIX)/bin/
clean:
	rm *.o
distclean:clean
	rm -rf a.out example