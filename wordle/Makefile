CFLAGS := -F/Library/Frameworks -framework SDL2 -DTORN_OPENGL -w -DINIREAD_IMPL
LDFLAGS := -Wl,-rpath,/Library/Frameworks
SRC := $(shell find . -type f -name "*.c" -not -path "glad/")
HDR := $(shell find . -type f -name "*.h")
EXEC := game_proj

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -I. $(SRC) -o $(EXEC) $(LDFLAGS)
.PHONY:
	clean run

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)
