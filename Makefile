make:
	bython -c ./src
	./venv/bin/python ./build/main.py
build:
	bython -c ./src -o transpile
	./venv/bin/pyinstaller transpile/main.py  --onefile --add-data "src/style.tcss:."

install:
	make build
	cp ./dist/main /bin/CMV
	make clean

clean:
	rm -rf ./build
	rm -rf ./dist
	rm -rf ./main.spec
	rm -rf ./transpile

c:
	gcc -o main main.c -g
