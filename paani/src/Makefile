CXX ?= "`xcrun --find clang++`"
CC := $(CXX)
LINK ?= $(CXX)

# Find out which version of gcc we're using.  We need at least gcc 3.
GCCVER := $(shell "$(CC)" --version | grep version | sed "s/.*version \([0-9.]*\) .*/\1/")
GCCVER_W_SPACES := $(subst ., ,$(GCCVER))
GCCMAJORVER := $(word 1,$(GCCVER_W_SPACES))
GCCMINORVER := $(word 2,$(GCCVER_W_SPACES))

GCC3 = 1
ifeq ($(GCCMAJORVER), 4)
    GCC4 = 1
endif

# Mac OS X supports only 64-bit.
AMD64 = 1
ARCHDEFS = -arch x86_64 -DAMD64 -fPIC
SIZEOF_VOID_P = 8
SIZEOF_DEFS = -DSIZEOF_VOID_P=$(SIZEOF_VOID_P)

THIRDPARTYDEFS = -DFBX_ENABLED=1 -DOPENCL_ENABLED=1 -DOPENVDB_ENABLED=1

# Warning flags.
OBJCWFLAGS = \
    -Wall -W -Wno-parentheses -Wno-sign-compare \
    -Wno-uninitialized -Wunused -Wno-unused-parameter
CXXWFLAGS = $(OBJCWFLAGS) -Wno-deprecated -Wno-reorder

ifndef OPTIMIZER
    OPTIMIZER = -O2
endif

# Compiler defines.
HDEFINES = \
    -DVERSION=\"14.0.258\" -D_GNU_SOURCE -DMAKING_DSO \
    -DMBSD -DMBSD_COCOA -DMBSD_INTEL \
    $(ARCHDEFS) $(SIZEOF_DEFS) $(THIRDPARTYDEFS) \
    -DSESI_LITTLE_ENDIAN -DENABLE_THREADS -DUSE_PTHREADS -D_REENTRANT \
    -DNEED_SPECIALIZATION_STORAGE -DGCC3
ifdef GCC4
    HDEFINES += -DGCC4
endif

# Compiler flags.
OBJOUTPUT = -o 
OBJCFLAGS = -c $(HDEFINES) $(INCINC) $(OBJCWFLAGS) $(OPTIMIZER) -fobjc-gc \
    -mmacosx-version-min=10.8
OBJFLAGS = -c $(HDEFINES) $(INCINC) $(CXXWFLAGS) $(OPTIMIZER) \
    -mmacosx-version-min=10.8

# For clang >= 5.0, we need to make sure we link to GNU's standard C++ implementation.
IS_CLANG_GTE_5 = $(shell test $(GCCMAJORVER) -ge 5 && echo true)
ifeq ($(IS_CLANG_GTE_5),true)
    STDLIB_LINK_FLAG := -stdlib=libstdc++
endif

# Linker flags.
H_LDFLAGS = -arch x86_64 -fobjc-gc  -Wl,-macosx_version_min,10.8 \
    $(STDLIB_LINK_FLAG)

DEV_DSOLIB = $(HFS)/dsolib
ifeq ($(wildcard $(DEV_DSOLIB)),)
    H_DIR = /Library/Frameworks/Houdini.framework/Versions/14.0.258
    HLIBDIR = $(H_DIR)/Libraries
    H_FRAMEWORK = $(H_DIR)/Houdini

    H_LDINC = -Wl,-rpath,$(HLIBDIR) -L$(HLIBDIR)
    H_LDLIBS = $(H_FRAMEWORK)
else
    H_LDINC = -L$(HFS)/dsolib -F$(HFS)/Frameworks
    H_LDLIBS = -framework Houdini
endif
SHAREDFLAG = -bundle
SYSLIBINC = 
SYSLIBS = -framework OpenGL -framework Cocoa
INCINC = $(INCDIRS) -I$(HFS)/toolkit/include
DSOOUTPUT = -o
DSOFLAGS = \
    $(SYSLIBINC) $(LIBDIRS) $(LIBS) $(SYSLIBS) \
    $(H_LDFLAGS) $(H_LDINC) $(H_LDLIBS)

# Standalone linker flags.
HLIBINC = $(H_LDINC)
HLIBS = $(H_LDLIBS)
SAOUTPUT = -o
SAFLAGS = $(HLIBINC) $(HLIBS) $(DSOFLAGS)

RM = /bin/rm -f

# default: paani
#
# paani: SOP_paani
#
# clean:
# 	$(RM) count *.o