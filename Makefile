target=abook

src=${wildcard ./*.c}                # 查找当前目录下所有的.c文件
object=${patsubst %.c, %.o, ${src}}  # 将.c替换为.o

CC=gcc
CPPFLAGS= -Wall -Wextra -pedantic -std=c99   # 指定编译时的参数

${target}:${object}                 # $@ 目标  $^  全部参数
	${CC} -o $@ $^

%.o: %.c
	${CC} -o $@ -c $^ ${CPPFLAGS}

clean:                # 需要执行 make clean命令，如果删掉.o文件，make命令会重新执行
	rm -f *.o
