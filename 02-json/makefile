#Compiler and Linker
CC          := g++

#The Target Binary Program
TARGET      := json 

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
TESTDIR     := tests
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o
TESTEXT     := json

#Flags, Libraries and Includes
CFLAGS      := -Wall -g -c -O0
LIB         := -lm 
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)

RUN_BIN		:= $(TARGETDIR)/./$(TARGET)
RUN_A1      := $(RUN_BIN) $(TESTDIR)/step1/invalid.$(TESTEXT)
RUN_A2      := $(RUN_BIN) $(TESTDIR)/step1/valid.$(TESTEXT)
RUN_B1      := $(RUN_BIN) $(TESTDIR)/step2/invalid.$(TESTEXT)
RUN_B2      := $(RUN_BIN) $(TESTDIR)/step2/invalid2.$(TESTEXT)
RUN_B3      := $(RUN_BIN) $(TESTDIR)/step2/valid.$(TESTEXT)
RUN_B4      := $(RUN_BIN) $(TESTDIR)/step2/valid2.$(TESTEXT)
RUN_C1      := $(RUN_BIN) $(TESTDIR)/step3/invalid.$(TESTEXT)
RUN_C2      := $(RUN_BIN) $(TESTDIR)/step3/valid.$(TESTEXT)
RUN_D1      := $(RUN_BIN) $(TESTDIR)/step4/invalid.$(TESTEXT)
RUN_D2      := $(RUN_BIN) $(TESTDIR)/step4/valid.$(TESTEXT)
RUN_A       := $(RUN_A1);$(RUN_A2)
RUN_B       := $(RUN_B1);$(RUN_B2);$(RUN_B3);$(RUN_B4)
RUN_C       := $(RUN_C1);$(RUN_C2)
RUN_D       := $(RUN_D1);$(RUN_D2)
RUN         := $(RUN_A);$(RUN_B);$(RUN_C)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: $(TARGET)

#Remake
remake: cleaner all

#Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)
	$(RUN)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

run: 
	$(RUN)

#Non-File Targets
.PHONY: all remake clean cleaner resources
