# Code Coverage Listener
This application is designed to capture code coverage results from a Parasoft instrumented C or C++ library/binary executing on a different machine or embedded system. 

When the Parasoft instrumented library/binary is executed remotely, it will attempt to phone home and report the coverage data. In order for this to work, an application (such as this one) must be listening for the connection on the proper port and then write the coverage data to a file locally.

This is necessary when a remote machine/embedded system doesn't have a file system or the file system is difficult to access. 

Two things are necessary to make this work:
1) The Parasoft runtime library, which defines functions to gather/report code coverage, must be built with the correct information:
    - To use socket based communication
    - The IP/Hostname & port to send the coverage data to
2) An application listening on the correct IP/Port must be ready to receive data and have the capability to write the received data to a log file.


# Building the Code Coverage Listener
```
make all clean
```

# Basic Usage
```
./socket_listener -port 3003 -clog /home/user/cpptest.clog
```
### Flags
`-port`: specifies the port to listen on. If not specified, defaults to 3003. Make sure the runtime library is built accordingly and the port matches.

`-clog`: specifies the location and name of the coverage log file to write to when coverage results are received from the instrumented binary. If not provided, `cpptest.clog` will be generated in the folder where the listener is executed.

`-append`: the log file will be appended to if this flag is present. If not specified, the log file will deleted and recreated on each run.

# Building Runtime Library
In the `runtime` folder found in the installation directory of C/C++test:
```
make CHANNEL_FILE=channel/unix-socket.mk TARGET_CFG=gcc-static.mk EXTRA_CFLAGS='-DCPPTEST_LOG_SOCKET_HOST=\"<host>\" -DCPPTEST_LOG_SOCKET_PORT=<port>'
```

Where `TARGET_CFG` is the make file for your toolchain and can be found in the `targets` subfolder.

The output of running make will be `build\cpptest.o` and this must be linked into your application which has been instrumented by our cmake/bazel integration or `cpptestcc` directly.

`<host>` and will be the hostname or IP address where this listener is executed. The instrumented library will need to know this in order to connect to the listener. The same goes for `<port>`. By default, the listener will bind to port 3003. Use 3003 for `<port>` if you don't want to specify a different port for the listener.


