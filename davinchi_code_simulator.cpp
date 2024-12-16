#include <iostream>
#include <fstream>

using namespace std;

enum class TileColor { WHITE, BLACK };
enum class TileStatus { HIDDEN, REVEALED };

struct Tile {
    int number;
    TileColor color;
    TileStatus status;
};

class Player {
private:
    Tile tiles[13];
    int numTiles = 0;
public:
    void addTile(Tile tile) {
        tiles[numTiles] = tile;
        numTiles++;
    }

    Tile& getTile(int index) {
        if (index < numTiles) return tiles[index];
        else {
            cerr << "out of tile index!" << endl;
            exit(1);
        } 
    }

    int getNumTiles() {
        return numTiles;
    }

    void displayTiles(ofstream& ofs) {
        
        /* DO NOT MODIFY the output format! */

        sortTiles();
        for (size_t i = 0; i < numTiles; i++) {
            ofs << tiles[i].number;
            ofs << (tiles[i].color == TileColor::WHITE ? "w" : "b") << "(";
            ofs << (tiles[i].status == TileStatus::HIDDEN ? "H" : "R") << ")";
            if (i != numTiles - 1) ofs << ", ";
        }
    }

    bool isTileRevealed(int index) {
        if (index < numTiles) return tiles[index].status == TileStatus::REVEALED;
        else {
            cerr << "Out of tile index!" << endl;
            exit(1);
        }
    }

    void sortTiles() {
        Tile temp;
        for(unsigned int i = 0; i < numTiles - 1; i++){
            for(unsigned int j = 0; j < numTiles - 1 -i; j++){
                if(tiles[j].number > tiles[j+1].number){
                    temp = tiles[j];
                    tiles[j] = tiles[j+1];
                    tiles[j+1] = temp;
                }
                else if(tiles[j].number == tiles[j+1].number && tiles[j].color == TileColor::WHITE){
                    temp = tiles[j];
                    tiles[j] = tiles[j+1];
                    tiles[j+1] = temp;
                }
            }
        }
        /* TODO */
    }

    bool allTilesRevealed() {
        for(int i = 0; i < numTiles; i++){
            if(tiles[i].status == TileStatus::HIDDEN){
                return false;
                break;
            }
        }
        return true;
        /* TODO: return true if one's tiles are all revealed */
    }
};

class DaVinciCodeGame {
private:
    Tile centerTiles[24];
    Player players[4];
    int numCenter = 24;
    int numPlayers;
    int currentPlayerIndex;
    int save_num1 = -1, save_num2 = -1;
    
public:
    DaVinciCodeGame(int num) {
        numPlayers = num;
        currentPlayerIndex = 0;
        initialize();
        initializeTiles();
        initializePlayerTiles();
    }

    void initialize() {
        /* If you need, initialize any variables. */

    }

    Tile popCenter() {
        if (numCenter < 0) {
            cerr << "Out of center tile index!" << endl;
            exit(1);
        }
        numCenter--;
        return centerTiles[23-numCenter];
    
        /* TODO: draw one tile from center tiles */
    }   
    int input_num(string A){
        if (A == "1") return 1;
        else if (A == "0") return 0;
        else if (A == "2") return 2;
        else if (A == "3") return 3;
        else if (A == "4") return 4;
        else if (A == "5") return 5;
        else if (A == "6") return 6;
        else if (A == "7") return 7;
        else if (A == "8") return 8;
        else if (A == "9") return 9;
        else if (A == "10") return 10;
        else if (A == "11") return 11;
        else return -1;
    }

    void initializeTiles() {
        ifstream ifs;
        ifs.open("input.txt");
        Tile temp;
        string str1, str1_num, str1_color;
        for(int i = 0; i < 24; i++){
            ifs >> str1;
            str1_num = str1.substr(0,str1.length()-1);
            centerTiles[i].number = input_num(str1_num);
            str1_color = str1.substr(str1.length()-1,str1.length());
            if(str1_color == "w") centerTiles[i].color = TileColor::WHITE;
            else centerTiles[i].color = TileColor::BLACK;
            centerTiles[i].status = TileStatus::HIDDEN;
        }

        /* TODO: initialize tiles with input.txt file */

    }  

