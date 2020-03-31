# set this variable to the director in which you saved the common files
commondir = ../common/
<<<<<<< HEAD
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
OSFLAGS = -lXt -lX11 -lGL -lm
OSSUF =c
else
OSFLAGS = -framework Cocoa -framework OpenGL -lm
OSSUF =m
UNAME=Mac
endif

OBJS = lab4-1

all:
	@$(MAKE) $(OBJS)

% : %.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)$(UNAME)/MicroGlut.$(OSSUF)
	gcc -Wall -o $* -I$(commondir) -I../common/$(UNAME) -DGL_GLEXT_PROTOTYPES $*.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)$(UNAME)/MicroGlut.$(OSSUF) $(OSFLAGS)

clean :
	rm $(OBJS)
	@echo So clean !
=======

all : lab4-1

lab4-1 : lab4-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-1 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm lab4-1
>>>>>>> 2661600cc0eea197352ee5f9e6518c1a025d3eea
