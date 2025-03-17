#!/usr/bin/env bash

emcc conway.c -o out/script.js -s ASYNCIFY=1
