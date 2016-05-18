#include <stdio.h>

#include "game.c"

int main(int argc, char const *argv[])
{
	init_field();
	convert();
	field_maker(contener);
	return 0;
}