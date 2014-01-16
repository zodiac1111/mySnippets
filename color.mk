# 参考:
# * http://blog.csdn.net/cherylnatsu/article/details/5878913
# * http://www.termsys.demon.co.uk/vtansi.htm
# 颜色定义
export GREEN=\033[32m
export RED=\033[31m
# 效果定义
export BRIGHT=\033[1m
export Underscore=\033[4m
export REVERSE=\033[7m
# 结束颜色/效果
export _COLOR=\033[0m
# 特别的需要高亮的指令
export CMD_HIGHLIGHT = $(RED)$(Underscore)$(BRIGHT)
# 压缩包
export DIST_PACKAGE_COLOR = $(GREEN)$(BRIGHT)