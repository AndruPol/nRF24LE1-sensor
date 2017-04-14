# Functions needed by this makefile and the one that builds the source libraries (MakefileSrc)
export ECHO = @echo
export RM = rm
export SED = sed
export MKDIR = mkdir -p
export RMDIR = rmdir
export TR = tr
export BLACKHOLE = /dev/null
export PWD = pwd
export CD = cd
export LS = ls

# Programs to use for creating dependencies, compiling source files, and creating the library file, respectively
DEP = sdcc
CC  = sdcc
LIB = sdcclib

# Flags for above programs when calling them from the command line
DFLAGS = -MM $(INCDIRS) $<
CFLAGS = --model-large --std-c99 $(INCDIRS) -c $< -o "obj/"
LFLAGS =

# File extensions for dependency files, source files, object files, and library files, respectively
DEPEXT = d
SRCEXT = c
OBJEXT = rel
LIBEXT = lib

# Compiler version
ifneq ($(MINCOMPILERBUILD),)
CCVERSION := $(subst \#,,$(filter \#%,$(strip $(shell $(CC) -v))))
CCVERSIONOK := $(shell [ $(CCVERSION) -ge $(MINCOMPILERBUILD) ] && echo "YES" || echo "NO")
ifeq ($(CCVERSIONOK),NO)
$(error "The build has detected an SDCC build of #$(CCVERSION). This library must be built by an SDCC build of at least #$(MINCOMPILERBUILD). Please update your SDCC installation.")
endif
endif

# Source dir name, include directories, and sub source dir
TARGET := aes
INCDIRS = -Iinclude
SRCDIR = src

# Directories for objects, dependencies, and library files
OBJDIR = obj
DEPDIR = dep
LIBDIR = lib

# Name of library file, list of source files, object files, and dependency files
LIBFILE = $(LIBDIR)/$(TARGET).$(LIBEXT)
LIBFILES = $(subst .$(OBJEXT),.$(LIBEXT),$(subst $(OBJDIR),$(LIBDIR),$(OBJFILES)))
SRCFILES := $(shell $(LS) $(SRCDIR)/*.$(SRCEXT))
OBJFILES = $(subst .$(SRCEXT),.$(OBJEXT),$(subst $(SRCDIR),$(OBJDIR),$(SRCFILES)))
DEPFILES = $(subst .$(SRCEXT),.$(DEPEXT),$(subst $(SRCDIR),$(DEPDIR),$(SRCFILES)))

# Used to makes sure source files get built if their dependency files are modified
#-include $(DEPFILES)

all: $(LIBFILE)

$(LIBDIR)/%.$(LIBEXT): $(OBJFILES)
	$(ECHO)
	$(ECHO) "Building library file '$@'"
	[ -d $(LIBDIR) ] || $(MKDIR) $(LIBDIR) > $(BLACKHOLE)
	$(LIB) $(LFLAGS) $@ $(OBJFILES)
	$(ECHO) "Finished building library file '$@'"
	$(ECHO)

$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT) $(DEPDIR)/%.$(DEPEXT)
	$(ECHO)
	$(ECHO) "Building object file '$@'"
	[ -d $(OBJDIR) ] || $(MKDIR) $(OBJDIR) > $(BLACKHOLE)
	$(CC) $(CFLAGS)
	$(ECHO) "Finished building object file '$@'"
	
clean:
	$(RM) -rf  $(OBJDIR)/* $(DEPDIR)/* $(LIBDIR)/* > $(BLACKHOLE)
	[ -d $(DEPDIR) ] &&	$(RMDIR) $(DEPDIR) > $(BLACKHOLE)
	[ -d $(OBJDIR) ] &&	$(RMDIR) $(OBJDIR) > $(BLACKHOLE)
	[ -d $(LIBDIR) ] &&	$(RMDIR) $(LIBDIR) > $(BLACKHOLE)

$(DEPDIR)/%.$(DEPEXT): $(SRCDIR)/%.$(SRCEXT)
	$(ECHO)
	$(ECHO) "Building dependency file '$@'"
	[ -d $(DEPDIR) ] || $(MKDIR) $(DEPDIR) > $(BLACKHOLE)
	$(ECHO) "$(OBJDIR)/" | $(TR) -d '\n' | $(TR) -d '\r' > $@.tmp
	$(DEP) $(DFLAGS) >> $@.tmp
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	$(RM) -f $@.tmp
	$(ECHO) "Finished building dependency file '$@'"

#.SECONDARY: $(OBJFILES) $(DEPFILES)
