#include <iostream>
#include <string>
#include "assembler.h"
#include "systema.h"
#include "instructions.h"

/*
system/a

this is the program that contains the code for simulating a
system/a computer and includes a translator for
output, a complete system/a instruction set and a full
assembler for converting ASM to machine code.
*/

int main()
{
	assembleFile("test.saa", "test.sam");
	runSystemAProgram("test.sam");
	std::cin.get();
	return 0;
}