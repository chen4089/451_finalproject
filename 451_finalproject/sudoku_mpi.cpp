#include<iostream>
#include<fstream>
#include <mpi.h>
#include<stdlib>
#

#define size 9
using namespace std;

int solution[size][size] = { 0 };
int isGiven[size][size] = { 0 };


void printTable(int table[size][size])
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << table[i][j] <<" ";
		}
		cout << endl;
	}
}


void loadTable(const char* fileName, int table[size][size])
{
	ifstream in;
	in.open(fileName);
	if (!in)
	{
		cout << "file load fail!" << endl;
		return;
	}

	char temp;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			in >> temp;
			table[i][j] = temp-'0';
			if (temp != '0') isGiven[i][j] = 1;
		}
	}

	in.close();

}

int main(int argc, char* argv[])
{
	int sudoku[size][size] = { 0 };
	string file = "problem.txt";
	string result = "result.txt";
	loadTable(file.c_str(), sudoku);
	printTable(sudoku);
}
