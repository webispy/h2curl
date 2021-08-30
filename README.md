# HTTP/2 POST test using curl library

Test environment
* Architecture: x86_64
* OS: Ubuntu 20.04

Test scenario

    client -> server: HTTP/2 GET /longPolling  (Stream-1)
    server -> client: Header response
    server -> client: Dummy data
    server -> client: (Stream hold for 10 seconds)
    ...
    after 7 seconds
    ...
    client -> server: HTTP/2 POST /post  (Stream-3)
    server -> client: Header response
    server -> client: (Stream hold for 5 seconds)
    client -> server: Send post data (100 bytes)
    client -> server: Send post data (100 bytes)
    ...

Simple test server code (node.js, tested with v14.17.5 LTS version)
* [index.js](server/index.js)

HTTP/2 POST test client code
* [h2post.c](src/h2post.c)

## Prepare

Clone this repository and update submodules. (nghttp2, curl)

    $ git clone https://github.com/webispy/h2curl.git
    $ cd h2curl
    h2curl$ git submodule update --init

## Curl library version

### abnormal test case - [c4e6968](https://github.com/curl/curl/commit/c4e6968127e876b01e5e0b4b7cdbc49d5267530c)

    cd externals/curl
    curl$ git reset --hard
    curl$ git clean -df
    curl$ git checkout c4e6968

### normal test case - [842f73d](https://github.com/curl/curl/commit/842f73de58f38bd6e285e08bbd1adb6c17cb62cd)

    cd externals/curl
    curl$ git reset --hard
    curl$ git clean -df
    curl$ git checkout 842f73d

## Run sample HTTP/2 server

In the `server` directory, there is a sample server program written in node.js for testing.

    $ cd server
    server$ npm install
    server$ npm start

## Build and run sample POST example

The test program used the [http2-upload.c](https://curl.se/libcurl/c/http2-upload.html) file provided in the [libcurl example](https://curl.se/libcurl/c/example.html) with some modifications.

Executing the commands below builds the nghttp2 library and curl library and then builds the example application.

    $ mkdir build && cd build
    build$ cmake ..
    build$ make

And in order to access the data file to be transmitted when executing the example, you need to copy the `dummy.dat` file to the directory to be executed.

    build$ cp ../dummy.dat .
    build$ ./src/h2post

## Test result

### abnormal test case

* [server log](log/fail_server.log)
* [client log](log/fail_client.log) - 21 MB !!!

### normal test case

* [server log](log/ok_server.log)
* [client log](log/ok_client.log)

