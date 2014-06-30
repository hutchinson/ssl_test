#!/usr/bin/env bash

rm -f ssl_client
clang++ client.cpp -Wall -Werror -Wno-deprecated-declarations -o ssl_client -lssl -lcrypto
