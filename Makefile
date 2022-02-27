.PHONY: all
all: db
	./db mydb.db

db: src/db.c src/input_handler.c src/tables.c src/pager.c
	gcc -o $@ $^

%.o: %.c
	gcc -c $< -o $@

.PHONY: test
test:
	pipenv run pytest

.PHONY: clean
clean:
	rm -f db
