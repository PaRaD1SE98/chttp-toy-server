## 编译程序
server: server.c
	gcc -g server.c -o server.exe -lws2_32

## 清除编译好的程序
clean:
	del server.exe

## 运行程序
run: server
	server.exe