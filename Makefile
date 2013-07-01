CC          := gcc -Wall -Wextra -pedantic -std=gnu99
CFLAGS      :=
LDFLAGS     :=

SOURCES     := $(wildcard *.c)
DEPENDS     := $(subst .c,.d,$(SOURCES))
MAINOBJS	  := $(subst .c,.o,$(shell grep -l "MAKE MAIN" $(SOURCES)))
SHAREDOBJS  := $(subst .c,.o,$(shell grep -l "MAKE SHARED LIBRARY" $(SOURCES)))
MAIN        := $(subst .o,,$(MAINOBJS))
SHARED			:= $(patsubst %.o,lib%.so,$(SHAREDOBJS))
TESTFILES		:= file0 file1 file2 file3 file4 file5 file6 file7
TRACEDFILES	:= $(filter-out file7, $(TESTFILES))

all: $(DEPENDS) $(MAIN) $(SHARED)

$(DEPENDS) : %.d : %.c
	$(CC) -MT $(<:.c=.o) -MM $< > $@
	@echo -e "\t"$(CC) -c $(CFLAGS) $< -o $(<:.c=.o) >> $@

$(MAIN) : % : %.o
	$(CC) $(LDFLAGS) -o $@ $^

$(SHARED) : lib%.so : %.o
	$(CC) $(LDFLAGS) -shared -Wl,-soname=$@ -o $@ $^

-include $(DEPENDS)

test-setup: all
	cd /sys/kernel/debug/tracing/; \
		echo 0 > tracing_on; \
		echo file_trace > current_tracer; \
		echo 1 > tracing_on
	touch $(TESTFILES)
	setfattr -n user.file_trace $(TRACEDFILES)

test: test-setup
	./rmux

clean:
	-rm -f *.o $(DEPENDS) $(MAIN) $(SHARED) $(MAINOBJS) $(SHAREDOBJS)
	-rm -f $(TESTFILES)

.PHONY: clean test
