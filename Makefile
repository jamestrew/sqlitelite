.PHONY: all clean wipe test

SRCS := $(shell fd --type f -e c)

all: db
	./db mydb.db

db: ${SRCS}
	gcc -o $@ $^

# can debug by attaching gdb to the process `sudo gdb -p xxx` where xxx is the process num
# find process id with `ps aux | grep mydb` or something similar
debug: ${SRCS}
	gcc -o $@ $^ -g

%.o: %.c
	gcc -c $< -o $@

test: db
	pipenv run pytest

clean:
	rm -f db debug

wipe:
	@echo "Wiping db data and logs..."
	rm -f mydb.db
	rm -f /tmp/sqlitelite.log
