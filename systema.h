#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "instructions.h"

void readFileIntoMemory(const std::string inputFile, bool(&memory)[256][32]);
void coreDumpToConsole(bool memory[256][32], int programCounter);
void processCycle(int &programCounter, bool(inputBuffer)[32], bool(&outputBuffer)[32], bool(&memory)[256][32]);
int toAddr(const int addrPos, const int programCounter, bool memory[256][32]);
int addrToDec(const int addr, bool memory[256][32]);

void readFileIntoMemory(const std::string inputFile, bool(&memory)[256][32])
{
	const int ASCIIOffset = 48;
	int lineNum = 0;
	std::ifstream machineCode(inputFile);
	std::string line;

	for (int lineNum= 0; lineNum < 256; lineNum++)
	{
		std::getline(machineCode, line);
		for (int CharcterNum = 0; CharcterNum < line.length(); CharcterNum++)
		{
			int numChar = line[CharcterNum];
			numChar = numChar - ASCIIOffset;
			bool slot = numChar;
			memory [lineNum][CharcterNum] = slot;
		}
	}
}


//this prints the state of the machine to the console
void coreDumpToConsole(bool memory[256][32], int programCounter)
{
	std::cout << "CURRENT INSTRUCTION ADDR :" << programCounter << std::endl;
	std::cout << "CORE DUMP OUTPUT : " << std::endl;
	for (int rowNum = 0; rowNum < 16; rowNum++)
	{
		for (int columnNum = 0; columnNum < 32; columnNum++)
		{
			int slot = memory[rowNum][columnNum];
			std::cout << slot;
		}
		std::cout << std::endl;
	}
}


void printOutBuf(bool outputBuffer[32])
{
	long long value = 0;
	long long positionVal = 0;
	for (int bit = 31; bit > 0; bit--)
	{
		value += outputBuffer[bit] ? pow(2, positionVal) : 0;
		positionVal++;
	}
	std::cout << value;
}

void runSystemAProgram(const std::string inputFile)
{
	bool inputBuffer [32];
	bool outputBuffer [32];
	bool memory [256][32];

	bool STP = false;
	int programCounter = 0;

	readFileIntoMemory(inputFile, memory);

	while (STP == false)
	{
		processCycle(programCounter, inputBuffer, outputBuffer, memory);
		if (programCounter < 0)
			STP = true;
		coreDumpToConsole(memory, programCounter); 	std::cin.get();
	}
	printOutBuf(outputBuffer);
}


// this simulates a operating cycle of system/a 
void processCycle(int &programCounter, bool (inputBuffer)[32], bool (&outputBuffer)[32], bool (&memory)[256][32])
{
	const bool iP0 = memory[programCounter][4];
	const bool iP1 = memory[programCounter][5];
	const bool iP2 = memory[programCounter][6];
	const bool iP3 = memory[programCounter][7];

	const int argAddr1 = toAddr(0, programCounter, memory);
	const int argAddr2 = toAddr(1, programCounter, memory);
	const int tgtAddr = toAddr(2, programCounter, memory);

	std::cout << "ProgCount : " << programCounter << " AA1 : " << argAddr1 << " AA2 : " << argAddr2 << " TGA : " << tgtAddr << " iP0 : " << iP0 << " iP1 : " << iP1 << " iP2 : " << iP2 << " iP3 : " << iP3 << std::endl;

	// FLP 00001101
	if (iP0 && iP1 && !iP2 && iP3)
		FLP(argAddr1, programCounter, memory);

	// ADD 00000010
	else if (!iP0 && !iP1 && iP2 && !iP3)
		ADD(argAddr1, argAddr2, tgtAddr, programCounter, memory);

	// JMP 00001010
	else if (iP0 && !iP1 && iP2 && !iP3)
		JMP(tgtAddr, programCounter, memory);

	// JIE 00000101
	else if (!iP0 && iP1 && !iP2 && iP3)
		JIE(argAddr1, argAddr2, tgtAddr, programCounter, memory);

	// WRT 00000011
	else if (!iP0 && !iP1 && iP2 && iP3)
		WRT(tgtAddr, outputBuffer, programCounter, memory);

	// RED 00001100
	else if (iP0 && iP1 && !iP2 && !iP3)
		RED(tgtAddr, inputBuffer, programCounter, memory);

	// MOV 00000100 -> this moves the 
	else if (!iP0 && iP1 && !iP2 && !iP3)
		MOV(argAddr1, tgtAddr, programCounter, memory);

	// STP 00001111 -> this stops the machine
	else if (iP0 && iP1 && iP2 && iP3) {
		STP(programCounter);
	}

	// if given illegal opcode the system ignores it and increments the program counter
	else {
		programCounter++;
	}
}


int toAddr(const int addrPos, const int programCounter, bool memory[256][32])
{
	const int addrPosF = (addrPos * 8) + 8;

	const int p0 = (memory[programCounter][addrPosF + 0]) ? 128 : 0;
	const int p1 = (memory[programCounter][addrPosF + 1]) ? 64 : 0;
	const int p2 = (memory[programCounter][addrPosF + 2]) ? 32 : 0;
	const int p3 = (memory[programCounter][addrPosF + 3]) ? 16 : 0;

	const int p4 = (memory[programCounter][addrPosF + 4]) ? 8 : 0;
	const int p5 = (memory[programCounter][addrPosF + 5]) ? 4 : 0;
	const int p6 = (memory[programCounter][addrPosF + 6]) ? 2 : 0;
	const int p7 = (memory[programCounter][addrPosF + 7]) ? 1 : 0;

	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7;
}


int addrToDec(const int addr, bool memory[256][32])
{
	long long value = 0;
	for (int bit = 31; bit > 0; bit--)
	{
		value += memory[addr][bit] ? pow(2, bit) : 0;
	}
	return value;
}
