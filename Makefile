CC = gcc
target = client server
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")

${warning src_${src}}
${warning deps_${deps}}
obj = $(src:%.c=%.o) 

all: ${target}

client: client.o orenda_socket.h
	$(CC) -o client client.c -lpthread
server: server.o orenda_socket.h
	$(CC) -g -o server server.c -lpthread

clean:
	rm -rf $(obj) ${target}