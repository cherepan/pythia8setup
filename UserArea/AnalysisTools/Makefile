#include make.inc
#################################################
### Defining Compiling variables              ###
#################################################

CXX           = g++ -Wno-deprecated -Wall
#-m32  -Wno-deprecated
LD            = g++ -Wno-deprecated -Wall
#-m32  -Wno-deprecated 
INSTALL	      = /usr/bin/install
SHELL = /bin/sh

####################################
###                              ###
####################################

SHAREDLIBFLAGS =
SHAREDCXXFLAGS =
SUBDIRS =
TAUSPINERFLAG = 

override CXXFLAGS += 
ALLCXXFLAGS = $(CXXFLAGS) -I$(ROOTSYS)/include -c -I/include $(SHAREDCXXFLAGS) -c -I./ -c
LDFLAGS       =  
ROOTLIBS      =  -L$(ROOTSYS)/lib -L/usr/lib/ -L/lib/i686/  -lCore -lHist -lGraf  -lGraf3d -lGpad -lTree -lRint  -lPostscript -lMatrix -lPhysics -lGui -LObj -lMinuit2 -lThread -lTreePlayer -lRIO -lMathCore -rdynamic -Wl,--rpath $(ROOTSYS)/lib

# -ldl -lpthread

DEFS  	      = -DSTANDALONE=1
LIBS          = $(ROOTLIBS) $(SHAREDLIBFLAGS)

TARGETS       =	Tools \
		ReadNtuple 

CINTTARGETS = 

# now include conditional options set by flags in make.inc

HDRS          = $(TARGETS:=.h) 		
SRCS          = $(TARGETS:=.cxx)  Run.cxx
OBJS          = $(SRCS:.cxx=.o)

PROGRAM       = Run.exe

$(PROGRAM):     $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	@echo $(LD) $(LDFLAGS) libs/*.o $(LIBS) -o $(PROGRAM)
	@$(LD) $(LDFLAGS) libs/*.o $(LIBS) -o $(PROGRAM)
	@echo "done"

VPATH = utilities:libs


#	@$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o ../$(PROGRAM)

##########################################################
###                                                    ###
##########################################################

$(OBJS): %.o : %.cxx
	$(CXX) $(ALLCXXFLAGS) $(DEFS) $< -o libs/$@ 

.PHONY: clean all install

#install: Run.exe



clean:
	@rm libs/*.o
	@rm Run.exe


all: install


