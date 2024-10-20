.POSIX:

# includes and libs
CFLAGS = $(INCS)
LDFLAGS = -lm -lraylib

# paths
OBJ_DIR = obj
DEST_DIR = /usr/local

BIN = pop
SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

all: options $(BIN)

options:
	@echo $(BIN) build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"
	@echo "$(OBJ)"

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: */%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

run: $(BIN)
	./$(BIN)

clean:
	$(RM) -rf $(OBJ) $(BIN)

install: $(BIN)
	mkdir -p $(DEST_DIR)/bin
	cp -f $(BIN) $(DEST_DIR)/bin
	chmod 755 $(DEST_DIR)/bin/$(BIN)

uninstall:
	rm -f $(DEST_DIR)/bin/$(BIN)

.PHONY: all options clean install uninstall
