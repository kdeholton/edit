CC = gcc-mp-7
CFLAGS += -Wall -Wpedantic -Werror -g -lncurses

SRCDIR = src
BUILDDIR = build
BINDIR = bin

TARGET := edit
SRCEXT = c
SOURCES := $(shell find  $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TARGETS := $(patsubst $(BUILDDIR)/%,$(BINDIR)/%,$(OBJECTS:.o=))
INC = -I include

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CC) $(CFLAGS) -o $(BINDIR)/$(TARGET) $(OBJECTS)"; $(CC) $(CFLAGS) -o $(BINDIR)/$(TARGET) $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo " $(RM) -rf $(BUILDDIR) $(BINDIR)"; $(RM) -rf $(BUILDDIR) $(BINDIR)

.PHONY: clean
.PRECIOUS: $(SRCDIR)/%.$(SRCEXT) $(BUILDDIR)/%.o
