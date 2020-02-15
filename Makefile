CC=gcc
CLIBS=
TARGET=default
SRCD=./src
OBJD=./obj
INCD=./inc
BDIR=./bin
STDRD=-std=gnu11
SRCF=$(filter-out $(SRCD)/*.c~, $(wildcard $(SRCD)/*.c))
OBJF=$(patsubst $(SRCD)/%.c, $(SRCD)/%.o, $(SRCF))
BINNAME=timerApp

#Creates a list of files in the src directory
build=complete

#Checkes whether the "TARGET" variable has been set
ifneq "$(TARGET)" "default"
build=target
endif

#Runs the appropriate target
all: $(build)

#Cycles through all files in src/ directory to make
complete: $(SRCF)
	gcc $(SRCF) -o $(BDIR)/$(BINNAME) $(CLIBS) -I$(INCD)/ $(STDRD)

#Makes a specific target
target:
	gcc $(SRCD)/$(TARGET).c -o $(BDIR)/$(TARGET) $(CLIBS) -I$(INCD)/ $(STDRD)

#Executes a specific binary
.PHONY: run
run:
	./bin/$(BINNAME)

#Cleans the directory
.PHONY: clean
clean:
	rm -rf ./*~
	rm -rf $(BDIR)/*
	rm -rf $(SRCD)/*~
	rm -rf $(INCD)/*~
