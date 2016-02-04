/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */
 
#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "math.h"
#include <ctype.h>
#include "point.h"

using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void setSize( int& boardSize);
bool customConfig();
void shuffleBoard(int boardSize,Grid<char>&board);
void readCustomInput(int boardSize,Grid<char>&board);
bool onlyCharacters(string str);
void humanTurn( Grid<char> &board,Lexicon & humanFoundWord);
bool isValidInput_Human(string word,Lexicon humanFoundWord);
bool isValidInput_Computer(string word,Lexicon humanFoundWord,Lexicon computerFoundWord,Lexicon &englishWords);

bool canBeFormedOnBoard(string word,Grid<char> board);
bool canFindPaths(string word,Grid<char> &board,Point pos);
Point  findAdjacentPoint(int dirction,Point origin );
void cleanDisplay(Grid<char> &board);
void computerTurn(Grid<char> &board,Lexicon & humanFoundWord);
bool findAllValidWords(Grid<char>&board,Point pos,string sofar,Lexicon &englishword,Lexicon &computerFoundWord,Lexicon humanFoundWord);

/* Main program */

int main() {
    int boardSize;
        
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();
    setSize(boardSize);
    bool isCustomConfig=customConfig();
    drawBoard(boardSize,boardSize);
    
    Grid<char> board(boardSize,boardSize);
    
    if(isCustomConfig)  readCustomInput(boardSize,board);
    else shuffleBoard(boardSize,board);
    
    Lexicon  humanFoundWord;
    
    humanTurn(board,humanFoundWord);
    computerTurn(board,humanFoundWord);
    
    
    
    
    
    
    return 0;
    
   
}
/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

// [TODO: Fill in the rest of the code]
void  setSize( int &boardSize){
    string result;
    cout<<endl;
    cout<< "You can choose standard Boggle (4x4 grid)"<< endl;
    cout<<"or Big Boggle (5x5)."<<endl;
    cout<< "Would you like Big Boggle?";
    while(true){
    
        result=getLine();
        result=toLowerCase(result);
        if(result=="yes") {boardSize=5;  break;}
        if(result=="no")  {boardSize=4;  break;}
        cout<< "invalid input. Please enter Yes or No"<<endl;
        
    }
}

bool customConfig(){
    string result;
    cout<<endl;
    cout << "I'll give you a chance to set up the board to your specification, which makes it easier to confirm your boggle program is working."<<endl;
    cout<<"Do you want to force the board configuration?";
    
    while (true){
        result=getLine();
        result=toLowerCase(result);
        if(result=="yes") return true;
        if(result=="no") return false;
        cout<< "invalid input. Please enter Yes or No"<<endl;
        
    }
    
    
    
}

void shuffleBoard(int boardSize,Grid<char> & board){
    
    
    Vector<string> copyCubes;
    
    if(boardSize==4){
        for(string item :STANDARD_CUBES ){
            copyCubes.add(item);
        }
    }
    else{
        for(string item :BIG_BOGGLE_CUBES ){
            copyCubes.add(item);
        }
        
    }
    
    // shuffle cubes position
    for(int i=0;i<copyCubes.size();i++){
        int  randomElementIndex= randomInteger(i,copyCubes.size()-1);
        string temp=copyCubes[i];
        copyCubes[i]=copyCubes[randomElementIndex];
        copyCubes[randomElementIndex]=temp;
        
    }
    
    
    for(int i=0;i<copyCubes.size();i++){
        char randomLetter=copyCubes[i][randomInteger(0, 5)];
        board[i/boardSize][i%boardSize]=randomLetter;
        labelCube(i/boardSize, i%boardSize, randomLetter);
    }
    
    
    
    
}

