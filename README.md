# sudokuSolver
solves sudoku puzzles

currently it solves sudoku puzzles with only one solution

how to use ?

1. open terminal
2. cd /directoty/to/sudokuSolver
3. make

4.create a text file with your sudoku puzzles

Ex.
  
0 3 9 5 0 0 0 0 0  <br />
0 0 0 8 0 9 0 7 0  <br />
0 0 0 0 1 0 9 0 4  <br />
1 0 0 4 0 0 0 0 3  <br />
0 0 0 0 0 0 0 0 0  <br />
0 0 7 0 0 0 8 6 0  <br />
0 0 6 7 0 8 2 0 0  <br />
0 1 0 0 9 0 0 0 5  <br />
0 0 0 0 0 1 0 0 8  <br />

0's are all the blank cells of the sudoku puzzles

5. ./sudokuSolver < sudoku.txt

6.  get answer
  
  If puzzle is solvable prints solution
  
  Else prints "no solution"
