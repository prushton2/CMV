buildc:
	gcc -o main debug/main.c src/cmv.c -g

run:
	python -m bython-prushton ./src -o ./dist -t

clean:
	-rm -rf dist
	-rm main
	-rm cmv