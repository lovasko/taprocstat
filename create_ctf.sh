#!/bin/sh

ctfconvert -L tpslabel obj/main.o
ctfconvert -L tpslabel obj/getter/svuid.o
ctfconvert -L tpslabel obj/getter/numthreads.o
ctfconvert -L tpslabel obj/getter/command.o
ctfconvert -L tpslabel obj/getter/osrel.o
ctfconvert -L tpslabel obj/getter/pid.o
ctfconvert -L tpslabel obj/getter/ruid.o
ctfconvert -L tpslabel obj/getter/ppid.o
ctfconvert -L tpslabel obj/getter/username.o
ctfconvert -L tpslabel obj/getter/uid.o
ctfconvert -L tpslabel obj/mode/default.o
ctfconvert -L tpslabel obj/mode/binary.o
ctfconvert -L tpslabel obj/mode/security.o

ctfmerge -L tpslabel -o bin/taprocstat obj/main.o obj/getter/*.o obj/mode/*.o
