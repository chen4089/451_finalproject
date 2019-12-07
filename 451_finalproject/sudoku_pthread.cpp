#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#define NUM_THREADS     9

//Puzzles
int easyPuzzle[81];
int meduimPuzzle[81];
int hardPuzzle[81];
int evilPuzzle[81];
int unsolveable[81];

int toSolve; //1 =easy; 2= meduim; 3=hard; 4 = evil; 5 = unsolvable
static bool finished = false;
static bool found = false;
long long start;

pthread_mutex_t print_mutex;

bool valueAllowedCheck(int row, int col, int value, int* puzzle);
void printPuzzle(int* puzzle);


long long start_timer() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}


// Prints the time elapsed since the specified time
long long stop_timer(long long start_time, char* name) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long long end_time = tv.tv_sec * 1000000 + tv.tv_usec;
	printf("%s", name);
	printf("%f seconds\n", ((float)(end_time - start_time)) / (1000 * 1000));
	return end_time - start_time;
}

bool solve(int row, int col, int* puzzle, int counter, int startValue)
{
	if (counter == 81) //every cell has been visted
	{
		return true;

	}
	if (finished)
	{
		return true;
	}
	//loop of column and rows
	col++;
	if (col == 9)
	{
		col = 0;
		row++;
		if (row == 9)
		{
			row = 0;
		}
	}

	//skip cells that are already filled
	if (puzzle[row * 9 + col] != 0)
	{
		return solve(row, col, puzzle, counter + 1, startValue);
	}

	//if the cell is empty
	for (int val = 1; val <= 9; val++)
	{
		//loop through the values being checked recursively
		if (++startValue == 10)
		{
			startValue = 1;
		}

		//check if the value is allowed
		if (valueAllowedCheck(row, col, startValue, puzzle))
		{
			puzzle[row * 9 + col] = startValue; //record if it is

			if (solve(row, col, puzzle, counter + 1, startValue)) //solve the next cell
			{
				return true;
			}
		}
	}
	puzzle[row * 9 + col] = 0; //set to zero if backtracking
	return false;
}

bool valueAllowedCheck(int row, int col, int value, int* puzzle) {
	int i; //loop vairable

	for (i = 0; i < 9; i++)
	{
		if (puzzle[row * 9 + i] == value) //check cells in the row
		{
			return false;
		}
		else if (puzzle[col + i * 9] == value) //check cells in the column
		{
			return false;
		}
		else if (puzzle[(row / 3 * 3 + i % 3) * 9 + (col / 3 * 3 + i / 3)] == value) //check the subsquare
		{
			return false;
		}
	}

	return true; //the value works
}

void* run(void* threadid) {


	int i = rand() % 8; //random row  (0-8)
	int j = rand() % 8; //random column  (0-8)
	int startVal = (int)threadid % 9; //Starting value (1-9) N

	int t;
	int puzzle[81];
	for (t = 0; t < 81; t++) {
		if (toSolve == 1) {
			puzzle[t] = easyPuzzle[t];
		}
		else if (toSolve == 2) {
			puzzle[t] = meduimPuzzle[t];
		}
		else if (toSolve == 3) {
			puzzle[t] = hardPuzzle[t];
		}
		else if (toSolve == 4) {
			puzzle[t] = evilPuzzle[t];
		}
		else if (toSolve == 5) {
			puzzle[t] = unsolveable[t];
		}
	}

	if (solve(i, j, puzzle, 0, startVal)) { //Does this need to be passing a pointer?
			 //the puzzle was solved
		if (!found) {
			pthread_mutex_lock(&print_mutex);
			printf("Puzzle Solved by %d\n", (int)threadid);
			found = true;
			printPuzzle(puzzle);
			long long stop = stop_timer(start, "\nTotal Time: ");
			pthread_exit(NULL);
			pthread_mutex_unlock(&print_mutex);
		}
	}
	else {
		if (!found) {
			pthread_mutex_lock(&print_mutex);
			printf("Not Solved\n");
			found = true;
			printPuzzle(puzzle);
			long long stop = stop_timer(start, "\nTotal Time: ");
			pthread_exit(NULL);
			pthread_mutex_unlock(&print_mutex);
		}
	}

	//timer end here

	if (!finished) {
		finished = true;
	}

	for (t = 0; t < 81; t++) {
		if (toSolve == 1) {
			easyPuzzle[t] = puzzle[t];
		}
		else if (toSolve == 2) {
			meduimPuzzle[t] = puzzle[t];
		}
		else if (toSolve == 3) {
			hardPuzzle[t] = puzzle[t];
		}
		else if (toSolve == 4) {
			evilPuzzle[t] = puzzle[t];
		}
		else if (toSolve == 5) {
			unsolveable[t] = puzzle[t];
		}
	}
}

