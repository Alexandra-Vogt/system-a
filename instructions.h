#pragma once


// this flips the bits of a memory address
void FLP(int argAddr1, int &programCounter, bool (&memory)[256][32])
{
	for (int bit = 0; bit < 32; bit++)
	{
		memory[argAddr1][bit] = !memory[argAddr1][bit];
	}
	programCounter++;
}


// this adds the number in argAddr1 to argAddr2 and places it in the tgtAddr
void ADD(int argAddr1, int argAddr2, int tgtAddr, int &programCounter, bool(&memory)[256][32])
{
	bool carry = false;
	bool contArgAddr1;
	bool contArgAddr2;


	for (int bit = 31; bit > 0; bit--)
	{
		contArgAddr1 = memory[argAddr1][bit];
		contArgAddr2 = memory[argAddr2][bit];

		if (contArgAddr1 && contArgAddr2 && carry)
		{
			memory[tgtAddr][bit] = 1;
			carry = 1;
		}
		else if (((contArgAddr1 && contArgAddr2) && !carry) || ((contArgAddr1 && carry) && !contArgAddr2) || ((contArgAddr2 && carry) && !contArgAddr1))
		{
			memory[tgtAddr][bit] = 0;
			carry = 1;
		}
		else if ((contArgAddr1 && !contArgAddr2 && !carry) || (!contArgAddr1 && contArgAddr2 && !carry) || (!contArgAddr1 && !contArgAddr2 && carry))
		{
			memory[tgtAddr][bit] = 1;
			carry = 0;
		}
		else
		{
			memory[tgtAddr][bit] = 0;
			carry = 0;
		}
	}
	programCounter++;
}


// this unconditionally sets the program counter to the target address
void JMP(int tgtAddr, int &programCounter, bool(&memory)[256][32])
{
	programCounter = tgtAddr;
}


// this sets the program counter to the target address if argAddr1 and argAddr2 is equal
void JIE(int argAddr1, int argAddr2, int tgtAddr, int &programCounter, bool(&memory)[256][32])
{
	bool equal = true;
	int bit = 0;
	while ((bit < 32) && equal)
	{
		if (memory[argAddr1][bit] != memory[argAddr2][bit])
			equal = false;
		bit++;
	}
	if (equal)
		programCounter = tgtAddr;
	else
		programCounter++;
}


// this writes the contents of tgtAddr to the output buffer
void WRT(int tgtAddr, bool (&outputBuffer)[32], int &programCounter, bool(&memory)[256][32])
{
	for (int bit = 0; bit < 32; bit++)
	{
		outputBuffer[bit] = memory[tgtAddr] [bit];
	}
	programCounter++;
}


// this writes the contents of the input buffer to tgtAddr
void RED(int tgtAddr, bool (inputBuffer)[32], int &programCounter, bool(&memory)[256][32])
{
	for (int bit = 0; bit < 32; bit++)
	{
		memory[tgtAddr][bit] = inputBuffer[bit];
		int bitVal = memory[tgtAddr][bit];
		memory[tgtAddr][bit] = 0;
	}
	programCounter++;
}


// this copies the value contained in argAddr1 to tgtAddr
void MOV(int argAddr1, int tgtAddr, int &programCounter, bool(&memory)[256][32])
{
	for (int bit = 0; bit < 32; bit++)
	{
		memory[tgtAddr][bit] = memory[argAddr1][bit];
	}
	programCounter++;
}


// this sets the program counter to an illegal value, causing the computer to stop and return the STP code
void STP(int &programCounter)
{
	programCounter = -1;
}
