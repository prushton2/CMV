buildc:
	cd debug; \
	gcc -o main main2.c ../clib/cmv.c -g

run:
	python -m bython-prushton ./src -o ./dist -t
	python ./dist/main.py

clean:
	-rm -rf dist
	-rm main
	-rm cmv