void printPuzzle(int* puzzle) {
	int i;
	int count = 0;
	int mod;

	for (i = 0; i < 81; i++) {
		mod = count % 3;
		if (count > 8) {
			count = 0;
			printf("\n");
		}
		else if (mod == 0)
		{
			printf("  ");
		}
		count++;
		printf("%d", puzzle[i]);
		printf(" ");
	}
}


int main(int argc, char* argv[]) {

	pthread_t threads[NUM_THREADS];
	long i;
	int rc;
	int inputEasy[81] = { 0,7,0,  0,5,0,  8,0,6,  //row 1
						8,0,0,  1,7,0,  9,5,2,  //row 2
						0,2,5,  0,4,6,  3,0,0,    //row 3

						7,0,0,  0,0,2,  0,0,5,   //row 4
						0,0,4,  0,9,0,  2,0,0,   //row 5
						0,8,0,  3,0,0,  0,0,0,    //row 6

						0,0,8,  5,3,0,  6,1,0,    //row 7
						3,5,1,  0,8,4,  0,0,9,    //row 8
						4,0,7,  0,2,0,  0,0,0   //row 9
	};
	int inputMeduim[81] = { 0,7,6,  3,0,0,  0,0,4,  //row 1
							0,0,0,  0,1,0,  3,9,0,  //row2
							3,2,9,  8,0,0,  5,0,0,   //row3

							0,0,0,  0,0,2,  0,8,0,    //row4
							2,0,0,  0,0,0,  0,0,1,  //row5
							0,8,0,  9,0,0,  0,0,0,   //row6

							0,0,2,  0,0,8,  6,4,9,  //row7
							0,6,7,  0,4,0,  0,0,0,  //row8
							9,0,0,  0,0,6,  7,5,0  //row9
	};
	int inputHard[81] = { 0,0,3,  0,9,0,   0,5,0,  //row1
						0,0,9,  0,0,2,  0,0,0,  //row2
						0,0,0,  5,0,1,  8,0,0,  //row3

						4,5,0,  0,7,3,  0,0,2,  //row4
						0,0,0,  4,0,6,  0,0,0,  //row5
						2,0,0,  1,8,0,  0,4,6,  //row6

						0,0,2,  7,0,4,  0,0,0,  //row 7
						0,0,0,  2,0,0,  5,0,0,  //row8
						0,9,0,  0,1,0,  2,0,0   //row9
	};
	int inputEvil[81] = { 6,0,0, 0,0,8,  9,4,0, //row1
						9,0,0,  0,0,6,  1,0,0, //row2
						0,7,0,  0,4,0,  0,0,0,  ///row3

						2,0,0,  6,1,0,  0,0,0, //row4
						0,0,0,  0,0,0,  2,0,0,  //row5
						0,8,9,  0,0,2,  0,0,0,   //row6

						0,0,0,  0,6,0,  0,0,5,  //row7
						0,0,0,  0,0,0,  0,3,0,  //row8
						8,0,0,  0,0,1,  6,0,0 //row9
	};
	int inputUnsolve[81] = { 1,2,3,   4,5,6,  7,8,0,  //row1
							  0,0,0,  0,0,0,  0,0,2,   //row2
							  0,0,0,  0,0,0,  0,0,3,  //row3

							  0,0,0,  0,0,0,  0,0,4,   //row4
							  0,0,0,  0,0,0,  0,0,5,   //row5
							  0,0,0,  0,0,0,  0,0,6,    //row6

							  0,0,0,  0,0,0,  0,0,7,  //row7
							  0,0,0,  0,0,0,  0,0,8,  //row8
							  0,0,0,  0,0,0,  0,0,9   //row9
	};

	for (i = 0; i < 81; i++) {
		easyPuzzle[i] = inputEasy[i];
		meduimPuzzle[i] = inputMeduim[i];
		hardPuzzle[i] = inputHard[i];
		evilPuzzle[i] = inputEvil[i];
		unsolveable[i] = inputUnsolve[i];
	}

	toSolve = 4;
	start = start_timer();
	for (i = 0; i < NUM_THREADS; i++) {
		rc = pthread_create(&threads[i], NULL, run, (void*)i);
	}
	pthread_exit(NULL);


}