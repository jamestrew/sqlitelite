.PHONY: run
all: db
	./db

db: src/db.c src/input_handler.c src/tables.c
	gcc -o $@ $^

%.o: %.c
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm -f db
