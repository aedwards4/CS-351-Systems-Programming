Lab4 - Networking Benchmarks

In order to run the benchmarks for function, pipe, and socket, run this command:
./test-netio.sh

In order to run the rpcgen benchmarks, you must un-comment the commented lines in test-netio.sh, then run exactly as stated above.

The rpcgen is commented out because it does not work correctly on Fourier due to requiring sudo permissions. When running, the user will be prompted for a password in order to enable sudo permissions. The user may be prompted multiple times throughout the benchmark.

Both the sockets and the rpc implementations utilize new terminal windows in order to run either a server or client.