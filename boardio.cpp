#include <iostream>
#include "boardio.h"
void makeBoard(int board[9][9]){

    int input;
    //get the input
    for(int r=0;r<9; r++){
        for(int c=0;c<9; c++){
            
            std::cin >> input;
            board[r][c] = input;
            
        }
    }
}    

void printline(){

    std::cout<<"    --------------------------------------------------------"<<std::endl;
}

void printBoard(int board[9][9]){
    
    // print the col line
    
    std::cout << "      ";
    
    for (int i=0;i<9;i++){
        
        std::cout <<"col" <<i+1 << "  "; 
    }
    
    std::cout<<std::endl<<std::endl;
    
    for(int r=0;r<9; r++){
        
        std::cout<<"row"<<r+1<<"   ";
        for(int c=0;c<9; c++){
            
            if(board[r][c]>0){

                std::cout << board[r][c]<<"  |  ";
            }else{

                std::cout<<" "<<"  |  ";
            }                
        }   
    
    std::cout << std::endl;
    printline();
    }
    
}
