export CFLAGS = -Wall -Wextra -Werror

CPPFLAGS = -I include

.PHONY: initializer


all: initializer run clean

initializer:
	gcc $(CFLAGS) $(CPPFLAGS) src/*.c -o out

run:
	@./out

clean:
	@rm -f out
