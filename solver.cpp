//#include <iostream> // for debugging purposes
#include <bitset>
#include "solver.h"
#include "boardio.h"


//checks if a cell in board is locked in
static bool isLocked(int board[9][9],int row,int col){

	return board[row][col]>0;
}

//removes digit from the row (var)row except (vars) (row,col)
static void removeFromRow(std::bitset<9> solBoard[9][9],int digit, int row,int col){

	for(int i=0;i<9;i++){

		if(i != col){
			//digit -1 since cell 0~8 and digit 1~9
			solBoard[row][i][digit-1]=0;
	 	}
	}
}

//removes digit from the col (var)col except (vars) (row,col)
static void removeFromCol(std::bitset<9> solBoard[9][9],int digit, int row,int col){

	for(int j=0;j<9;j++){

		if(j != row){
			solBoard[j][col][digit-1]=0;
	 	}
	}
}

//removes digit from box containing (var)(row,col) except (vars) (row,col)
static void removeFromBox(std::bitset<9> solBoard[9][9],int digit,int row,int col){

	int boxCornerRow= 3*(row/3);
	int boxCornerCol = 3*(col/3);

	for(int i = boxCornerRow; i<boxCornerRow+3; i++){

		for(int j= boxCornerCol; j<boxCornerCol+3;j++){

			//if they are not the same cell remove
			if(i!=row || j!=col){

				solBoard[i][j][digit-1] = 0;
			}
		}
	}
}


//remove digit from row col and box
static void removeFromAll(std::bitset<9> solBoard[9][9],int digit, int row,int col){

	removeFromRow(solBoard,digit,row,col);
	removeFromCol(solBoard,digit,row,col);
	removeFromBox(solBoard,digit,row,col);

}

//if there is only one digit set to 1 it gets the position of that 1
static int getOnlyDigit(std::bitset<9> oneDigSet){

	int i=0;

	while(oneDigSet[i]!=1 && i<9){
		
		i++;
	}

	return i+1;
}

//locks in a digit into sudoku board
//this is commented to keep the compiler happy
static void lockInDigit(int board[9][9],std::bitset<9> solBoard[9][9],int digit, int row,int col){

		board[row][col] = digit;
		removeFromAll(solBoard,digit,row,col);
}

//this is used for the init phase for locking in for the bitboard 
static void initLockInDigit(std::bitset<9> solBoard[9][9],int digit,int row,int col){
	
	std::bitset<9> setter;
	
	//set all the bits to 0 except the digit th one 
	setter.reset();
	setter[digit-1]=1;
	solBoard[row][col] = setter;

}

// set all bits in bitBoard to 1
static void createSolBoard(std::bitset<9> solBoard[9][9]){

	
	//initialize all the values to 1
	for(int i=0;i<9;i++) {
		
		for(int j=0;j<9;j++){ 

			solBoard[i][j].set();
	
		}
	}

}

// set up the inital state of bit board
static void setUpSolBoard(std::bitset<9> solBoard[9][9], int board[9][9]){

	for(int row=0;row<9;row++){

		for(int col=0;col<9;col++){

			//if the entry is fixed
			if(isLocked(board,row,col)){

				int digit = board[row][col];

				//set the bits to hold a bitset with digit-1 set to 1
				initLockInDigit(solBoard,digit,row,col);

				removeFromAll(solBoard,digit,row,col);
		
			}
		}
	}
}

// counts the number of bits in bitset that is set to 1
static int numberOfElem(std::bitset<9> solBoard[9][9],int row,int col){

	std::bitset<9> cell = solBoard[row][col];
	int numOfElem = 0;
	
	for(int i=0;i<9;i++){

		if(cell[i]==1){

			numOfElem++;
		}
	}
	//std::cout<<"number of elements in "<<cell<<"="<<numOfElem<<std::endl<<std::endl;
	return numOfElem;
}

// checks if a digit can be locked in at position (row,col)
static bool canBeLockedIn(std::bitset<9> solBoard[9][9],int row,int col){

	if(numberOfElem(solBoard,row,col)==1){

		return true;
	
	}else{

		return false;
	}
} 


static int numberOfSubsetsInRow(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	// the set is the number of availabe numbers
	int numOfSubsetsInRow = 0;
	
	std::bitset<9> currCellSet = solBoard[row][col];

	//std::cout<<"looking at subsets in row "<<"("<<row+1<<","<<col+1<<") "<<currCellSet<<std::endl;
	
	for(int c=0;c<9;c++){

		std::bitset<9> checkCellSet = solBoard[row][c];
		
		if((currCellSet | checkCellSet) == currCellSet){

			//std::cout<<"("<<row+1<<","<<c+1<<") "<<checkCellSet<<std::endl;
			numOfSubsetsInRow++;
		}
	}
	//std::cout<<"number of subset in row ="<<numOfSubsetsInRow<<std::endl<<std::endl;
	return numOfSubsetsInRow;
}

