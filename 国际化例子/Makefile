#大致流程
#   po目录
PO=po
LANGDIR=languages
#   翻译的语言的代码
LG=zh_CN

########   一. 简单整合命令   #########
all:build
#第一次,初始化翻译项目,建目录->生成待翻译文件->初始化翻译工程
init:makedir pot msginit 
#日常更新翻译文本
update_po:merge 

#进行翻译后,build程序
build:msgfmt copyto compile

clean:
	rm -f gettext_demo
distclean:clean
	rm -rf ${PO} ${LANGDIR}

########   二. 详细步骤   #############
# 1  第一次开发,创建翻译目录
makedir:
	mkdir -p ${PO} ${LANGDIR}/${LG}/LC_MESSAGES/

# 1.0. 从当前所有.c文件中生成 pot文件,放到po目录下
pot:
	xgettext -k_  --package-name gettext_demo \
		--package-version 0.1 \
		*.c  -o ${PO}/gettext_demo.pot

# 1.1. 在po目录下 初始化翻译工程,只需一次
msginit:
	cd ${PO} && msginit

# 1.2. 对*.po文件进行翻译工作

# 1.3. 完成翻译后,格式化po生成编译过的mo文件
msgfmt:
	msgfmt ${PO}/zh_CN.po -o ${PO}/zh_CN.mo

# 1.4. 复制二进制的mo文件到指定的国际化目录 (其实可以和1.3一起完成)
#   目录由c文件中bindtextdomain等函数指定
copyto:
	cp ${PO}/zh_CN.mo ${LANGDIR}/zh_CN/LC_MESSAGES/gettext_demo.mo

# 1.5. 编译:按照正常程序编译
compile:
	gcc gettext_demo.c -o gettext_demo

# 1.6. 测试
# 英文环境: LC_ALL=en_US ./gettext_demo
test_en:
	LC_ALL=en_US ./gettext_demo
# 中文环境: ./gettext_demo
test_cn:
	LC_ALL=zh_CN.UTF-8 ./gettext_demo

# 2. 平时增量开发步骤
# 2.0 对c文件进行了修改,编译等日常开发工作
# 2.1. 先生成pot,再对其进行合并(merge)
merge:pot
	cd ${PO} && \
	msgmerge ${LG}.po gettext_demo.pot -o ${LG}.po

# 2.2. 翻译/修改zh_CN.po文件

# 2.3. 编译/格式化zh_CN.po文件到mo文件. 即 msgfmt


init:makedir pot msginit 
