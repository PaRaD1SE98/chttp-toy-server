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

main() 函数是程序的入口函数，它是程序的第一个执行的函数。
例如，当你将server.c编译成了server.exe,
然后在命令行中输入 `server.exe` 时，就会从 main() 函数开始执行。
如果你在命令行中输入 `server.exe 1 2 3`，那么 argc 的值就是 4，
argv 的值就是 `{"server.exe", "1", "2", "3"}`。
*/

int main(int argc, char *argv[])
{
    // 初始化 Winsock 库
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa);

    /*
    printf(), puts() 都是用于向屏幕输出字符串的函数。
    printf() 是一个格式化输出函数，可以使用格式化字符串（%s, %d, ...）来输出。
    常用的格式化字符串有：
    %s - 字符串
    %d - 十进制整数
    %x - 十六进制整数
    %f - 浮点数
    %c - 字符
    %p - 指针
    puts() 只能输出普通字符串，虽然不能使用格式化，但是它结尾自带换行符`\n`。
    */

    if (result != 0)
    {
        // 初始化失败，退出程序
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    else
    {
        // 初始化成功，继续执行
        puts("WSAStartup success");
    }
    // 创建套接字
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    puts("Socket created");

    // 将套接字绑定到本地地址
    int port = 8080;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;                // IPv4
    server_addr.sin_port = htons(port);              // 监听端口 8080
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听本地所有地址

    /*
    htons 和 htonl 是两种不同的函数，用于将主机字节序转换为网络字节序。
    htons 函数用于将 16 位的主机字节序转换为网络字节序。
    htonl 函数用于将 32 位的主机字节序转换为网络字节序。

    主机字节序和网络字节序是两种不同的字节序，用于表示数字在存储器中的顺序。
    字节序是指字节在内存中的存储顺序。

    主机字节序是指一台计算机所使用的字节序。 例如，如果你的计算机是一台
    Intel 架构的计算机，则它的主机字节序是小端字节序；如果你的计算机是一台
     ARM 架构的计算机，则它的主机字节序是大端字节序。

    网络字节序是指计算机网络中所使用的字节序。 一般来说，计算机网络中所使用
    的字节序是大端字节序，即高位字节存储在内存的低地址，低位字节存储在内存
    的高地址。
    */

    printf("Binding socket to local address in port %d\n", port);

    /*
    (struct sockaddr *)&server_addr 是一个强制类型转换表达式。

    在 C 语言中，可以使用括号和类型名将表达式强制转换为指定的
    类型，例如 (int)x 将表达式 x 强制转换为整型。

    在这里，(struct sockaddr *)&server_addr 将变量 server_addr 的
    地址强制转换为 struct sockaddr * 类型的指针。

    struct sockaddr 结构体是一个通用的地址结构体，可以表示
    各种网络地址（例如 IPv4 地址、IPv6 地址、Unix 地址等）。

    在服务器程序中，变量 server_addr 是一个 struct sockaddr_in
    结构体，该结构体可以表示 IPv4 地址。

    使用强制类型转换表达式将 server_addr 强制转换为 struct sockaddr *
    类型的指针，目的是为了将其传递给一些函数（例如 bind 函数），
    该函数要求接受一个通用的地址结构体指针作为参数。
    */

    bind(listen_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    puts("Bind done");

    // 开始监听
    listen(listen_socket, 5);
    puts("Waiting for incoming connections...");

    while (1)
    {
        // 接受客户端连接
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
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

            printf("Received:\n%s",buffer);
            char *method = strtok(buffer, " ");
            char *request_uri = strtok(NULL, " ");
            char *http_version = strtok(NULL, "\r\n");
            // puts(method);
            // puts(request_uri);
            // puts(http_version);

            /*
            最后，你可以使用 send 函数向客户端发送响应。
            HTTP 响应由一个状态行、一个或多个响应头，以及
            可选的响应体组成。你可以使用字符串函数或者格式
            化字符串函数（例如 sprintf）来构造响应，然后
            使用 send 函数将响应发送给客户端。

            例如，你可以使用 sprintf 函数来构造一个 HTTP 响应，
            并使用 send 函数将其发送给客户端。
            */

            char *server_error_html =
                "<html><body>"
                "This is the 500 Internal Server Error page."
                "</body></html>";
            if (strcmp(method, "GET") == 0)
            {
                // 处理 GET 请求
                if (request_uri == NULL)
                {
                    // 发生错误，发送错误响应
                    char *server_error_content =
                        "HTTP/1.1 500 Internal Server Error\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n"
                        "%s";
                    char response[4096];
                    sprintf(response, server_error_content, strlen(server_error_html), server_error_html);
                    send(client_socket, response, strlen(response), 0);
                    perror("Internal Server Error");
                }
                else
                {
                    // 找到了 URL，将其解析出来
                    char response[4096];
                    char *html;
                    char *res_content;

                    if (strcmp(request_uri, "/") == 0)
                    {
                        html = "<html><body>"
                               "Welcome to the index page!\n"
                               "Go to <a href=\"/about\">about</a> page."
                               "</body></html>";
                        res_content =
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: %d\r\n"
                            "\r\n"
                            "%s";
                        sprintf(response, res_content, strlen(html), html);
                        send(client_socket, response, strlen(response), 0);
                        printf("Response:\n%s",response);
                    }
                    else if (strcmp(request_uri, "/about") == 0)
                    {
                        html = "<html><body>"
                               "This is the about page.\n"
                               "Go to <a href=\"/\">index</a> page."
                               "</body></html>";
                        res_content =
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: %d\r\n"
                            "\r\n"
                            "%s";
                        sprintf(response, res_content, strlen(html), html);
                        send(client_socket, response, strlen(response), 0);
                        printf("Response:\n%s", response);
                    }
                    else if (strcmp(request_uri, "/favicon.ico") == 0)
                    {
                        FILE *picFile = fopen("favicon.png", "rb");
                        // 获取图片大小
                        fseek(picFile, 0, SEEK_END);
                        int picSize = ftell(picFile);
                        rewind(picFile);
                        // 发送响应头
                        const char *header =
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: image/png\r\n"
                            "Content-Length: %d\r\n"
                            "\r\n";
                        char headerBuffer[1024];
                        sprintf_s(headerBuffer, 1024, header, picSize);
                        result = send(client_socket, headerBuffer, strlen(headerBuffer), 0);
                        if (result == SOCKET_ERROR)
                        {
                            // 处理错误
                        }
                        // 发送图片数据
                        const int BUFFER_SIZE = 8192;
                        char buffer[BUFFER_SIZE];
                        int bytesRead;
                        while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, picFile)) > 0)
                        {
                            result = send(client_socket, buffer, bytesRead, 0);
                            if (result == SOCKET_ERROR)
                            {
                                // 处理错误
                            }
                        }
                    }
                    else
                    {
                        html = "<html><body>"
                        "This is the 404 page."
                        "</body></html>";
                        res_content =
                            "HTTP/1.1 404 Not Found\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: %d\r\n"
                            "\r\n"
                            "%s";
                        sprintf(response, res_content, strlen(html), html);
                        send(client_socket, response, strlen(response), 0);
                        printf("Response:\n%s", response);
                    }
                }
            }
            else if (strcmp(method, "POST") == 0)
            {
                // 处理 POST 请求
                if (strcmp(request_uri, "/post") == 0)
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