# C 语言实现的 http 玩具服务器

如果你在翻看 C 语言入门教程的时候，看到第一个程序，就是一个打印`Hello World!`的程序。

```C
#include <stdio.h>

int main()
{
    printf("Hello World!");
    return 0;
}
```

你跟着做了一遍，嗯，能运行，但是你发现你什么都没学会。你甚至问不出一些有价值的问题。你依然看不懂别人写的 C 程序，看不懂 C 语言的更多语法。你看不懂指针是什么，看不懂`#include`的`.h`头文件是什么东西，哪里凭空变出来的。这个程序很无聊，没有人会为了打印几个字符而去学 C 语言。你的兴致已经不复存在了，根本不想看接下来的教程。这就像你在听大学教授的讲座，因为太难而跟不上，最后教授问有没有哪里不懂，你却根本不知道该从何问起，索性直接躺了。

如果你跟我有同样的感受，那么你可以试试这个程序。这段代码是我学习 C 语言时通过询问 OPENAI 的 ChatGPT 模型，经过整合得到的一个最简单的可用程序。他是一个 http 玩具服务器，只有不到 300 行的代码，甚至大部分都是注释，却能直接运行。它不够抽象，所以不能在生产环境使用。但通过一些拙略的硬编码，它却构成了完整的服务器雏形，它解释了我们每天都在使用的互联网是如何被计算机处理的。

最重要的是，利用这个程序，你会有目的性的问出很多有意义的问题，这些问题正是一个程序员在解决实际问题中会有的正常思路。你可以基于实际问题，去搜索引擎找到具体的答案。

这个程序囊括了一些常用的 C 语言的基本语法，和编译，链接，运行的基本流程。它可以同时作为 C 语言和网络编程的入门示例。

如果你因为它不够抽象而不满意，那么你可以自己学着去抽象它，把它变成一个更加通用的 http 服务器。这样你就可以在生产环境中使用它了，这是一个令人振奋的学习动力。

### 注意事项

由于 Windows 和 Linux 环境下的 socket 库不一样，本程序使用<winsock2.h>作为套接字库，因此只能在 Windows 环境编译运行。
如果用 Unix 系统,可以使用<sys/socket.h>库,并且不需要 WSAStartup()和 WSACleanup()，其它函数命名可能也会有些许不同，尚未测试。

本程序中虚拟了两个 GET 方法页面`/`和`/about`，和一个 POST 方法页面`/post`，在实际使用中，不可能把各种页面和返回内容硬编码在服务器代码文件中，所以这只是一个简单的示例。

### 1.编译

使用`gcc`编译器编译，需要添加`-lws2_32`参数链接 winsock2 库。

```powershell
gcc -o server.exe server.c -lws2_32
```

### 2.运行

```powershell
./server.exe
```

### 3.测试

#### GET

使用浏览器访问`http://localhost:8080/`和`http://localhost:8080/about`。

或使用`Powershell`的`Invoke-WebRequest`命令。

```powershell
Invoke-WebRequest http://localhost:8080/
Invoke-WebRequest http://localhost:8080/about
```

#### POST

使用`Powershell`的`Invoke-WebRequest`命令访问`http://localhost:8080/post`。

```powershell
Invoke-WebRequest -Method Post -Body 'key1=value1&key2=value2' http://localhost:8080/post
```

### Makefile

编译运行的命令可以写在`Makefile`中，方便使用。可以在命令行直接运行`make`命令编译，或者使用`make run`命令进行编译+运行。

```powershell
make run
```
