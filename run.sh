#!/bin/bash

gcc main.c `pkg-config --cflags --libs gtk+-3.0` `pkg-config --cflags --libs cairo` -o cp && ./cp $@
