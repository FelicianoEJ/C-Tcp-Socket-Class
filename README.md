# C-Tcp-Socket-Class
The project main purpose is to establish a OOP approach into Winsock 2 socket programming for the TCP standard. (Windows Only)

* The repo contains the include header file ``tcpserver.h`` and the class implementation as ``tcpserver_class.cpp``

* Note: this class uses functions from Windows Socket 2 library which can be included in various form in order for this source to compile as a program.
    
    * Visual Studio https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-basic-winsock-application

    * Add a folder to the project named ``lib`` and add the ``Ws2_32.lib`` as static or dinamically link to ``system32/ws2_32.dll``

    * My approach was using mingw ``libws2_32.a`` (Yes I made my project on VS Code) (Check the ``.vscode/task.json`` to see how I linked the lib)

* The project also implements some basic error handling. 

Feel free to FORK and modify the source for more funtion implementations and better error handling and logging.