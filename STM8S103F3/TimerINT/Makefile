# We're assuming POSIX conformance
.POSIX:

#Compiler
CC = sdcc

#Platform
PLATFORM = stm8

#Product name
PNAME = main

#Output directory for intermediate and final compiled file(s)
ODIR = bin
#Directory for helpers
IDIR = inc
IDIR += ../stm8s_header_sdcc

SDIR = src
HSDIR = ../stm8s_header_sdcc/src

# In case you ever want a different name for the main source file
MAINSRC = $(PNAME).c

# These are the sources that must be compiled to .rel files:
EXTRASRCS = \
	#$(HSDIR)/stm8s_it.c

# The list of .rel files can be derived from the list of their source files
RELS = $(EXTRASRCS:.c=.rel)

INCLUDES = $(foreach d, $(IDIR), -I$d)
CFLAGS   = -m$(PLATFORM) --out-fmt-ihx --std-c99
LIBS     = -l$(PLATFORM)

# This just provides the conventional target name "all"; it is optional
# Note: I assume you set PNAME via some means not exhibited in your original file
all: clean $(PNAME)

# How to build the overall program
$(PNAME): $(MAINSRC) $(RELS)
	@mkdir -p $(ODIR)
	$(CC) $(INCLUDES) $(CFLAGS) $(LIBS) $(MAINSRC) $(wildcard $(ODIR)/*.rel) -o$(ODIR)/

# How to build any .rel file from its corresponding .c file
# GNU would have you use a pattern rule for this, but that's GNU-specific
.c.rel:
	@mkdir -p $(ODIR)
	$(CC) -c $(INCLUDES) $(CFLAGS) $(LIBS) $< -o$(ODIR)/

# Suffixes appearing in suffix rules we care about.
# Necessary because .rel is not one of the standard suffixes.
.SUFFIXES: .c .rel


#phonies

.PHONY:	clean flash

clean:
	@echo "Removing $(ODIR)..."
	@rm -rf $(ODIR)
	@echo "Done."

flash:
	@echo "Uploading resulted " $(PNAME)".ihx to target board..." 
	stm8flash -c stlinkv2 -p stm8s103f3 -w $(ODIR)/$(PNAME).ihx
	@echo "Flash completed."
	
