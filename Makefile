db: db.c
	gcc db.c -o db

run: db
	./db

clean:
	rm -f db
