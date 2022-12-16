#include <stdio.h>
#include <winsock2.h>
/*
下面是一个使用 C 语言在 Windows 端编写的最简单的 HTTP 服务器的例子。
这个例子使用了 Windows 的套接字库（Winsock）来创建和监听套接字，并使用
HTTP 协议的格式来处理来自客户端的请求和发送响应。

套接字（socket）是一种通信机制，用于在计算机网络中的两台计算机之间发送和
接收数据。

套接字可以通过网络协议（例如 TCP 或者 UDP）进行通信，并使用特定的地址
（例如 IP 地址和端口号）来标识发送和接收数据的计算机。

套接字通常被用于网络编程，例如创建网络服务器或客户端，或者在两台计算机之
间进行点对点通信。

套接字可以使用许多不同的编程语言来实现，例如 C、C++、Java 等。在 C 语
言中，Windows环境下你可以使用 Winsock 库来创建和使用套接字。
*/

/*
在 C 语言中，int argc, char *argv[] 是一种常用的参数声明方式。
它用于声明函数的两个参数：argc 和 argv。

argc（argument count）是一个整型变量，表示命令行参数的个数。
它的值等于命令行参数的个数加 1，因为程序名本身也算作一个参数。
argv（argument values）是一个字符串数组，用于存储命令行参数。
它的第 0 个元素（即 argv[0]）是程序名本身，其余元素存储命令行参数。
*/

int main(int argc, char *argv[])
{
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (result != 0)
    {
        // 初始化失败，退出程序
        return 1;
    }
    // 继续执行
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    puts("Socket created");

    // 将套接字绑定到本地地址
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);              // 监听端口 8080
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听本地所有地址
    bind(listen_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    puts("bind done");

    // 开始监听
    listen(listen_socket, 5);
    puts("Waiting for incoming connections...");

    while (1)
    {
        // 接受客户端连接
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        /*
        (struct sockaddr *)&server 是一个强制类型转换表达式。

        在 C 语言中，可以使用括号和类型名将表达式强制转换为指定的
        类型，例如 (int)x 将表达式 x 强制转换为整型。

        在这里，(struct sockaddr *)&server 将变量 server 的
        地址强制转换为 struct sockaddr * 类型的指针。

        struct sockaddr 结构体是一个通用的地址结构体，可以表示
        各种网络地址（例如 IPv4 地址、IPv6 地址、Unix 地址等）。

        在服务器程序中，变量 server 是一个 struct sockaddr_in
         结构体，该结构体可以表示 IPv4 地址。

        使用强制类型转换表达式将 server 强制转换为 struct sockaddr *
        类型的指针，目的是为了将其传递给一些函数（例如 bind 函数），
        该函数要求接受一个通用的地址结构体指针作为参数。
        */
        SOCKET client_socket = accept(listen_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        char buffer[4096];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0)
        {
            // 处理来自客户端的请求

            /*
            接下来，你可以根据 HTTP 协议的格式来解析来自客户端的请求。
            HTTP 请求由一个请求行、一个或多个请求头，以及可选的请求体组成。
            你可以使用字符串函数或者正则表达式来解析请求，并根据请求内容来
            决定如何处理请求。

            例如，你可以使用 `strtok` 函数来解析请求行，并根据请求方法
            （例如 GET 或 POST）和请求的资源路径来决定如何处理请求。
            */

            puts(buffer);
            char *method = strtok(buffer, " ");
            char *request_uri = strtok(NULL, " ");
            char *http_version = strtok(NULL, "\r\n");
            puts(method);
            puts(request_uri);
            puts(http_version);

            /*
            最后，你可以使用 send 函数向客户端发送响应。
            HTTP 响应由一个状态行、一个或多个响应头，以及
            可选的响应体组成。你可以使用字符串函数或者格式
            化字符串函数（例如 sprintf）来构造响应，然后
            使用 send 函数将响应发送给客户端。

            例如，你可以使用 sprintf 函数来构造一个 HTTP 响应，
            并使用 send 函数将其发送给客户端。
            */

            char *server_error_html = "<html><body>This is the 404 page.</body></html>";
            if (strcmp(method, "GET") == 0)
            {
                // 处理 GET 请求
                if (request_uri == NULL)
                {
                    // 未找到 URL，发送错误响应
                    char response[4096];

                    sprintf(response, "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(server_error_html), server_error_html);
                    send(client_socket, response, strlen(response), 0);
                    perror("url is null");
                }
                else
                {
                    // 找到了 URL，将其解析出来
                    char response[4096];
                    char *html;

                    if (strcmp(request_uri, "/") == 0)
                    {
                        html = "<html><body>Welcome to the index page!</body></html>";
                    }
                    else if (strcmp(request_uri, "/about") == 0)
                    {
                        html = "<html><body>This is the about page.</body></html>";
                    }
                    else
                    {
                        html = "<html><body>This is the 404 page.</body></html>";
                    }
                    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(html), html);
                    send(client_socket, response, strlen(response), 0);
                    puts(response);
                }
            }
            else if (strcmp(method, "POST") == 0)
            {
                // 处理 POST 请求
                if (strcmp(request_uri,"/post")==0)
                {
                    char *json_response = "{\"success\":\"post request\"}";
                    char response[4096];

                    sprintf(response, "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(json_response), json_response);
                    send(client_socket, response, strlen(response), 0);
                }
                else
                {
                    char *json_response = "{\"error\":\"invalid request\"}";
                    char response[4096];

                    sprintf(response, "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(json_response), json_response);
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
        closesocket(client_socket);
    }

    /*
    最后，在程序结束时，你需要关闭套接字并释放 Winsock 库。
    */

    closesocket(listen_socket);
    WSACleanup();
    return 0;
}