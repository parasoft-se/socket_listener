# socket_listener
Code Coverage Listener (C)

# Basic Usage
```
./socket_listener -port 3003 -clog /home/user/cpptest.clog -overwrite
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


