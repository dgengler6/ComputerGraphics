# set this variable to the director in which you saved the common files
commondir = ../common/
CC=gcc
CFLAGS= -Wall -DGL_GLEXT_PROTOTYPES
DEPDIR=dependencies
OBJDIR=objects
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
OSFLAGS = -lXt -lX11 -lGL -lm
OSSUF =c
else
OSFLAGS = -framework Cocoa -framework OpenGL -lm
OSSUF =m
UNAME=Mac
endif

OBJS = lab4-1 lab4-2 lab4-3 lab4-4 lab4-5

all:
	@$(MAKE) $(OBJS)


init:
	@if ! test -f "$(OBJDIR)"; then \
		mkdir -p $(OBJDIR); \
	fi; \
	if ! test -f "$(DEPDIR)"; then \
		mkdir -p $(DEPDIR); \
	fi; \

%.o: %.c init
	$(CC) -c -o $(OBJDIR)/$@ $< $(CFLAGS)

%.d: %.c init
	@$(CC) -MM $< > $@.$$$$; \
	sed -i -e 's|$*\.o: ||g; s|\.c|\.o|g; s|\.h|\.d|g; s|$*\.d||g;' $@.$$$$; \
	cp $@.$$$$ $*.$$$$; \
	echo "$@" > $@.$$$$$$; \
	while grep -q "\.d" $*.$$$$; \
	do \
	  echo "" > $*.$$$$; \
		echo "DEBUT"; \
		while grep -q "\.d" $@.$$$$; do \
			file=$$(echo $$(cat $@.$$$$) | head -n1 | cut -d " " -f1 ); \
			echo $$(cat $@.$$$$); \
			echo "IF"; \
			if $$(echo $$file | grep -q "\.d") ; then \
				f=$$(echo $$file | sed -e 's/\.d//g'); \
				fc=$$(echo $$file  | sed -e 's/\.d/\.c/g'); \
				fil=$$(grep -oh -m 1 "\w*\.d" $@.$$$$ | head -1); \
				if test -f "$(DEPDIR)/$$fil"; then \
			 		sed -i -e "s|$$f\.d|$$(sed 's:|:\\|:g' $(DEPDIR)/$$fil)/" $@.$$$$; \
		  	else \
					echo $$file >> $@.$$$$$$; \
					$(CC) -MM $$fc | cut -f 2- -d ' ' > $$file.$$$$; \
					echo "3.5"; \
					echo $$(cat $$file.$$$$); \
					sed -i -e "s|\.c|\.o|g; s|\.h|\.d|g; s|$$f\.d||g;" $$file.$$$$; \
					echo "4"; \
					echo $$(cat $$file.$$$$); \
					echo $$(cat $$file.$$$$) >> $*.$$$$; \
					echo "5"; \
					sed -i -e "s|$$f\.d||" $@.$$$$; \
					echo "6"; \
					rm -f $$file.$$$$; \
					echo "7"; \
				fi; \
			else \
				echo "ELSE"; \
				echo $$file >> $*.$$$$; \
				sed -i -e "s|$$file||" $@.$$$$; \
				echo "END ELSE"; \
			fi; \
	  done; \
		echo "FIN"; \
		echo $$(cat $@.$$$$) >> $*.$$$$; \
		echo $$(awk '{for (i=1;i<=NF;i++) if (!a[$$i]++) printf("%s%s",$$i,FS)}{printf("\n")}' $*.$$$$) > $*.$$$$; \
		cp $@.$$$$$$ $*.$$$$$$; \
		while grep -q "\.d" $*.$$$$$$; do \
			f=$$(grep -oh -m 1 "\w*\.d" $*.$$$$$$ | head -1 | sed 's|\.d||g'); \
			sed -i "s|$$f\.d||" $*.$$$$; \
			sed -i "s|$$f\.d||" $*.$$$$$$ ; \
		done; \
		rm -f $*.$$$$$$; \
		cp $*.$$$$ $@.$$$$; \
	done; \
	cp $*.$$$$ $(DEPDIR)/$@; \
	echo $$(cat $*.$$$$); \
	rm -f $@.$$$$ $*.$$$$ $@.$$$$$$;

%.build: init
	@$(MAKE) $*.d; \
	cp $(DEPDIR)/$*.d $*.$$$$; \
	while grep -q "\.o" $*.$$$$; \
	do \
			object=$$(grep -oh -m 1 "\w*\.o" $*.$$$$ | head -1); \
			sed -i -e "s/$$object//" $*.$$$$; \
			$(MAKE) $$object; \
	done; \
	sed 's/\w*.o/$(OBJDIR)\/&/g' $(DEPDIR)/$*.d > $*.$$$$ ; \
	deps=`cat $*.$$$$`; \
	echo $$deps; \
	rm -f $*.$$$$; \
	$(CC) -o $* $$deps $(CFLAGS) $(OSFLAGS);

%.run: %.build
	@./$*

clean:
		@rm -r $(DEPDIR); \
		rm -r $(OBJDIR); \
		find -type f \
			-not -iname "*.h" \
			-a -not -iname 'makefile' \
			-a -not -iname "*.c" \
			-a -not -iname "*.frag" \
			-a -not -iname "*.vert" \
			-a -not -iname "*.obj" \
			-a -not -iname "*.tga" \
			-exec rm -f {} +
		@echo cleaned !
