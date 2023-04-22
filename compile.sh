#!/bin/bash

gcc $(pkg-config --cflags libadwaita-1) -o app app.c $(pkg-config --libs libadwaita-1) -export-dynamic