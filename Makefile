.PHONY: all
all: db
	./db mydb.db

db: src/db.c src/input_handler.c src/tables.c src/pager.c src/dev/logging.c
	gcc -o $@ $^

%.o: %.c
	gcc -c $< -o $@

.PHONY: test
test:
	pipenv run pytest

.PHONY: clean
clean:
	rm -f db

.PHONY: wipe
wipe:
	@echo "Wiping db data and logs..."
	rm -f mydb.db
	rm -f /tmp/sqlitelite.log
