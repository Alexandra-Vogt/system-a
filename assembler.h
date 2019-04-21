#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>



int assembleFile(const std::string inputFile, const std::string outputFile);
std::string assembleLine(std::string asmLine);
int hexToDec(const std::string hexNum);


// this assembles a system/a assembly file and converts it into a system/a machine code file
int assembleFile(const std::string inputFile, const std::string outputFile)
{
	std::string machineLine, machineCode;

	std::ifstream assembly(inputFile);
	for (std::string line; std::getline(assembly, line);) {
		machineLine = assembleLine(line);
		machineCode += (machineLine + "\n");
	}
	std::ofstream machineCodeFile;
	machineCodeFile.open(outputFile);
	machineCodeFile << machineCode;
	machineCodeFile.close();
	return 0;
}


// this reads lines of fixed-width assembly and converts them into binary machine code
std::string assembleLine(std::string asmLine)
{
	int position = 0;
	std::stringstream asmStream(asmLine);
	std::string token;
	std::string machineLine;
	bool NUMMarker = false;

	std::map<const std::string, const std::string> instructions = {
		{ "FLP", "00001101" },{ "ADD", "00000010" },{ "JMP", "000010100000000000000000" },{ "JIE" , "00000101" },
		{ "WRT" , "000000110000000000000000" },{ "RED" , "000011000000000000000000" },
		{ "MOV", "00000100" },{ "STP", "00001111000000000000000000000000" } };

	while (getline(asmStream, token, ' ') && (machineLine.length() < 32) && (token != ";"))
	{

		if (NUMMarker == true) {
			std::bitset<32> Binary(hexToDec(token));
			machineLine += Binary.to_string();
		}

		else if (instructions.find(token) != instructions.end())
			machineLine += instructions.find(token)->second;

		else if (token == "NUM")
			NUMMarker = true;

		else if (token != "") {
			std::bitset<8> Binary(hexToDec(token));
			machineLine += Binary.to_string();
		}

	}

	while (machineLine.length() < 32)
	{
		machineLine += "00000000";
	}
	return machineLine;
} 


// this converts intigers to decimal values
int hexToDec(const std::string hexNum)
{
	std::stringstream numStream;
	numStream << std::hex << hexNum;
	int decimalNum;
	numStream >> decimalNum;
	return decimalNum;
}
