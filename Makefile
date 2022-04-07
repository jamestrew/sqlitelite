.PHONY: all
all: db
	./db mydb.db

db: src/db.c src/input_handler.c src/tables.c src/pager.c src/dev/logging.c src/cursor.c src/btree.c src/dev/testing.c
	gcc -o $@ $^

# can debug by attaching gdb to the process `sudo gdb -p xxx` where xxx is the process num
# find process id with `ps aux | grep mydb` or something similar
debug: src/db.c src/input_handler.c src/tables.c src/pager.c src/dev/logging.c src/cursor.c src/btree.c src/dev/testing.c
	gcc -o $@ $^ -g

%.o: %.c
	gcc -c $< -o $@

.PHONY: test
test:
	pipenv run pytest

.PHONY: clean
clean:
	rm -f db debug

.PHONY: wipe
wipe:
	@echo "Wiping db data and logs..."
	rm -f mydb.db
	rm -f /tmp/sqlitelite.log
