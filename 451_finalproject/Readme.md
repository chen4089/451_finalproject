# Sudoku-solving-parallel-computing
## Simple Backtracking
Both cpu_backtracking.cpp and gpu_backtracking.cu was tested on USC HPC Server. 

Input file prepare:
```bash
Insert your Sudoku probelm in a .txt file.
Use '0' represents a empty space.
Use space to separate digits.
Each row goes into a new line.
```
Run serial code:
```bash
Compile: 
	"g++ -std=c++11 -o cpurun cpu_backtracking.cpp"
run: 
	./cpurun ...[filepath]/[fullname of you Sudoku file] 
```
Running parallcl code:
```bash
Method 1：
	Write your own .sl file:
		#!/bin/sh
		#SBATCH --gres=gpu:1

		#SBATCH -o cuda1.out

		#SBATCH -e cuda1.error

		source /usr/usc/cuda/default/setup.sh

		nvcc -o cudasudoku gpu_backtracking.cu

		./cudasudoku [threadperblock] [block number] [full path of your Sudoku file]
	Submit to server by running "sbatch [filename].sl"

Method 2:
	Require GPU resources on Server:
		salloc --gres=gpu:k20:1 --time=02:00:00
		source /usr/usc/cuda/default/setup.sh
	Compile .cu file:
		nvcc -o cudasudoku gpu_backtracking.cu
	Run .cu file:
		./cudasudoku [threadperblock] [block number] [full path of your Sudoku file]
```