static int numberOfSubsetsInCol(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	// the set is the number of availabe numbers
	int numOfSubsetsInCol = 0;
	std::bitset<9> currCellSet = solBoard[row][col];

	//std::cout<<"looking at subsets in col "<<"("<<row+1<<","<<col+1<<") "<<currCellSet<<std::endl;
	for(int r=0;r<9;r++){

		std::bitset<9> checkCellSet = solBoard[r][col];
		
		if( (currCellSet | checkCellSet) == currCellSet) {

			//std::cout<<"("<<r+1<<","<<col+1<<") "<<checkCellSet<<std::endl;
			numOfSubsetsInCol++;
		}
	}

	//std::cout<< "number of subsets in col =" << numOfSubsetsInCol<<std::endl<<std::endl;
	return numOfSubsetsInCol;
}

static int numberOfSubsetsInBox(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	// the set is the number of availabe numbers
	int numOfSubsetsInBox = 0;
	
	std::bitset<9> currCellSet = solBoard[row][col];

	//std::cout<<"looking at subsets in box "<<"("<<row+1<<","<<col+1<<") "<<currCellSet<<std::endl;
	int boxCornerRow = 3*(row/3);
	int boxCornerCol = 3*(col/3);

	for(int r = boxCornerRow;r<boxCornerRow+3;r++){

		for(int c = boxCornerCol; c< boxCornerCol+3;c++){

			std::bitset<9> checkCellSet = solBoard[r][c];
		
			if(((currCellSet | checkCellSet) == currCellSet)){

				//std::cout<<"("<<r+1<<","<<c+1<<") "<<checkCellSet<<std::endl;
				numOfSubsetsInBox++;
			}
		
		}
	}
	//std::cout<<"number of subset in box ="<<numOfSubsetsInBox<<std::endl<<std::endl;
	return numOfSubsetsInBox;
}

static bool preEmptiveSetExistsRow(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	return numberOfElem(solBoard,row,col) == numberOfSubsetsInRow(solBoard,board,row,col);
}

static bool preEmptiveSetExistsCol(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	return numberOfElem(solBoard,row,col) == numberOfSubsetsInCol(solBoard,board,row,col);
}
static bool preEmptiveSetExistsBox(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	return numberOfElem(solBoard,row,col) == numberOfSubsetsInBox(solBoard,board,row,col);
}

//eliminate elements of the bitset from elements not in pre emptive set containing currBitSet
static void rowElim (std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	std::bitset<9> currBitSet = solBoard[row][col];
	
	//compCurrBitSet is the complement of currBitSet
	std::bitset<9> compCurrBitSet = currBitSet;
	compCurrBitSet.flip();
	//std::cout<<"Eliminate Row"<<std::endl;
	//std::cout<<"(r,c) ="<<"("<<row+1<<","<<col+1<<")"<<std::endl;
	//std::cout<<"set is ";
	for(int c=0;c<9;c++){

		std::bitset<9> checkBitSet = solBoard[row][c];
		
		//if not in preemptive set eliminate
		if((currBitSet | checkBitSet) != currBitSet){

			solBoard[row][c] = (compCurrBitSet & checkBitSet);
		
		}
		/*
		else if(((currBitSet | checkBitSet) == currBitSet) ){

			std::cout<<"("<<row+1<<","<<c+1<<") "<<checkBitSet<<std::endl;
		}
		*/	
	}

}

static void colElim (std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	std::bitset<9> currBitSet = solBoard[row][col];
	std::bitset<9> compCurrBitSet = currBitSet;
	compCurrBitSet.flip();
	//std::cout<<"Eliminate Column"<<std::endl;
	//std::cout<<"(r,c) ="<<"("<<row+1<<","<<col+1<<")"<<std::endl;
	//std::cout<<"set is ";
	for(int r=0;r<9;r++){

		std::bitset<9> checkBitSet = solBoard[r][col];
		
		//if not in preemptive set eliminate
		if((currBitSet | checkBitSet) != currBitSet){

			solBoard[r][col] = (compCurrBitSet & checkBitSet);
		
		}

		/*
		else if((currBitSet | checkBitSet) == currBitSet){

				std::cout<<"("<<r+1<<","<<col+1<<") "<<checkBitSet<<std::endl;
		}

		*/	
	}
}

static void boxElim(std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	std::bitset<9> currBitSet = solBoard[row][col];
	std::bitset<9> compCurrBitSet = currBitSet;
	compCurrBitSet.flip();
	//std::cout<<"Eliminate Box"<<std::endl;
	//std::cout<<"(r,c) ="<<"("<<row+1<<","<<col+1<<")"<<std::endl;
	//std::cout<<"set is ";
	int boxCornerRow = 3*(row/3);
	int boxCornerCol = 3* (col/3);

	for(int r = boxCornerRow;r<boxCornerRow+3;r++){

		for(int c = boxCornerCol;c<boxCornerCol+3;c++){

			std::bitset<9> checkBitSet = solBoard[r][c];
			
			if((currBitSet | checkBitSet) != currBitSet){

				solBoard[r][c] = (compCurrBitSet & checkBitSet);
			
			}

			/*
			else if((currBitSet | checkBitSet) == currBitSet ){

				std::cout<<"("<<r+1<<","<<c+1<<") "<<checkBitSet<<std::endl;
			}

			*/	

		}
	}
}

