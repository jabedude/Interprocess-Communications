CFLAGS+=-Wall -Wextra -Wpedantic
CFLAGS+=-Wwrite-strings -Wstack-usage=1024 -Wfloat-equal
CFLAGS+=-Waggregate-return -Winline
CFLAGS+=-std=c11

.PHONY: clean debug profile

BINS=dispatcher listener

DEPS1=dispatcher.o relay.o
DEPS2=listener.o relay.o

all: $(BINS)
dispatcher: $(DEPS1)
listener: $(DEPS2)

debug: CFLAGS+=-g
debug: $(BINS)

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(BINS)

clean:
	$(RM) $(DEPS1) $(DEPS2) $(BINS)

