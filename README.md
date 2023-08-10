File Transfer Server and Client
===============================

The project is cmake and make based and has been successfully built and simple testing has been done on 
an Ubuntu 22.04 system. It has minimal dependencies:
1. libssl & libcrypto for SHA-256 computation on a file to know when it has changed
2. pthread for server component to allow handling parallel client connections

Building the Software
---------------------

1. launch a terminal and `cd` to the folder containing all these files
2. run command `mkdir build && cd build`
3. run command `cmake ..`
4. run command `make`
5. This will result in 2 binaries `ft_server` and `ft_client` in the build folder
6. Both these binaries are written to take the folder they are located in as the folder from where files are served.
   If required this can be changed by editing `common.h` file SERVER_FOLDER and CLIENT_FOLDER defines.
7. Alternatively, these binaries can be moved to separate folders

Running the server
------------------
Run command `./ft_server` on a terminal to launch server. It listens on port 4340. This can be changed by editing
FT_PORT define in common.h and rebuilding the binary. Similarly max parallel connections can be changed by editing
FT_MAX_CLIENTS define in the same file.

Running the client
------------------
Use the following instructions to run a client:
```
./ft_client -l | -r remote_file
  -l: List available files on the server
  -r remote_file: Get file remote_file from server, only if not locally
                  available or is different from server copy.

  The ops you can perform with the client are mutually exclusive:
    * either one can list files available on server
    * or one can retrieve a file from server
``````

TODO
----
* support optimized file transfer - there is a file chunking and chunk hash computation functions already available. It needs a couple more days of work
* thourough testing
* cleanup and ensuring proper handling of dynamically allocated memory
