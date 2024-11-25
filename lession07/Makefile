#定义变量
#add.c mult.c main.c 
src=$(wildcard ./*.c)
objs=$(patsubst %.c,%.o,$(src))
target=app
$(target):$(objs)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) $< -c -o $@
.PHONY:clean
clean:
	rm $(objs) -f
