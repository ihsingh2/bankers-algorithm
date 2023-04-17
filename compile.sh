#!/bin/bash

gcc -Wno-format -o app app.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk4` -export-dynamic