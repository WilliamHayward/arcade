arcade: arcade.c arcade.h common.h common.c
	gcc -std=gnu99 -o builds/arcade arcade.c arcade.h common.h common.c -I.
rummi: rummi.c rummi.h common.h common.c
	gcc -std=gnu99 -o builds/rummi rummi.c rummi.h common.h common.c -I.
