#################################################################
#
#				Warning!!!!
#				============
#	
# this make file compiles all the sources with implicit rules
# therefore in order to make sure all the changes were made 
# run it with "make clean all"
#
##################################################################

EXEC = ex1

LOGIN1 = eranws
ID1 = 300489960

LOGIN2 = ilansh
ID2 = 302514401


# list of all the sources files
SRCS	=  	ex1.cpp		\
		Model.cpp 	\
		ShaderIO.cpp	\
		Ball.cpp \

# list of all the headers files
HDRS	=  	ex1.h		\
		Model.h 	\
		ShaderIO.h	\
		Ball.h

# list EXTRAS (shaders, etc.)
EXTRAS =	shaders/CheckeredShader.frag	\
		shaders/TranslateShader.vert	\
		Readme.txt			\
		Makefile			\

# compiler 
CC	= g++

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated

else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O2
endif


# linker
LD	= $(CC)
LDFLAGS	=


# the name of the lib to create 
APP = ex1

# each lib should add with -l flag
LIBS =  -L/usr/lib -lm -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib


# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}

# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

##################################
# 	default target 		##
##################################
all : $(APP)

$(APP) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(APP)

$(OBJS) : $(SRCS) $(HDRS)

run: $(APP)
	$(APP)

zip:
	zip $(EXEC)_$(ID1)_$(LOGIN1)_$(ID2)_$(LOGIN2).zip $(SRCS) $(HDRS) $(EXTRAS)

tar:
	tar cvf $(EXEC).tar Makefile $(SRCS) $(HDRS) README

.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~ $(APP)
