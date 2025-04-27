CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = main.o my_string.o 
CFILES = main.c my_string.c generic_vector.c ../clib/cmv.c

main:
	$(CC) $(CFLAGS) -o main $(CFILES) -g
# main.o: main.c
# 	$(CC) $(CFLAGS) -c main.c -o main.o
# clean:
# 	rm string_driver $(OBJECTS)

#unit_test_bad: my_string.o unit_test.o test_def.o
#	$(CC) $(CFLAGS) -o unit_test unit_test.o test_def.o my_string.o
#unit_test:
#	$(CC) $(CFLAGS) -o unit_test -I . unit_test.c test_defs.c my_string.c
