# HavlongSockets
C++ library for socket client-server interaction

## Visual C++
This library works on **Windows Sockets API**. You can use it in any Visual C++ project.

To build a socket server with **HavlongSockets**, add to your project files **HavlongSServer.cpp** and **HavlongSServer.h**.
Then you will need to implement method `handle()`, that will be called after receiving the response.
Remember to save data you want to send to the client in the `data` variable;
You can implement this method using for example *inheritance*.
After all, use public `start()` method in your main program.

To build a socket client with **HavlongSockets**, add to your project files **HavlongSClient.cpp** and **HavlongSClient.h**.
There you need only to implement method `handleData()`, that will give you info from the server in the `data` variable.

####You can check out the example of:
- Server built with this library in files CheckerServerExample.cpp and CheckerServerImpl.cpp.

- Client built with this library in files CheckerClientExample.cpp and CheckerClientImpl.cpp.
