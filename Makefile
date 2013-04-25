CC		:= gcc
AR		:= ar
LEX		:= flex
BISON	:= bison

ifeq ($(DEBUG), 1)
CFLAGS  := -O0 -g -DDEBUG=1 -DYYDEBUG=1 -DYYERROR_VERBOSE=1
BFLAGS  := --verbose -d
LFLAGS  := -d
LDFLAGS :=
else
CFLAGS  := -O3 -ffunction-sections -fdata-sections
BFLAGS  :=
LFLAGS  :=
LDFLAGS := -Wl,-gc-sections
endif

ifeq ($(MAKE_VERBOSE), 1)
Q :=
else
Q := @
endif

all: sHellTest

sHellTest: sHellTest.c sHell.a
	$(Q)$(CC) $^ $(CFLAGS) -o $@ $(LDFLAGS)

sHell.a: sHellParser.c sHellLexer.c sHellStmt.c sHellStmt.h sHell.h sHell.c
	$(Q)$(CC) $^ $(CFLAGS) -c $(LDFLAGS)
	$(Q)$(AR) cr $@ sHellParser.o sHellLexer.o sHellStmt.o sHell.o

sHellParser.c: sHellParser.y
	$(Q)$(BISON) $(BFLAGS) $^

sHellLexer.c: sHellLexer.l
	$(Q)$(LEX) $(LFLAGS) $^

clean:
	$(Q)rm -rf *.o *.a *.gch *.output sHellParser.[ch] sHellLexer.[ch] sHellTest
