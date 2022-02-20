db: db.c
	gcc db.c input_handler.c -o db

run: db
	./db

clean:
	rm -f db
