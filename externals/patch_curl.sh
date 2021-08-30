#!/bin/sh

patch -N --silent -p1 < ../0001_curl_find_nghttp2.patch
patch -N --silent -p1 < ../0002_curl_http2_debug.patch

echo "Patched!"