    void initializePlayerTiles() {
        for(int i = 0; i < 4; i++){
            players[0].addTile(popCenter());
        }
        for(int i = 0; i < 4; i++){
            players[1].addTile(popCenter());
        }
        players[0].sortTiles();
        players[1].sortTiles();

        /* TODO: make each player draw 4 tiles */

    }
    

    void displayPlayersTiles(ofstream& ofs) {

        /* DO NOT MODIFY the output format! */

        for (int i = 0; i < numPlayers; ++i) {
            ofs << "Player " << i + 1 << "'s Tile : [ ";
            players[i].displayTiles(ofs);
            ofs << " ]";
            ofs << endl;
        }
    }

    bool makeGuess( /* TODO */ Player& targetPlayer, int targetTileIndex, int guess_num) {
        
        /* TODO: check and return if the guess was correct */
        
            if(targetPlayer.getTile(targetTileIndex).number == guess_num){
                targetPlayer.getTile(targetTileIndex).status = TileStatus::REVEALED;
                return true;
            }
            else{
                return false;
            }
        /* if the guess was correct, target tile should be revealed */

    }

    void play() {
        ofstream ofs;
        TileColor clue_color;
        int clue_number;
        bool turnContinue = false;
        int save_num1, save_num2;
        int blackTile1[12] = {0,0,0,0,0,0,0,0,0,0,0}, whiteTile1[12] = {0,0,0,0,0,0,0,0,0,0,0};
        int blackTile2[12] = {0,0,0,0,0,0,0,0,0,0,0}, whiteTile2[12] = {0,0,0,0,0,0,0,0,0,0,0};

        ofs.open("output.txt");
        if (!ofs.is_open()) {
            cerr << "Outout File error!" << endl;
            exit(1);
        }

        /* DO NOT MODIFY the output format! */
        ofs << "Game Start!" << endl;

        while (true) {
            displayPlayersTiles(ofs);   

            // Do not modify the output format
            ofs << "--------------------------------------------------------------------------------------------------" << endl;  
            ofs << "Player " << currentPlayerIndex + 1 << "'s Turn: " << endl;
            Player& currentPlayer = players[currentPlayerIndex];
   
            if (!currentPlayer.allTilesRevealed() && !turnContinue) {  

                /* TODO: make currentPlayer recieve one clue tile */
                currentPlayer.addTile(popCenter());
                clue_number = currentPlayer.getTile(currentPlayer.getNumTiles()-1).number;
                clue_color = currentPlayer.getTile(currentPlayer.getNumTiles()-1).color;
                currentPlayer.sortTiles();
                /* TODO: make currentPlayer recieve one clue tile */

                /* DO NOT MODIFY the output format! */

                ofs << "Clue tile received: " << clue_number << (clue_color == TileColor::WHITE ? "w" : "b" ) << endl;
            }


            int targetPlayerIndex, targetTileIndex, guessedNumber = 0;
            int min_num = 0;// 내가 정의한 변수
            targetPlayerIndex = (currentPlayerIndex + 1) % numPlayers ;
            Player& targetPlayer = players[targetPlayerIndex];

            
            /* TODO: guess number of target tile */         
            int q = 0;
            while(1){
                if(!targetPlayer.isTileRevealed(q)) break;
                else{
                    if(targetPlayer.getTile(q).color == TileColor::BLACK) min_num = targetPlayer.getTile(q).number;
                    else min_num = targetPlayer.getTile(q).number + 1;
                    q++;
                    if(currentPlayerIndex == 0){
                        if(targetPlayer.getTile(q-1).color == TileColor::BLACK) blackTile1[targetPlayer.getTile(q-1).number] = 1;
                        else whiteTile1[targetPlayer.getTile(q-1).number] = 1;
                    }
                    else{
                        if(targetPlayer.getTile(q-1).color == TileColor::BLACK) blackTile2[targetPlayer.getTile(q-1).number] = 1;
                        else whiteTile2[targetPlayer.getTile(q-1).number] = 1;
                    }
                }
            }
            targetTileIndex = q;
            if(currentPlayerIndex == 0){
                for(int i = 0; i < currentPlayer.getNumTiles(); i++){
                    if(currentPlayer.getTile(i).color == TileColor::WHITE) whiteTile1[currentPlayer.getTile(i).number] = 1;
                    else blackTile1[currentPlayer.getTile(i).number] = 1;
                }
            }
            else{
                for(int i = 0; i < currentPlayer.getNumTiles(); i++){
                    if(currentPlayer.getTile(i).color == TileColor::WHITE) whiteTile2[currentPlayer.getTile(i).number] = 1;
                    else blackTile2[currentPlayer.getTile(i).number] = 1;
                }               
            }
            if(currentPlayerIndex == 0){
                while(1){
                    if(targetPlayer.getTile(targetTileIndex).color == TileColor::WHITE){
                        if(whiteTile1[min_num] == 1) min_num++;
                        else break;
                    }
                    else{
                        if(blackTile1[min_num] == 1) min_num++;
                        else break;
                    }
                }
            }
            else{
                while(1){
                    if(targetPlayer.getTile(targetTileIndex).color == TileColor::WHITE){
                        if(whiteTile2[min_num] == 1) min_num++;
                        else break;
                    }
                    else{
                        if(blackTile2[min_num] == 1) min_num++;
                        else break;
                    }
                }
            }

            guessedNumber = min_num;

            if(currentPlayerIndex == 0){
                if(targetPlayer.getTile(targetTileIndex).color == TileColor::WHITE) whiteTile1[guessedNumber] = 1;
                else blackTile1[guessedNumber] = 1;
            }
            if(currentPlayerIndex == 1){
                if(targetPlayer.getTile(targetTileIndex).color == TileColor::WHITE) whiteTile2[guessedNumber] = 1;
                else blackTile2[guessedNumber] = 1;
            }

            /* DO NOT MODIFY the output format! */

            ofs << endl << "Target index is: " << targetTileIndex + 1 << ", Guessed number is: " << guessedNumber << endl;

            if (makeGuess( targetPlayer, targetTileIndex, guessedNumber )) {

                // Do not modify the output format
                if (targetPlayer.allTilesRevealed()) {
                    ofs << "\nCorrect guess!\n" ;
                    ofs << "Player " << targetPlayerIndex + 1 << " has lost!" << endl;
                    ofs << "Player " << currentPlayerIndex + 1 << " is the winner!" << endl;
                    return;
                }
                ofs << "Correct guess! Player " << currentPlayerIndex + 1 << " gets another turn. \n" << endl;
                turnContinue = true;
            } else {
                /* DO NOT MODIFY the output format! */
                 
                ofs << "Incorrect guess! Player " << currentPlayerIndex + 1 << " reveals a clue tile. \n" << endl;
                if (!currentPlayer.allTilesRevealed()) {

                    /* TODO: reveal one tile of currentPlayer */
                
                    int k = 0;
                    while(1){
                        if(currentPlayer.getTile(k).number == clue_number && currentPlayer.getTile(k).color == clue_color){
                            currentPlayer.getTile(k).status = TileStatus::REVEALED;
                            break;
                        }
                        k++;
                    }

                }
                turnContinue = false;
            }

            if (targetPlayer.allTilesRevealed()) {

                /* DO NOT MODIFY the output format! */

                ofs << "Player " << targetPlayerIndex + 1 << " has lost!" << endl;
                ofs << "Player " << currentPlayerIndex + 1 << " is the winner!" << endl;
                return;
            }

            if (!turnContinue) {
                currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
            }
        }
        ofs.close();
    }
};

int main() {
    DaVinciCodeGame game(2);
    game.play();
    return 0;
}