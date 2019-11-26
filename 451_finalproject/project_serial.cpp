//serial sudoku solver--backtracking
//author: Siqi Chen
#include<iostream>
#include <vector>
#include<fstream>

using namespace std;
#define size 9


void loadTable(const char* fileName, vector<vector<int>>& table)
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
		vector<int> line;
		for (int j = 0; j < size; j++)
		{
			in >> temp;
			if (temp >= '1' && temp <= '9') line.push_back((int)(temp - '0'));
			else line.push_back(0);
		}
		table.push_back(line);
	}

	in.close();

}

void printTable(vector<vector<int>> &table)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << table[i][j]<<" ";
		}
		cout << endl;
	}
}

bool checkValid(vector<vector<int>> & table, int i, int j, int val)
{
	//check along row, column and cube for the validation of this assignment
	//the start index of coresponding cube 
	int row = i - i % 3, col = j - j % 3;
	for (int h = 0; h < 9; h++)
	{
		//check along row
		if (table[h][j] == val)
		{
			return false;
		}
	}
	for (int h = 0; h < 9; h++)
	{
		//check along column
		if (table[i][h] == val)
		{
			return false;
		}
	}

	//check within a cube
	for (int h = 0; h < 3; h++)
	{
		for (int k = 0; k < 3; k++)
		{
			if (table[row + h][col + k] == val) return false;
		}
	}

	return true;
}

bool solveSudoku(vector<vector<int>> & table, int i, int j)
{
	//traverse along the line 
	if (i == 9) return true;
	if (j == 9) return solveSudoku(table, i + 1, 0);
	if (table[i][j] != 0) return solveSudoku(table, i , j+1);

	for (int c = 1; c <= 9; c++)
	{
		if (checkValid(table, i, j, c))
		{
			table[i][j] = c;
			if (solveSudoku(table, i, j + 1)) return true;
			table[i][j] = 0;
		}
	}

	return false;
}

void solveSudoku(vector<vector<int>> &table)
{
	solveSudoku(table, 0, 0);
}


void loadToFile(const char* outFile, vector<vector<int>> &table)
{
	ofstream out;
	out.open(outFile);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			out << table[i][j] << " ";
		}
		out << endl;
	}

	out.close();
}



int main()
{
	vector<vector<int>> table;
	string file = "problem.txt";
	string result = "result.txt";
	loadTable(file.c_str(), table);
	printTable(table);
	solveSudoku(table);
	printTable(table);
	loadToFile(result.c_str(), table);
	//while (1);
	return 0;

}

