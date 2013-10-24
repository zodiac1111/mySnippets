# 演示如何生成patch和使用patch

* A: main-old 原始的文件样子
* B: main-changed 修改后的文件样子
* P: patch.patch B相较与A修改的部分
* A1: main-old-need-to-patch : 需要打补丁的文件样子(与main-old一模一样)

简单关系
```
B-A=P
A+P=B
A1=A
A1+P=B
```

# make a patch (diff)

比较 main-changed和main-old得到到底修改了什么东西

diff <old-dir> <changed-dir> -uNr > <patch-file>

diff main-old main-changed  -uNr  > patch.patch

将得到一个patch文件记录了修改的内容

# use a patch (patch)

## 模拟一份未打补丁的文件系统

使用原始的文件和一个(份)patch文件,通过打补丁得到main-changed

* main-old-need-to-patch: 一份未修改的文件系统
* patch.patch 补丁文件

## 打补丁

cd <欲打补丁的目录>
cd main-old-need-to-patch

patch -p<忽略的目录级数> -E(删除空文件) < <补丁文件>
patch -p1 -E < ../patch.patch

## 检查

现在 main-old-need-to-patch 就与 main-changed 文件一模一样了
