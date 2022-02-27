.PHONY: all
all: db
	./db

db: src/db.c src/input_handler.c src/tables.c
	gcc -o $@ $^

%.o: %.c
	gcc -c $< -o $@

.PHONY: test
test:
	pipenv run pytest

.PHONY: clean
clean:
	rm -f db
