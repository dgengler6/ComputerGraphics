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

OBJS = lab1-1 lab1-2 lab1-3

all:
	@$(MAKE) $(OBJS)

% : %.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)$(UNAME)/MicroGlut.$(OSSUF)
	gcc -Wall -o $* -I$(commondir) -I../common/$(UNAME) -DGL_GLEXT_PROTOTYPES $*.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)$(UNAME)/MicroGlut.$(OSSUF) $(OSFLAGS)

clean :
	rm $(OBJS)
	@echo So clean !