void readCustomInput(int boardSize,Grid<char>&board){
    int legalLength=boardSize*boardSize;
  
    
    string result;
    cout<<endl;
    cout<<"Enter a "<< legalLength<<" character string to identify which letters you want on the cubes."<<endl;
    cout<<"The first "<< sqrt(legalLength) << " letters are the cubes on the top row from left to right, the next "<<  sqrt(legalLength) <<" letters are the second row, and so on."<<endl;
    cout<<"Enter the string: ";
    
    while(true){
        result=getLine();
        result=toUpperCase(result);
        
        
        
        
        if(result.size()==legalLength&& onlyCharacters(result)){
           
            for( int i=0; i<result.size();i++){
                board[i/boardSize][i%boardSize]=result[i];
                labelCube(i/boardSize,  i%boardSize, result[i]);
            }
            break;
        }
        
        cout<<"String must include "<< legalLength<<" characters! Try again: ";
    }
    
}


bool onlyCharacters(string str){
    
    for(char ch: str){
        
        if(!isalpha(ch)) return false;
    }
    return true;
}

bool isValidInput_Human(string word,Lexicon existedWord ){
    
    Lexicon english("EnglishWords.dat");
    
        
    if(word.size()<4){
        cout<<"I'm sorry, but we have our standards."<<endl;
        cout<<"That word doesn't meet the minimum word length."<<endl;

      return false;
    }
    
    if(!english.contains(word)){
        cout<< "You can't make that word!"<<endl;
        return false;
    }

    if(existedWord.contains(word)){
        cout<<"You've already guessed that!"<<endl;
        return false;
    }
    else   return true;
}


bool canBeFormedOnBoard(string word,Grid<char> board){
    
    
    
    
    
   
    
    for(int i=0;i<board.numRows();i++){
        for(int j=0 ;j<board.numCols();j++){
            if(board[i][j]==word[0]){
                Point pos(i,j);
                
                if(canFindPaths(word,board ,pos)) return true;
                
    
            }
        }
    }
    cout<<"You can't make that word"<<endl;
    return false;
    
}

bool canFindPaths(string word,Grid<char> &board,Point pos){
    
    if(word.size()==1&& word[0]== board[pos.getX()][pos.getY()]){
        
        
        highlightCube(pos.getX(), pos.getY(), true);
        board[pos.getX()][pos.getY()]=' ';
        return true;
        
    }
    
    
    if(word[0]!=board[pos.getX()][pos.getY()]){
        return false;
    }
    
    enum Direction{north,northEast,east, southEast,south,southWest,west,northWest};
    

    bool result=false;
    char tmp=board[pos.getX()][pos.getY()];
    board[pos.getX()][pos.getY()]=' ';     //make choice
    // try all possible choices
    for(int dir=north ;dir<=northWest;dir++){
        Point adjacentPos=findAdjacentPoint(dir,pos);
        if(board.inBounds(adjacentPos.getX(),adjacentPos.getY())){
                
               result=canFindPaths(word.substr(1),board,adjacentPos);
            if(result){
                highlightCube(pos.getX(), pos.getY(), true);
                return true;
            }
        }
            
    }
    if (result==false) {
        board[pos.getX()][pos.getY()]=tmp;     //unmake choice
        return false;
    }
        
        
}


Point  findAdjacentPoint(int direction,Point origin ){
    
    
    if(direction==0){
        Point north(origin.getX()-1,origin.getY());
        return north;
    }
    
    if(direction==1){
        Point northEast(origin.getX()-1,origin.getY()+1);
        return northEast;
    }
    

    if(direction==2){
        Point east(origin.getX(),origin.getY()+1);
        return east;
    }
    

    if(direction==3){
        Point southEast(origin.getX()+1,origin.getY()+1);
        return southEast;
    }
    

    if(direction==4){
        Point south(origin.getX()+1,origin.getY());
        return south;
    }
    

    if(direction==5){
        Point southWest(origin.getX()+1,origin.getY()-1);
        return southWest;
    }
    

    if(direction==6){
        Point west(origin.getX(),origin.getY()-1);
        return west;
    }
    

    if(direction==7){
        Point northWest(origin.getX()-1,origin.getY()-1);
        return northWest;
    }
    
    
    
}


