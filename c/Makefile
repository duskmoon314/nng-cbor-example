CC = gcc

mkdir-build:
	mkdir -p build

build-producer: mkdir-build
	$(CC) -o build/producer src/producer.c -lnng -lcbor

clean:
	rm -rf build

.PHONY: clean