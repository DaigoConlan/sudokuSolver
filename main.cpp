#include <iostream>
#include "boardio.h"
#include "solver.h"

int main(){
    
    // this will be the sudoku board 
    int board[9][9];
    
    makeBoard(board);
    
    std::cout<< std::endl<<"your board"<<std::endl<<std::endl;
    printBoard(board);
    
    bool solved = solveBoard(board);

    if(solved){
    	
    	std::cout<<std::endl<<"solution to your board"<<std::endl<<std::endl;
    	printBoard(board);
   
    }else{

    	std::cout<<"There is no solution to your board"<<std::endl;
    }

    return 0;
   
}
