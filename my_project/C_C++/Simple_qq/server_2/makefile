.SUFFIXES: .c .o
CFLAGS=-g
CC=gcc

SERVER_SRCS=socket_api.c \
			seqlist.c\
			mysqlite.c\
			msgqueue.c\
			semaphore.c\
			mysignal.c\
			work.c\
			server_main.c

SERVER_OBJS=$(SERVER_SRCS:.c=.o)

SERVER_EXEC=server

all: $(SERVER_OBJS)
	$(CC) -o $(SERVER_EXEC) $(SERVER_OBJS) -lsqlite3 -lpthread
	@echo '-------------ok--------------'

.c.o:
	$(CC) -Wall -g -o $@ -c $<

clean:
	rm -f $(SERVER_OBJS)
	rm -f core*