static void preEmptiveElim (std::bitset<9> solBoard[9][9],int board[9][9],int row,int col){

	if(preEmptiveSetExistsRow(solBoard,board,row,col)){

		rowElim(solBoard,board,row,col);
	}
	
	if(preEmptiveSetExistsCol(solBoard,board,row,col)){

		colElim(solBoard,board,row,col);
	}
	 
	 if(preEmptiveSetExistsBox(solBoard,board,row,col)){

		boxElim(solBoard,board,row,col);
	}
}

// returns if there was an elimination due to premptive set


static bool solved(int board[9][9]){

	for(int i=0;i<9;i++){

		for(int j=0;j<9;j++){

			if(board[i][j]==0){

				return false;
			}

		}
	}

	return true;
}

/*
void printSolBoard(std::bitset<9> sol_board[9][9]){

	int i,j,k;
	for(i=0;i<9;i++){

		for(j=0;j<9;j++){

			std::cout<<"("<<i+1<<","<<j+1<<"): ";

			for(k=0;k<9;k++){

				if(sol_board[i][j][k]==1){

				std::cout<<k+1<<" ";
				}
			}


			std::cout<<" ";
			if((j+1)%3==0){

				std::cout<<std::endl;
			}
		}

		std::cout<<std::endl;

	}

}
*/
/*
void copySolBoard(std::bitset<9> solBoard[9][9],std::bitset<9> copyOfSolBoard[9][9]){

	for(int i=0;i<9;i++){

		for(int j=0;j<9;j++){

			copyOfSolBoard[i][j] = solBoard[i][j];
		}
	}

}
*/

/*
void showChange(std::bitset<9> solBoard[9][9], std::bitset<9> copyOfSolBoard[9][9]){

	for(int i=0;i<9;i++){

		for(int j=0;j<9;j++){


			if(copyOfSolBoard[i][j] != solBoard[i][j]){

				std::cout<<"("<<i+1<<","<<j+1<<") ";
				for(int k=0;k<9;k++){

					if(copyOfSolBoard[i][j][k]==1){

						std::cout<<k+1<<", ";
					}	
				}

				std::cout<<" -> ";

				for(int k=0;k<9;k++){

					if(solBoard[i][j][k]==1){

						std::cout<<k+1<<",";
					}	
				}

				std::cout<<std::endl;

			}
		}
	}

}
*/

static bool solvable(std::bitset<9> solBoard[9][9]){

	// the bits of blank are all set to 0
	std::bitset<9> blank;
	blank.reset();
	for(int i =0;i<9;i++){

		for(int j=0;j<9;j++){

			if(solBoard[i][j]== blank){

				return false;
			}

		}
	}

	return true;
}

bool solveBoard(int board[9][9]){

	// this will be our main board used for solving
	std::bitset<9> solBoard[9][9];
	//std::bitset<9> copyOfSolBoard[9][9];

	//get the number of fixed cells
	//int numOfLockedCells = numberOfLockedCells(board);
	//each position will hold a std::bitset<9>
	//a std::bitset<9> indicates the possible numbers that can be selected for a cell
	//Ex 110010001
	//the 1st,2nd,5th and 9th bits are set to 1 thus the candidates are 1,2,5 and 9  
	
	// initiate the solBoard 

	// I decided to not allocate memory dynamically
	// because really there's not point if size is fixed
	createSolBoard(solBoard);
	//printSolBoard(solBoard);
	
	setUpSolBoard(solBoard,board);
	//printSolBoard(solBoard);
	do{

		for(int r=0;r<9;r++){

			for(int c=0;c<9;c++){

				//std::cout<<std::endl<<"========================================================================================"<<std::endl;
				//std::cout<<"processing for "<< r+1 <<" "<< c+1 <<std::endl;
				//std::cout<<std::endl;
				
				//printSolBoard(solBoard);
				//printBoard(board);
				
				//copySolBoard(solBoard,copyOfSolBoard);
				if(!isLocked(board,r,c)){

					preEmptiveElim(solBoard,board,r,c);

					if(canBeLockedIn(solBoard,r,c) && !isLocked(board,r,c)){

							int digit = getOnlyDigit(solBoard[r][c]);
							lockInDigit(board,solBoard,digit,r,c);
						
								//numOfLockedCells++;
					}
				}
			
				//showChange(solBoard,copyOfSolBoard);

				//std::cout<<"finished processing "<<r+1<<" "<< c+1 <<std::endl;
				//printSolBoard(solBoard);
				//printBoard(board);
			}		
		}

				
	

			
	//std::cout<<std::endl<<"========================================================================================"<<std::endl;	
	//printSolBoard(solBoard);
	//printBoard(board);
	//pause();

	}while(!solved(board) && solvable(solBoard));

	if(solved(board)){

		return true;
	
	}else{

		return false;
	}

	///printSolBoard(solBoard);
}