void showMarkedEntry(Grid<char> &board){
    for( int i=0;i<board.numRows();i++){
        for(int j=0;j<board.numCols();j++){
            if(board[i][j]==' '){
                highlightCube(i, j, true);
            }
        }
    }
}


void cleanDisplay(Grid<char> &board){
    for( int i=0;i<board.numRows();i++){
        for(int j=0;j<board.numCols();j++){
            highlightCube(i, j, false);
        }
    }
}

void humanTurn(Grid<char> &board ,Lexicon & humanFoundWord){
  
    
    while(true){
        
        
        
        string word;
        cout<< "Enter a word: ";
        word=getLine();
        if(word=="") break;
        word=toUpperCase(word);
        if(isValidInput_Human(word,humanFoundWord)&&canBeFormedOnBoard(word,board)){
                humanFoundWord.add(word);
                recordWordForPlayer(word, HUMAN);
                pause(500);
                cleanDisplay(board);
            
        }
        
    }
}

void computerTurn(Grid<char> &board,Lexicon & humanFoundWord){
    
    Lexicon english("EnglishWords.dat");
    Lexicon computerFoundWord;
    
    for(int i=0;i<board.numRows();i++){
        for(int j=0;j<board.numCols();j++){
            Point pos(i,j);
            string sofar="";
            sofar+=board[i][j];
            findAllValidWords(board,pos,sofar,english,computerFoundWord,humanFoundWord);
        }
    }
    for(string item:computerFoundWord){
        recordWordForPlayer(item, COMPUTER);
    }
   
}


bool isValidInput_Computer(string sofar,Lexicon humanFoundWord,Lexicon computerFoundWord,Lexicon&englishword){
   
    
    if((!computerFoundWord.contains(sofar))&& !humanFoundWord.contains(sofar)&&englishword.contains(sofar)
       && sofar.size()>=4){
        return true;
    }
    return false;
    
}


bool findAllValidWords(Grid<char>&board,Point pos,string sofar,Lexicon &englishword,Lexicon &computerFoundWord,Lexicon humanFoundWord ){
    
    //simple case
    if(isValidInput_Computer(sofar,humanFoundWord,computerFoundWord,englishword)){
        
        char tmp=board[pos.getX()][pos.getY()];
        board[pos.getX()][pos.getY()]=' ';
        
        showMarkedEntry(board);
        pause(500);
        cleanDisplay(board);
        recordWordForPlayer(sofar, COMPUTER);
        computerFoundWord.add(sofar);
        board[pos.getX()][pos.getY()]=tmp;
        return true;
        
    }
    
    
    //recurring case 
    if(englishword.containsPrefix(sofar)){
        
        enum Direction{north,northEast,east, southEast,south,southWest,west,northWest};
    
        char tmp=board[pos.getX()][pos.getY()];
        board[pos.getX()][pos.getY()]=' ';   // mark selection
        
        for(Direction dir=north;dir<=northWest;dir++){
            Point adjacentPos=findAdjacentPoint(dir,pos);
            if(board.inBounds(adjacentPos.getX(),adjacentPos.getY())){
                if(board[adjacentPos.getX()][adjacentPos.getY()]!=' '){
                string updatedSofar=sofar+board[adjacentPos.getX()][adjacentPos.getY()];
                bool result=findAllValidWords(board,adjacentPos,updatedSofar,englishword,computerFoundWord,humanFoundWord);
                if(result) {
                    findAllValidWords(board, pos, sofar, englishword, computerFoundWord,humanFoundWord);
                    
                }
            }
            }
        }
        
        //unmake choice
        
        board[pos.getX()][pos.getY()]=tmp;
        return false;
        
        
    }
    
    
    
    
    
}


