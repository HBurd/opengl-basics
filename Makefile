.DEFAULT_GOAL := ogl

LDFLAGS  = -Llib
LDLIBS   = -l:libglfw3.a -l:libglew.a -lGL -ldl -lX11 -lm -lpthread
CPPFLAGS = -g -Iinclude -Wall -Wextra -Wpedantic
