# set this variable to the director in which you saved the common files
commondir = ../common/
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
OSFLAGS = -lXt -lX11 -lGL -lm
OSSUF =c
else
OSFLAGS = -framework Cocoa -framework OpenGL -lm
OSSUF =m
UNAME=Mac
endif


all : lab1-1


lab1-1 : lab1-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)$(UNAME)/MicroGlut.$(OSSUF)
	gcc -Wall -o lab1-1 -I$(commondir) -I../common/$(UNAME) -DGL_GLEXT_PROTOTYPES lab1-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)$(UNAME)/MicroGlut.$(OSSUF) $(OSFLAGS)


clean :
	rm lab1-1
