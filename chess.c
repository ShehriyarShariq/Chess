/*
    **READ ME**
    GUIDE FOR VISUAL STUDIO *ONLY*
    INORDER FOR THE CODE TO RUN ON YOUR MACHINE ON VISUAL STUDIO CERTAIN LIBRARIES AND FILES NEED TO BE INCLUDED,
    TO DO SO, FOLLOW THE FOLLOWING STEPS,
    1) OPEN THE PROJECT PROPERTIES FROM 'PROJECT/*PROGECT_NAME* PROPERTIES',
    2) CLICK ON THE 'VC++ DIRECTORIES' TAB,
    3) IN THE 'INCLUDE DIRECTORIES' FIELD, ADD A NEW REFERENCE TO IT,
    3.1) FROM THE OPENED WINDOW BROWSE FOR THE LOCATION OF THE 'include' FOLDER PRESENT IN THE PROJECT DIRECTORY UNDER 'PROJECT_NAME/SDL/include' and select it,
    4) IN THE 'LIBRARY DIRECTORIES' FIELD, ADD A NEW REFERENCE TO IT,
    4.1) FROM THE OPENED WINDOW BROWSE FOR THE LOCATION OF THE 'win32' FOLDER PRESENT IN THE PROJECT DIRECTORY UNDER 'PROJECT_NAME/SDL/lib/win32' and select it,
    5) ON THE SIDEBAR, CLICK ON THE 'Linker' TAB AND THEN CLICK ON THE 'Input' TAB WITHIN IT,
    6) IN THE 'Additional Dependencies' FIELD, EDIT IT AND ADD TO ITS LIST AT THE TOP:
        SDL2.lib
        SDL2main.lib
        SDL2_image.lib
    7) ONCE THIS IS DONE, CLEAN THE PROJECT AND THEN RESTART VISUAL STUDIO JUST IN CASE,
    8) IF SOME ERROR PERSISTS THEN CONTACT ME AT MY EMAIL, 'sshariq.bscs18seecs@nust.edu.pk' or 'shehriyarshariq@gmail.com'
*/

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>

// Constants
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define NUM_OF_BOX_IN_WIDTH 20
#define NUM_OF_BOX_IN_HEIGHT 14

// Texture paths
#define BACKGROUND_IMAGE "images/bg.png"
#define IMAGE_TURN_PLAYER_ONE "images/turn_player_one.png"
#define IMAGE_TURN_PLAYER_TWO "images/turn_player_two.png"

#define IMAGE_SELCTED_BOX "images/highlight_green_box.png"
#define IMAGE_POSSIBLE_MOVES "images/possible_moves_box.png"
#define IMAGE_UNDER_CHECK_BOX "images/undercheck_box.png"
#define IMAGE_PLAYER_ONE_WON "images/player_one_won_screen.png"

#define IMAGE_BLACK_PAWN "images/black_pawn.png"
#define IMAGE_BLACK_KNIGHT "images/black_knight.png"
#define IMAGE_BLACK_ROOK "images/black_rook.png"
#define IMAGE_BLACK_KING "images/black_king.png"
#define IMAGE_BLACK_QUEEN "images/black_queen.png"
#define IMAGE_BLACK_BISHOP "images/black_bishop.png"

#define IMAGE_PAWN "images/pawn.png"
#define IMAGE_KNIGHT "images/knight.png"
#define IMAGE_ROOK "images/rook.png"
#define IMAGE_KING "images/king.png"
#define IMAGE_QUEEN "images/queen.png"
#define IMAGE_BISHOP "images/bishop.png"

// Board Possible Player Pieces
#define PLAYER_ONE_ID 1
#define PLAYER_TWO_ID 2
#define EMPTY 0

// States of the game
#define RUNNING_STATE 0
#define GAME_FINISHED_STATE 1
#define GAME_DRAW_AND_FINISHED 2
#define PLAYER_1_WON_STATE 3
#define PLAYER_2_WON_STATE 4
#define QUIT_STATE = 5


// Global vars
typedef struct { // Structure for the location of a piece
    int x;
    int y;
} pos;

typedef struct possiblePos { // Linked List for the possible position a piece can move to
    pos boxLoc;
    struct possiblePos * next;
} PossiblePositions;

typedef struct {
    int player; // To keep track of current player
    int state; // To keep track of current state of the game
} Game;

Game game; // Instance of the current game
char isBoxSelected = 'F'; // Keeps track of if a box is selected or not
pos selectedBoxPos; // Stores position in array of the last selected piece

char hasPlayer1KingMoved = 'F'; // Check for whether player1's king has moved or not in the entirety of the game
char hasPlayer2KingMoved = 'F'; // Check for whether player2's king has moved or not in the entirety of the game
char hasPlayer1KingSideRookMoved = 'F', hasPlayer1QueenSideRookMoved = 'F'; // For castling, checks if the player1 rooks have moved or not
char hasPlayer2KingSideRookMoved = 'F', hasPlayer2QueenSideRookMoved = 'F'; // For castling, checks if the player2 rooks have moved or not
char pawnUpgradeToQueen = 'Q'; // Pawn upgrade option. Only queen cuz don't wanna make the UI for the choosing option
// Points, P=1, R=5, H=3, Q=8, B=3

char playerPieces[8][8] = {' '}; // To store the position of the pieces on the board

// Original state of the board
char resetToThis[8][8] = { {'R', 'H', 'B', 'K', 'Q', 'B', 'H', 'R'},
                            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                            {'R', 'H', 'B', 'K', 'Q', 'B', 'H', 'R'} };

int chessBoard[8][8] = {EMPTY}; // To store the playerID of the pieces on the board

int playerOneTakenOutPieces[6] = {0}; // Stores the taken out pieces for player1
int playerTwoTakenOutPieces[6] = {0}; // Stores the taken out pieces for player2

int movedDouble[8][8] = { 0 }; // To store at which position a pawn moved double 

//int checkPiecesStatus = 0;

// Method prototypes
// GUI methods
pos getBoxCD(int row, int column, int playerID);
PossiblePositions * getPossibleMovesRowAndColumn(pos currPos, int playerID, char typeOfPiece, int BOX_HEIGHT, int BOX_WIDTH); // Gets the possibleMoves window position
char listContains(PossiblePositions *possiblePositions, pos searchFor); // Check if the clicked position is present in the possible moves list
void changePlayerTurn(); // Changes turn
void getTakenOutPieces(int playerID); // Updates the array of taken out pieces 
void removePossiblePosition(PossiblePositions * possiblePositions, pos posToRemove); // Remove positions that are checked

// Pieces method headers
void king(int moveToX, int moveToY, int playerID); // Piece: KING
void queen(int currX, int currY, int moveToX, int moveToY, int playerID); // Piece: QUEEN
void bishop(int currX, int currY, int moveToX, int moveToY, int playerID); // Piece: BISHOP
void rook(int currX, int currY, int moveToX, int moveToY, int playerID); // Piece: ROOK
void knight(int currX, int currY, int moveToX, int moveToY, int playerID); // Piece: KNIGHT
void pawn(int currX, int currY, int moveToX, int moveToY, int playerID); // Piece: PAWN

// Piece logic checker method headers
void pieceMovement(int playerID, char pieceType, pos currPos, pos newPos); // Visual update of the position of a piece
char legalMoveChecker(int playerID, char typeOfPiece, pos currPos, pos moveToPos); // Checks if a move is legal for a piece
PossiblePositions* getAllPossibleMoves(int playerID, char typeOfPiece, pos currPos); // Gives the list of all possible moves for a piece
pos getKingPos(int playerID); // Gets the position of a player's king
char checkForCheck(int playerID, pos posToCheckFor); // Check if a player's king is under check
char isCheckmate(int playerID); // Checks if a player's king is checkmated
char isGameDraw(); // Check game draw status
char isInStaleMate(int playerID);
char isInPrimaryDiag(pos inDiagOf, pos searchFor); // Checks if 'searchFor' is in the primary diagonal of 'inDiagOf' 
char isInSecondaryDiag(pos inDiagOf, pos searchFor); // Checks if 'searchFor' is in the secondary diagonal of 'inDiagOf'
char boundaryCheck(pos boxLoc); // Checks if a box is within the bounds of the board
char areSameBoxes(pos pos1, pos pos2); // Checks if two boxes are the same or not

// Utility method headers
int getAbsVal(int num); // Returns the abs value of a number
void flipBoard(); // Flips the board arrays
void gameInit(); // Initializes the game
void freeLinkedList(PossiblePositions * list); // Free the allocated memory

int main(int argc, char *args[]) {

    gameInit(); // Initializes the game

    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // Initializes SDL2, if fails then exits
        return EXIT_FAILURE;
    }

    IMG_Init(IMG_INIT_PNG); // Initialize Image loading thorugh SDL

    // Game Window of Size 1000 x 700
    SDL_Window *window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) { // If window failed to initialize then exit
        return EXIT_FAILURE;
    }

    // Initializes the renderer, the renderer is what allows for stuff to be drawn on then window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) { // If renderer failed to initialize then exit 
        SDL_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    // The entire window is divided into squares of dimension 50x50px
    const BOX_WIDTH = SCREEN_WIDTH / NUM_OF_BOX_IN_WIDTH; // Width of a single box => 1000 / 20 => 50px wide
    const BOX_HEIGHT = SCREEN_HEIGHT / NUM_OF_BOX_IN_HEIGHT; // Height of a single box => 700 / 14 => 50px long
    
    // Textures to be loaded in the game
    SDL_Texture *background = IMG_LoadTexture(renderer, BACKGROUND_IMAGE);
    SDL_Texture *turnPlayerOne = IMG_LoadTexture(renderer, IMAGE_TURN_PLAYER_ONE);
    SDL_Texture *turnPlayerTwo = IMG_LoadTexture(renderer, IMAGE_TURN_PLAYER_TWO);

    SDL_Texture *selectedBox = IMG_LoadTexture(renderer, IMAGE_SELCTED_BOX);
    SDL_Texture *possibleMoveLoc = IMG_LoadTexture(renderer, IMAGE_POSSIBLE_MOVES);
    SDL_Texture *underCheckBox = IMG_LoadTexture(renderer, IMAGE_UNDER_CHECK_BOX);
    SDL_Texture *playerOneWonBox = IMG_LoadTexture(renderer, IMAGE_PLAYER_ONE_WON);

    SDL_Texture *pawnTexture = IMG_LoadTexture(renderer, IMAGE_PAWN);
    SDL_Texture *blackPawnTexture = IMG_LoadTexture(renderer, IMAGE_BLACK_PAWN);
    SDL_Texture *rookTexture = IMG_LoadTexture(renderer, IMAGE_ROOK);
    SDL_Texture *blackRookTexture = IMG_LoadTexture(renderer, IMAGE_BLACK_ROOK);
    SDL_Texture *knightTexture = IMG_LoadTexture(renderer, IMAGE_KNIGHT);
    SDL_Texture *blackKnightTexture = IMG_LoadTexture(renderer, IMAGE_BLACK_KNIGHT);
    SDL_Texture *kingTexture = IMG_LoadTexture(renderer, IMAGE_KING);
    SDL_Texture *blackKingTexture = IMG_LoadTexture(renderer, IMAGE_BLACK_KING);
    SDL_Texture *queenTexture = IMG_LoadTexture(renderer, IMAGE_QUEEN);
    SDL_Texture *blackQueenTexture = IMG_LoadTexture(renderer, IMAGE_BLACK_QUEEN);
    SDL_Texture *bishopTexture = IMG_LoadTexture(renderer, IMAGE_BISHOP);
    SDL_Texture *blackBishopTexture = IMG_LoadTexture(renderer, IMAGE_BLACK_BISHOP);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Draws the window with a white background
    SDL_RenderClear(renderer); // Clears the renderer of any existing elements

    SDL_Rect bgDest; // Position of bg
    bgDest.x = 0;
    bgDest.y = 0;

    SDL_QueryTexture(background, NULL, NULL, &bgDest.w, &bgDest.h);
    SDL_RenderCopy(renderer, background, NULL, &bgDest); // Draws the background texture

    SDL_Event e; // Event, anything such as a mouse click or window close
    char quit = 'F';
    while (quit != 'T') {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { // On window close
                quit = 'T';
            } else if(e.type == SDL_MOUSEBUTTONDOWN) { // For any mouse button down       
                int row = (e.button.y / BOX_HEIGHT);
                int column = (e.button.x / BOX_WIDTH);

                if (game.state == RUNNING_STATE) { // While game is in running state                    
                    pos boxClicked = getBoxCD(row, column, game.player); // Position of the box clicked in terms of the array position, might be < 0 or > 7
                    char pieceType = playerPieces[boxClicked.y][boxClicked.x]; // Type of the piece at the clicked position
                    
                    // Get all possible moves of the clicked box
                    PossiblePositions * allPossibleMovesRowAndColumn = getPossibleMovesRowAndColumn(boxClicked, game.player, pieceType, BOX_HEIGHT, BOX_WIDTH); 
                    
                    // If by moving a piece, the king gets under check then the move is invalid, hence to be removed from the list of possible moves
                    if (chessBoard[boxClicked.y][boxClicked.x] == game.player) {
                        PossiblePositions * temp = allPossibleMovesRowAndColumn;
                        while (temp != NULL) {
                            pos possiblePosition;
                            possiblePosition.x = (temp->boxLoc.x / BOX_WIDTH) - 6;
                            possiblePosition.y = (temp->boxLoc.y / BOX_HEIGHT) - 3;
                            
                            char pieceAtNewPos = playerPieces[possiblePosition.y][possiblePosition.x];
                            int pieceIdAtNewPos = chessBoard[possiblePosition.y][possiblePosition.x];

                            // Move the piece on the board temporarily
                            playerPieces[possiblePosition.y][possiblePosition.x] = pieceType;
                            playerPieces[boxClicked.y][boxClicked.x] = ' ';
                            chessBoard[possiblePosition.y][possiblePosition.x] = game.player;
                            chessBoard[boxClicked.y][boxClicked.x] = EMPTY;

                            pos kingCurrPos = getKingPos(game.player);

                            if (checkForCheck(game.player, kingCurrPos) == 'T') { // If king is under check when the piece moves
                                // Make the pos of the possible move out of bounds hence not shown
                                temp->boxLoc.x = 0;
                                temp->boxLoc.y = 0;
                            }

                            // Reset the board back to the previous state before moving the piece
                            playerPieces[possiblePosition.y][possiblePosition.x] = pieceAtNewPos;
                            playerPieces[boxClicked.y][boxClicked.x] = pieceType;
                            chessBoard[possiblePosition.y][possiblePosition.x] = pieceIdAtNewPos;
                            chessBoard[boxClicked.y][boxClicked.x] = game.player;

                            temp = temp->next;
                        }
                    }

                    if (pieceType == 'K' && areSameBoxes(boxClicked, getKingPos(game.player)) == 'T') { // If player clicks on his own king
                        PossiblePositions * temp = allPossibleMovesRowAndColumn;
                        pos kingOriginalPos = getKingPos(game.player);

                        // For removing a checked position as a possible move
                        while (temp != NULL) {
                            if (temp->boxLoc.x == 0 ) { // Skip past positions that have already been deemed incorrect
                                temp = temp->next;
                                continue;
                            }

                            pos possiblePosition;
                            possiblePosition.x = (temp->boxLoc.x / BOX_WIDTH) - 6;
                            possiblePosition.y = (temp->boxLoc.y / BOX_HEIGHT) - 3;

                            char pieceAtNewPos = playerPieces[possiblePosition.y][possiblePosition.x];
                            int playerIdAtNewPos = chessBoard[possiblePosition.y][possiblePosition.x];
                                
                            // Move the king on the board temporarily
                            playerPieces[kingOriginalPos.y][kingOriginalPos.x] = ' ';
                            playerPieces[possiblePosition.y][possiblePosition.x] = 'K';
                            chessBoard[kingOriginalPos.y][kingOriginalPos.x] = EMPTY;
                            chessBoard[possiblePosition.y][possiblePosition.x] = game.player;                       

                            pos kingCurrPos = getKingPos(game.player);

                            if (checkForCheck(game.player, kingCurrPos) == 'T') { // If king is under check when the king moves to the new pos
                                // Make the pos of the possible move out of bounds hence not shown
                                temp->boxLoc.x = 0;
                                temp->boxLoc.y = 0;
                            }

                            // Reset the board back to the previous state before moving the king
                            playerPieces[kingOriginalPos.y][kingOriginalPos.x] = 'K';
                            playerPieces[possiblePosition.y][possiblePosition.x] = pieceAtNewPos;
                            chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                            chessBoard[possiblePosition.y][possiblePosition.x] = playerIdAtNewPos;
                            
                            temp = temp->next;
                        }

                        // FOR 'CASTLING'
                        PossiblePositions * castlingTemp = allPossibleMovesRowAndColumn;
                        // Status of current player's king
                        char hasKingMoved = game.player == PLAYER_ONE_ID ? hasPlayer1KingMoved : hasPlayer2KingMoved; 
                        // Status of current player's king-side rook
                        char hasKingSideRookMoved = game.player == PLAYER_ONE_ID ? hasPlayer1KingSideRookMoved : hasPlayer2KingSideRookMoved;
                        // Status of current player's queen-side rook
                        char hasQueenSideRookMoved = game.player == PLAYER_ONE_ID ? hasPlayer1QueenSideRookMoved : hasPlayer2QueenSideRookMoved;
                        
                        char isKingSideRookDead = 'T';
                        char isQueenSideRookDead = 'T';

                        if (game.player == PLAYER_ONE_ID) { // Dead check
                            if ((chessBoard[0][0] == PLAYER_ONE_ID) && (playerPieces[0][0] == 'R')) {
                                isKingSideRookDead = 'F';
                            }

                            if ((chessBoard[0][7] == PLAYER_ONE_ID) && (playerPieces[0][7] == 'R')) {
                                isQueenSideRookDead = 'F';
                            }
                        } else {
                            if ((chessBoard[7][0] == PLAYER_ONE_ID) && (playerPieces[7][0] == 'R')) {
                                isKingSideRookDead = 'F';
                            }

                            if ((chessBoard[7][7] == PLAYER_ONE_ID) && (playerPieces[7][7] == 'R')) {
                                isQueenSideRookDead = 'F';
                            }
                        }

                        // For King Side Castling
                        pos kingPos = getKingPos(game.player);
                        if (hasKingMoved == 'F' && hasKingSideRookMoved == 'F' && isKingSideRookDead == 'F' && checkForCheck(game.player, kingPos) == 'F') {
                            char canCastleKingSide = 'T';
                            for (int i = (kingOriginalPos.x - 1); i >= (kingOriginalPos.x - 2); i--) { // For 2 positions Left
                                if (playerPieces[kingOriginalPos.y][i] != ' ') { // If not an empty space
                                    canCastleKingSide = 'F';
                                    break;
                                } else if (playerPieces[kingOriginalPos.y][i] == ' ') {
                                    // Move the king on the board temporarily
                                    playerPieces[kingOriginalPos.y][kingOriginalPos.x] = ' ';
                                    playerPieces[kingOriginalPos.y][i] = pieceType;
                                    chessBoard[kingOriginalPos.y][kingOriginalPos.x] = EMPTY;
                                    chessBoard[kingOriginalPos.y][i] = game.player;

                                    pos kingCurrPos = getKingPos(game.player);

                                    if (checkForCheck(game.player, kingCurrPos) == 'T') { // If is getting checked 
                                        // Reset
                                        canCastleKingSide = 'F';
                                        playerPieces[kingOriginalPos.y][kingOriginalPos.x] = pieceType;
                                        playerPieces[kingOriginalPos.y][i] = ' ';
                                        chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                        chessBoard[kingOriginalPos.y][i] = EMPTY;
                                        break;
                                    }

                                    // Reset
                                    playerPieces[kingOriginalPos.y][kingOriginalPos.x] = pieceType;
                                    playerPieces[kingOriginalPos.y][i] = ' ';
                                    chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                    chessBoard[kingOriginalPos.y][i] = EMPTY;
                                }
                            }

                            if (canCastleKingSide == 'T') { // Add castling as part of the legal move then
                                PossiblePositions * current = castlingTemp;
                                while (current->next != NULL) {
                                    current = current->next;
                                }

                                current->next = malloc(sizeof(PossiblePositions));
                                current->next->boxLoc.x = ((kingOriginalPos.x - 2) + 6) * BOX_WIDTH;
                                current->next->boxLoc.y = (kingOriginalPos.y + 3) * BOX_HEIGHT;
                                current->next->next = NULL;
                            }
                        }

                        // For Queen Side Castling
                        if (hasKingMoved == 'F' && hasQueenSideRookMoved == 'F' && isQueenSideRookDead == 'F' && checkForCheck(game.player, kingPos) == 'F') {
                            char canCastleQueenSide = 'T';
                            for (int i = (kingOriginalPos.x + 1); i <= (kingOriginalPos.x + 2); i++) { // For 2 positions right
                                if (playerPieces[kingOriginalPos.y][i] != ' ') { // If not an empty space
                                    canCastleQueenSide = 'F';
                                    break;
                                } else if (playerPieces[kingOriginalPos.y][i] == ' ') {
                                    // Move the king on the board temporarily
                                    playerPieces[kingOriginalPos.y][kingOriginalPos.x] = ' ';
                                    playerPieces[kingOriginalPos.y][i] = pieceType;
                                    chessBoard[kingOriginalPos.y][kingOriginalPos.x] = EMPTY;
                                    chessBoard[kingOriginalPos.y][i] = game.player;

                                    pos kingCurrPos = getKingPos(game.player);

                                    if (checkForCheck(game.player, kingCurrPos) == 'T') { // If is getting checked 
                                        // Reset
                                        canCastleQueenSide = 'F';
                                        playerPieces[kingOriginalPos.y][kingOriginalPos.x] = pieceType;
                                        playerPieces[kingOriginalPos.y][i] = ' ';
                                        chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                        chessBoard[kingOriginalPos.y][i] = EMPTY;
                                        break;
                                    }

                                    // Reset
                                    playerPieces[kingOriginalPos.y][kingOriginalPos.x] = pieceType;
                                    playerPieces[kingOriginalPos.y][i] = ' ';
                                    chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                    chessBoard[kingOriginalPos.y][i] = EMPTY;
                                }
                            }

                            if (canCastleQueenSide == 'T') { // Add castling as part of the legal move then
                                PossiblePositions * current = castlingTemp;
                                while (current->next != NULL) {
                                    current = current->next;
                                }

                                current->next = malloc(sizeof(PossiblePositions));
                                current->next->boxLoc.x = ((kingOriginalPos.x + 2) + 6) * BOX_WIDTH;
                                current->next->boxLoc.y = (kingOriginalPos.y + 3) * BOX_HEIGHT;
                                current->next->next = NULL;
                            }


                        }

                    }

                    if (isBoxSelected == 'T') { // If a box was previously selected
                        // Get all possible moves of the selected piece
                        PossiblePositions * selectedBoxPossiblePositions = getPossibleMovesRowAndColumn(selectedBoxPos, game.player, playerPieces[selectedBoxPos.y][selectedBoxPos.x], BOX_HEIGHT, BOX_WIDTH);

                        if (areSameBoxes(boxClicked, selectedBoxPos) == 'F') { // If the selected box isn't clicked again
                            char typeOfPiece = playerPieces[selectedBoxPos.y][selectedBoxPos.x];
                            
                            // If by moving a piece, the king gets under check then the move is invalid, hence to be removed from the list of possible moves
                            if (chessBoard[selectedBoxPos.y][selectedBoxPos.x] == game.player) {
                                PossiblePositions * temp = selectedBoxPossiblePositions;
                                while (temp != NULL) {
                                    pos possiblePosition;
                                    possiblePosition.x = (temp->boxLoc.x / BOX_WIDTH) - 6;
                                    possiblePosition.y = (temp->boxLoc.y / BOX_HEIGHT) - 3;

                                    char pieceAtNewPos = playerPieces[possiblePosition.y][possiblePosition.x];
                                    int pieceIdAtNewPos = chessBoard[possiblePosition.y][possiblePosition.x];
                                    
                                    // Move the piece on the board temporarily
                                    playerPieces[possiblePosition.y][possiblePosition.x] = typeOfPiece;
                                    playerPieces[selectedBoxPos.y][selectedBoxPos.x] = ' ';
                                    chessBoard[possiblePosition.y][possiblePosition.x] = game.player;
                                    chessBoard[selectedBoxPos.y][selectedBoxPos.x] = EMPTY;

                                    pos kingCurrPos = getKingPos(game.player);

                                    if (checkForCheck(game.player, kingCurrPos) == 'T') { // If king is under check when the king moves to the new pos
                                        // Make the pos of the possible move out of bounds hence not shown
                                        temp->boxLoc.x = 0;
                                        temp->boxLoc.y = 0;
                                    }

                                    // Reset the board back to the previous state before moving the piece
                                    playerPieces[possiblePosition.y][possiblePosition.x] = pieceAtNewPos;
                                    playerPieces[selectedBoxPos.y][selectedBoxPos.x] = typeOfPiece;
                                    chessBoard[possiblePosition.y][possiblePosition.x] = pieceIdAtNewPos;
                                    chessBoard[selectedBoxPos.y][selectedBoxPos.x] = game.player;

                                    temp = temp->next;
                                }
                            }
                            
                            if (typeOfPiece == 'K') { // For king
                                PossiblePositions * temp = selectedBoxPossiblePositions;
                                pos kingOriginalPos = getKingPos(game.player);
                                
                                // For removing a checked position as a possible move
                                while (temp != NULL) {
                                    if (temp->boxLoc.x == 0) { // Skip past positions that have already been deemed incorrect
                                        temp = temp->next;
                                        continue;
                                    }

                                    pos possiblePosition;
                                    possiblePosition.x = (temp->boxLoc.x / BOX_WIDTH) - 6;
                                    possiblePosition.y = (temp->boxLoc.y / BOX_HEIGHT) - 3;

                                    char pieceAtNewPos = playerPieces[possiblePosition.y][possiblePosition.x];
                                    int playerIdAtNewPos = chessBoard[possiblePosition.y][possiblePosition.x];

                                    // Move the king on the board temporarily
                                    playerPieces[kingOriginalPos.y][kingOriginalPos.x] = ' ';
                                    playerPieces[possiblePosition.y][possiblePosition.x] = typeOfPiece;
                                    chessBoard[kingOriginalPos.y][kingOriginalPos.x] = EMPTY;
                                    chessBoard[possiblePosition.y][possiblePosition.x] = game.player;

                                    pos kingCurrPos = getKingPos(game.player);

                                    if (checkForCheck(game.player, kingCurrPos) == 'T') { // If king is under check when the king moves to the new pos
                                        // Make the pos of the possible move out of bounds hence not shown
                                        temp->boxLoc.x = 0;
                                        temp->boxLoc.y = 0;
                                    }

                                    // Reset the board back to the previous state before moving the piece
                                    playerPieces[kingOriginalPos.y][kingOriginalPos.x] = typeOfPiece;
                                    playerPieces[possiblePosition.y][possiblePosition.x] = pieceAtNewPos;
                                    chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                    chessBoard[possiblePosition.y][possiblePosition.x] = playerIdAtNewPos;
                                    
                                    temp = temp->next;
                                }

                                // FOR 'CASTLING'
                                PossiblePositions * castlingTemp = selectedBoxPossiblePositions;
                                // Status of current player's king
                                char hasKingMoved = game.player == PLAYER_ONE_ID ? hasPlayer1KingMoved : hasPlayer2KingMoved;
                                // Status of current player's king-side rook
                                char hasKingSideRookMoved = game.player == PLAYER_ONE_ID ? hasPlayer1KingSideRookMoved : hasPlayer2KingSideRookMoved;
                                // Status of current player's queen-side rook
                                char hasQueenSideRookMoved = game.player == PLAYER_ONE_ID ? hasPlayer1QueenSideRookMoved : hasPlayer2QueenSideRookMoved;
                                
                                char isKingSideRookDead = 'T';
                                char isQueenSideRookDead = 'T';

                                if (game.player == PLAYER_ONE_ID) { // Dead check
                                    if ((chessBoard[0][0] == PLAYER_ONE_ID) && (playerPieces[0][0] == 'R')) {
                                        isKingSideRookDead = 'F';
                                    }

                                    if ((chessBoard[0][7] == PLAYER_ONE_ID) && (playerPieces[0][7] == 'R')) {
                                        isQueenSideRookDead = 'F';
                                    }
                                } else {
                                    if ((chessBoard[7][0] == PLAYER_ONE_ID) && (playerPieces[7][0] == 'R')) {
                                        isKingSideRookDead = 'F';
                                    }

                                    if ((chessBoard[7][7] == PLAYER_ONE_ID) && (playerPieces[7][7] == 'R')) {
                                        isQueenSideRookDead = 'F';
                                    }
                                }

                                // For King Side Castling
                                pos kingPos = getKingPos(game.player);
                                if (hasKingMoved == 'F' && hasKingSideRookMoved == 'F' && isKingSideRookDead == 'F' && checkForCheck(game.player, kingPos) == 'F') {
                                    char canCastleKingSide = 'T';
                                    for (int i = (kingOriginalPos.x - 1); i >= (kingOriginalPos.x - 2); i--) { // For 2 positions Left
                                        if (playerPieces[kingOriginalPos.y][i] != ' ') {
                                            canCastleKingSide = 'F';
                                            break;
                                        } else if (playerPieces[kingOriginalPos.y][i] == ' ') {
                                            // Move the king on the board temporarily
                                            playerPieces[kingOriginalPos.y][kingOriginalPos.x] = ' ';
                                            playerPieces[kingOriginalPos.y][i] = typeOfPiece;
                                            chessBoard[kingOriginalPos.y][kingOriginalPos.x] = EMPTY;
                                            chessBoard[kingOriginalPos.y][i] = game.player;

                                            pos kingCurrPos = getKingPos(game.player);

                                            if (checkForCheck(game.player, kingCurrPos) == 'T') { // If is getting checked 
                                                // Reset
                                                canCastleKingSide = 'F';
                                                playerPieces[kingOriginalPos.y][kingOriginalPos.x] = typeOfPiece;
                                                playerPieces[kingOriginalPos.y][i] = ' ';
                                                chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                                chessBoard[kingOriginalPos.y][i] = EMPTY;
                                                break;
                                            }

                                            // Reset
                                            playerPieces[kingOriginalPos.y][kingOriginalPos.x] = typeOfPiece;
                                            playerPieces[kingOriginalPos.y][i] = ' ';
                                            chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                            chessBoard[kingOriginalPos.y][i] = EMPTY;
                                        }
                                    }

                                    if (canCastleKingSide == 'T') { // Add castling as part of the legal move then
                                        PossiblePositions * current = castlingTemp;
                                        while (current->next != NULL) {
                                            current = current->next;
                                        }

                                        current->next = malloc(sizeof(PossiblePositions));
                                        current->next->boxLoc.x = ((kingOriginalPos.x - 2) + 6) * BOX_WIDTH;
                                        current->next->boxLoc.y = (kingOriginalPos.y + 3) * BOX_HEIGHT;
                                        current->next->next = NULL;
                                    }
                                }

                                // For Queen Side Castling
                                if (hasKingMoved == 'F' && hasQueenSideRookMoved == 'F' && isQueenSideRookDead == 'F' && checkForCheck(game.player, kingPos) == 'F') {
                                    char canCastleQueenSide = 'T';
                                    for (int i = (kingOriginalPos.x + 1); i <= (kingOriginalPos.x + 2); i++) { // For 2 positions right
                                        if (playerPieces[kingOriginalPos.y][i] != ' ') { // If not an empty space
                                            canCastleQueenSide = 'F';
                                            break;
                                        } else if (playerPieces[kingOriginalPos.y][i] == ' ') {
                                            // Move the king on the board temporarily
                                            playerPieces[kingOriginalPos.y][kingOriginalPos.x] = ' ';
                                            playerPieces[kingOriginalPos.y][i] = typeOfPiece;
                                            chessBoard[kingOriginalPos.y][kingOriginalPos.x] = EMPTY;
                                            chessBoard[kingOriginalPos.y][i] = game.player;

                                            pos kingCurrPos = getKingPos(game.player);

                                            if (checkForCheck(game.player, kingCurrPos) == 'T') { // If is getting checked 
                                                // Reset
                                                canCastleQueenSide = 'F';
                                                playerPieces[kingOriginalPos.y][kingOriginalPos.x] = typeOfPiece;
                                                playerPieces[kingOriginalPos.y][i] = ' ';
                                                chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                                chessBoard[kingOriginalPos.y][i] = EMPTY;
                                                break;
                                            }

                                            // Reset
                                            playerPieces[kingOriginalPos.y][kingOriginalPos.x] = typeOfPiece;
                                            playerPieces[kingOriginalPos.y][i] = ' ';
                                            chessBoard[kingOriginalPos.y][kingOriginalPos.x] = game.player;
                                            chessBoard[kingOriginalPos.y][i] = EMPTY;
                                        }
                                    }

                                    if (canCastleQueenSide == 'T') { // Add castling as part of the legal move then
                                        PossiblePositions * current = castlingTemp;
                                        while (current->next != NULL) {
                                            current = current->next;
                                        }

                                        current->next = malloc(sizeof(PossiblePositions));
                                        current->next->boxLoc.x = ((kingOriginalPos.x + 2) + 6) * BOX_WIDTH;
                                        current->next->boxLoc.y = (kingOriginalPos.y + 3) * BOX_HEIGHT;
                                        current->next->next = NULL;
                                    }
                                }
                            }


                            if (listContains(selectedBoxPossiblePositions, boxClicked) == 'T') { // If clicked at a possible position
                                // Piece movement on board
                                if (typeOfPiece == 'P') {
                                    if (game.player == PLAYER_ONE_ID) {
                                        flipBoard();
                                        pawn(7 - selectedBoxPos.x, 7 - selectedBoxPos.y, 7 - boxClicked.x, 7 - boxClicked.y, PLAYER_ONE_ID);
                                        flipBoard();
                                    } else {
                                        pawn(selectedBoxPos.x, selectedBoxPos.y, boxClicked.x, boxClicked.y, PLAYER_TWO_ID);
                                    }
                                } else if (typeOfPiece == 'R') {
                                    if (game.player == PLAYER_ONE_ID) {
                                        flipBoard();
                                        rook(7 - selectedBoxPos.x, 7 - selectedBoxPos.y, 7 - boxClicked.x, 7 - boxClicked.y, PLAYER_ONE_ID);
                                        flipBoard();
                                    } else {
                                        rook(selectedBoxPos.x, selectedBoxPos.y, boxClicked.x, boxClicked.y, PLAYER_TWO_ID);
                                    }
                                } else if (typeOfPiece == 'H') {
                                    if (game.player == PLAYER_ONE_ID) {
                                        flipBoard();
                                        knight(7 - selectedBoxPos.x, 7 - selectedBoxPos.y, 7 - boxClicked.x, 7 - boxClicked.y, PLAYER_ONE_ID);
                                        flipBoard();
                                    } else {
                                        knight(selectedBoxPos.x, selectedBoxPos.y, boxClicked.x, boxClicked.y, PLAYER_TWO_ID);
                                    }
                                } else if (typeOfPiece == 'K') {
                                    if (game.player == PLAYER_ONE_ID) {
                                        flipBoard();
                                        king(7 - boxClicked.x, 7 - boxClicked.y, PLAYER_ONE_ID);
                                        
                                        if (selectedBoxPos.x - boxClicked.x == 2) {
                                            rook(7, 7, 7 - (boxClicked.x + 1), 7 - boxClicked.y, PLAYER_ONE_ID);
                                        } else if (boxClicked.x - selectedBoxPos.x == 2) {
                                            rook(0, 7, 7 - (boxClicked.x - 1), 7 - boxClicked.y, PLAYER_ONE_ID);
                                        }

                                        flipBoard();
                                    } else {
                                        king(boxClicked.x, boxClicked.y, PLAYER_TWO_ID);

                                        if (selectedBoxPos.x - boxClicked.x == 2) {
                                            rook(0, 7, boxClicked.x + 1, boxClicked.y, PLAYER_TWO_ID);
                                        } else if (boxClicked.x - selectedBoxPos.x == 2) {
                                            rook(7, 7, boxClicked.x - 1, boxClicked.y, PLAYER_TWO_ID);
                                        }
                                    }
                                } else if (typeOfPiece == 'Q') {
                                    if (game.player == PLAYER_ONE_ID) {
                                        flipBoard();
                                        queen(7 - selectedBoxPos.x, 7 - selectedBoxPos.y, 7 - boxClicked.x, 7 - boxClicked.y, PLAYER_ONE_ID);
                                        flipBoard();
                                    } else {
                                        queen(selectedBoxPos.x, selectedBoxPos.y, boxClicked.x, boxClicked.y, PLAYER_TWO_ID);
                                    }
                                } else if (typeOfPiece == 'B') {
                                    if (game.player == PLAYER_ONE_ID) {
                                        flipBoard();
                                        bishop(7 - selectedBoxPos.x, 7 - selectedBoxPos.y, 7 - boxClicked.x, 7 - boxClicked.y, PLAYER_ONE_ID);
                                        flipBoard();
                                    } else {
                                        bishop(selectedBoxPos.x, selectedBoxPos.y, boxClicked.x, boxClicked.y, PLAYER_TWO_ID);
                                    }
                                }

                                changePlayerTurn(); // Change turn

                                // Game end check
                                char checkMate = isCheckmate(game.player);
                                char isDraw = isGameDraw();
                                char isStaleMate = isInStaleMate(game.player);

                                if (checkMate == 'T') {
                                    printf("CHECKMATE\n");
                                    if (game.player == PLAYER_ONE_ID) {
                                        printf("Black WON!\n");
                                    } else {
                                        printf("White WON!\n");
                                    }
                                                                        
                                    gameInit();
                                } else if (isDraw == 'T' || isStaleMate == 'T') {
                                    printf("DRAW!");
                                    gameInit();
                                }

                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);

                                // Redraw the background as SDL_RenderClear clears everything
                                // Needs to be cleared to update the board visually
                                SDL_Rect bgDest;
                                bgDest.x = 0;
                                bgDest.y = 0;

                                SDL_QueryTexture(background, NULL, NULL, &bgDest.w, &bgDest.h);
                                SDL_RenderCopy(renderer, background, NULL, &bgDest);
                            } else { // Deselects the previously selected box
                                isBoxSelected = 'F';
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);

                                // Redraw the background as SDL_RenderClear clears everything
                                // Needs to be cleared to update the board visually
                                SDL_Rect bgDest;
                                bgDest.x = 0;
                                bgDest.y = 0;

                                SDL_QueryTexture(background, NULL, NULL, &bgDest.w, &bgDest.h);
                                SDL_RenderCopy(renderer, background, NULL, &bgDest);
                            }
                        } else { // Same boc clicked hence deselect it
                            isBoxSelected = 'F';
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderClear(renderer);
                            
                            // Redraw the background as SDL_RenderClear clears everything
                            // Needs to be cleared to update the board visually
                            SDL_Rect bgDest;
                            bgDest.x = 0;
                            bgDest.y = 0;

                            SDL_QueryTexture(background, NULL, NULL, &bgDest.w, &bgDest.h);
                            SDL_RenderCopy(renderer, background, NULL, &bgDest);
                        }
                    }

                    if (boundaryCheck(boxClicked) == 'T') { // If clicked inside the bounds of the board
                        // Check if player clicked on his piece and is not the same as the last selected piece
                        if (chessBoard[boxClicked.y][boxClicked.x] == game.player && areSameBoxes(boxClicked, selectedBoxPos) == 'F') {
                            isBoxSelected = 'T';
                            selectedBoxPos = boxClicked;

                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderClear(renderer);

                            // Redraw the background as renderer was cleared
                            SDL_Rect bgDest;
                            bgDest.x = 0;
                            bgDest.y = 0;

                            SDL_QueryTexture(background, NULL, NULL, &bgDest.w, &bgDest.h);
                            SDL_RenderCopy(renderer, background, NULL, &bgDest);

                            // Show clicked box as selected
                            SDL_Rect textureDestination;
                            textureDestination.x = (BOX_WIDTH * column) + 1;
                            textureDestination.y = (BOX_HEIGHT * row) + 1;
                            textureDestination.h = 48;
                            textureDestination.w = 48;

                            SDL_SetTextureAlphaMod(selectedBox, 100); // Changes opacity of the selected box

                            SDL_QueryTexture(selectedBox, NULL, NULL, &textureDestination.w, &textureDestination.h);
                            SDL_RenderCopy(renderer, selectedBox, NULL, &textureDestination);

                            // Draw all the possible moves a box can move
                            while (allPossibleMovesRowAndColumn != NULL) {
                                if (allPossibleMovesRowAndColumn->boxLoc.x != 0) {
                                    SDL_Rect textureDestination;
                                    textureDestination.x = allPossibleMovesRowAndColumn->boxLoc.x + 1;
                                    textureDestination.y = allPossibleMovesRowAndColumn->boxLoc.y + 1;
                                    textureDestination.h = 48;
                                    textureDestination.w = 48;

                                    SDL_QueryTexture(possibleMoveLoc, NULL, NULL, &textureDestination.w, &textureDestination.h);
                                    SDL_RenderCopy(renderer, possibleMoveLoc, NULL, &textureDestination);
                                }
                                allPossibleMovesRowAndColumn = allPossibleMovesRowAndColumn->next;
                            }
                        } else { // Deselect the box
                            selectedBoxPos.x = -1;
                            selectedBoxPos.y = -1;
                        }
                    }

                    //freeLinkedList(allPossibleMovesRowAndColumn);
                }
            }
        }

        // Draw the current turn text
        SDL_Rect turnTextDest;
        turnTextDest.x = BOX_WIDTH * 8;
        turnTextDest.y = 10;

        if (game.player == PLAYER_ONE_ID) {
            SDL_QueryTexture(turnPlayerOne, NULL, NULL, &turnTextDest.w, &turnTextDest.h);
            SDL_RenderCopy(renderer, turnPlayerOne, NULL, &turnTextDest);
        } else {
            SDL_QueryTexture(turnPlayerTwo, NULL, NULL, &turnTextDest.w, &turnTextDest.h);
            SDL_RenderCopy(renderer, turnPlayerTwo, NULL, &turnTextDest);
        }
            
        // Draw the taken out pieces
        getTakenOutPieces(PLAYER_ONE_ID, playerOneTakenOutPieces);
        getTakenOutPieces(PLAYER_TWO_ID, playerTwoTakenOutPieces);
        char typeOfTakenOutPiece[] = { 'P', 'R', 'H', 'B', 'K', 'Q' };
        int player1TakenOutPiecesCount = 0;
        int player2TakenOutPiecesCount = 0;

        SDL_Rect playerTwoTakenOutPieceTextureDest; // On Player1's side
        playerTwoTakenOutPieceTextureDest.x = SCREEN_WIDTH - (BOX_WIDTH * 5);
        playerTwoTakenOutPieceTextureDest.y = (BOX_HEIGHT * 2) + 20;
        playerTwoTakenOutPieceTextureDest.h = 40;
        playerTwoTakenOutPieceTextureDest.w = 40;

        SDL_Rect playerOneTakenOutPieceTextureDest; // On Player2's side
        playerOneTakenOutPieceTextureDest.x = SCREEN_WIDTH - (BOX_WIDTH * 5);
        playerOneTakenOutPieceTextureDest.y = SCREEN_HEIGHT - (BOX_HEIGHT * 8) + 20;
        playerOneTakenOutPieceTextureDest.h = 40;
        playerOneTakenOutPieceTextureDest.w = 40;

        for (int i = 0; i < 6; i++) {
            if (playerOneTakenOutPieces[i] != 0) {
                    
                for (int j = 0; j < playerOneTakenOutPieces[i]; j++) {
                    if (player1TakenOutPiecesCount % 5 == 0) { // Move to new line
                        playerOneTakenOutPieceTextureDest.y += BOX_HEIGHT;
                        playerOneTakenOutPieceTextureDest.x = SCREEN_WIDTH - (BOX_WIDTH * 5);
                    }
                        
                    player1TakenOutPiecesCount++;

                    if (typeOfTakenOutPiece[i] == 'P') {
                        SDL_QueryTexture(pawnTexture, NULL, NULL, &playerOneTakenOutPieceTextureDest.w, &playerOneTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, pawnTexture, NULL, &playerOneTakenOutPieceTextureDest);
                    } else if (typeOfTakenOutPiece[i] == 'R') {
                        SDL_QueryTexture(rookTexture, NULL, NULL, &playerOneTakenOutPieceTextureDest.w, &playerOneTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, rookTexture, NULL, &playerOneTakenOutPieceTextureDest);
                    } else if (typeOfTakenOutPiece[i] == 'H') {
                        SDL_QueryTexture(knightTexture, NULL, NULL, &playerOneTakenOutPieceTextureDest.w, &playerOneTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, knightTexture, NULL, &playerOneTakenOutPieceTextureDest);
                    } else if (typeOfTakenOutPiece[i] == 'B') {
                        SDL_QueryTexture(bishopTexture, NULL, NULL, &playerOneTakenOutPieceTextureDest.w, &playerOneTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, bishopTexture, NULL, &playerOneTakenOutPieceTextureDest);
                    } else if (typeOfTakenOutPiece[i] == 'K') {
                        SDL_QueryTexture(kingTexture, NULL, NULL, &playerOneTakenOutPieceTextureDest.w, &playerOneTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, kingTexture, NULL, &playerOneTakenOutPieceTextureDest);
                    } else if (typeOfTakenOutPiece[i] == 'Q') {
                        SDL_QueryTexture(queenTexture, NULL, NULL, &playerOneTakenOutPieceTextureDest.w, &playerOneTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, queenTexture, NULL, &playerOneTakenOutPieceTextureDest);
                    }

                    playerOneTakenOutPieceTextureDest.x += BOX_WIDTH - 5; // Move forward
                }
            }

            if (playerTwoTakenOutPieces[i] != 0) {
                for (int j = 0; j < playerTwoTakenOutPieces[i]; j++) {
                    if (player2TakenOutPiecesCount % 5 == 0) { // Move to new line
                        playerTwoTakenOutPieceTextureDest.y += BOX_HEIGHT;
                        playerTwoTakenOutPieceTextureDest.x = SCREEN_WIDTH - (BOX_WIDTH * 5);
                    }

                    player2TakenOutPiecesCount++;

                    if (typeOfTakenOutPiece[i] == 'P') {
                        SDL_QueryTexture(blackPawnTexture, NULL, NULL, &playerTwoTakenOutPieceTextureDest.w, &playerTwoTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, blackPawnTexture, NULL, &playerTwoTakenOutPieceTextureDest);
                    }
                    else if (typeOfTakenOutPiece[i] == 'R') {
                        SDL_QueryTexture(blackRookTexture, NULL, NULL, &playerTwoTakenOutPieceTextureDest.w, &playerTwoTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, blackRookTexture, NULL, &playerTwoTakenOutPieceTextureDest);
                    }
                    else if (typeOfTakenOutPiece[i] == 'H') {
                        SDL_QueryTexture(blackKnightTexture, NULL, NULL, &playerTwoTakenOutPieceTextureDest.w, &playerTwoTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, blackKnightTexture, NULL, &playerTwoTakenOutPieceTextureDest);
                    }
                    else if (typeOfTakenOutPiece[i] == 'B') {
                        SDL_QueryTexture(blackBishopTexture, NULL, NULL, &playerTwoTakenOutPieceTextureDest.w, &playerTwoTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, blackBishopTexture, NULL, &playerTwoTakenOutPieceTextureDest);
                    }
                    else if (typeOfTakenOutPiece[i] == 'K') {
                        SDL_QueryTexture(blackKingTexture, NULL, NULL, &playerTwoTakenOutPieceTextureDest.w, &playerTwoTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, blackKingTexture, NULL, &playerTwoTakenOutPieceTextureDest);
                    }
                    else if (typeOfTakenOutPiece[i] == 'Q') {
                        SDL_QueryTexture(blackQueenTexture, NULL, NULL, &playerTwoTakenOutPieceTextureDest.w, &playerTwoTakenOutPieceTextureDest.h);
                        SDL_RenderCopy(renderer, blackQueenTexture, NULL, &playerTwoTakenOutPieceTextureDest);
                    }

                    playerTwoTakenOutPieceTextureDest.x += BOX_WIDTH - 5; // Move forward
                }
            }
        }

        char isPlayerOneCheck = checkForCheck(PLAYER_ONE_ID, getKingPos(PLAYER_ONE_ID));
        char isPlayerTwoCheck = checkForCheck(PLAYER_TWO_ID, getKingPos(PLAYER_TWO_ID));

        if (isPlayerOneCheck == 'T') {
            pos playerOneKingPos = getKingPos(PLAYER_ONE_ID);
            SDL_Rect playerOneKingTextureDest;
            playerOneKingTextureDest.x = (BOX_WIDTH * (6 + playerOneKingPos.x)) + 1;
            playerOneKingTextureDest.y = (BOX_HEIGHT * (3 + playerOneKingPos.y)) + 1;
            playerOneKingTextureDest.h = 48;
            playerOneKingTextureDest.w = 48;

            SDL_QueryTexture(underCheckBox, NULL, NULL, &playerOneKingTextureDest.w, &playerOneKingTextureDest.h);
            SDL_RenderCopy(renderer, underCheckBox, NULL, &playerOneKingTextureDest);
        }

        if (isPlayerTwoCheck == 'T') {
            pos playerTwoKingPos = getKingPos(PLAYER_TWO_ID);
            SDL_Rect playerTwoKingTextureDest;
            playerTwoKingTextureDest.x = (BOX_WIDTH * (6 + playerTwoKingPos.x)) + 1;
            playerTwoKingTextureDest.y = (BOX_HEIGHT * (3 + playerTwoKingPos.y)) + 1;
            playerTwoKingTextureDest.h = 48;
            playerTwoKingTextureDest.w = 48;

            SDL_QueryTexture(underCheckBox, NULL, NULL, &playerTwoKingTextureDest.w, &playerTwoKingTextureDest.h);
            SDL_RenderCopy(renderer, underCheckBox, NULL, &playerTwoKingTextureDest);
        }

        // Draw the pieces on the board
        // Updated whenever the board array is updated
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (chessBoard[i][j] == PLAYER_ONE_ID) {
                    char typeOfPiece = playerPieces[i][j];

                    SDL_Rect textureDestination;
                    textureDestination.x = (BOX_WIDTH * (6 + j)) + 1;
                    textureDestination.y = (BOX_HEIGHT * (3 + i));
                    textureDestination.h = 50;
                    textureDestination.w = 50;                  

                    if (typeOfPiece == 'P') {
                        SDL_QueryTexture(pawnTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, pawnTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'R') {
                        SDL_QueryTexture(rookTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, rookTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'H') {
                        SDL_QueryTexture(knightTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, knightTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'K') {                                           
                        SDL_QueryTexture(kingTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, kingTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'Q') {
                        SDL_QueryTexture(queenTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, queenTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'B') {
                        SDL_QueryTexture(bishopTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, bishopTexture, NULL, &textureDestination);
                    }
                } else if (chessBoard[i][j] == PLAYER_TWO_ID) {
                    char typeOfPiece = playerPieces[i][j];

                    SDL_Rect textureDestination;
                    textureDestination.x = (BOX_WIDTH * (6 + j)) + 1;
                    textureDestination.y = (BOX_HEIGHT * (3 + i));
                    textureDestination.h = 40;
                    textureDestination.w = 40;

                    if (typeOfPiece == 'P') {
                        SDL_QueryTexture(blackPawnTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, blackPawnTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'R') {
                        SDL_QueryTexture(blackRookTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, blackRookTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'H') {
                        SDL_QueryTexture(blackKnightTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, blackKnightTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'K') {                        
                        SDL_QueryTexture(blackKingTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, blackKingTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'Q') {
                        SDL_QueryTexture(blackQueenTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, blackQueenTexture, NULL, &textureDestination);
                    } else if (typeOfPiece == 'B') {
                        SDL_QueryTexture(blackBishopTexture, NULL, NULL, &textureDestination.w, &textureDestination.h);
                        SDL_RenderCopy(renderer, blackBishopTexture, NULL, &textureDestination);
                    }                                       
                }
            }
        }        

        SDL_RenderPresent(renderer);
    }
    
    // Textures need to be destroyed to avoid memory leak
    // All created textures deleted
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(turnPlayerOne);
    SDL_DestroyTexture(turnPlayerTwo);
    SDL_DestroyTexture(selectedBox);
    SDL_DestroyTexture(possibleMoveLoc);
    SDL_DestroyTexture(underCheckBox);
    SDL_DestroyTexture(playerOneWonBox);
    SDL_DestroyTexture(pawnTexture);
    SDL_DestroyTexture(blackPawnTexture);
    SDL_DestroyTexture(rookTexture);
    SDL_DestroyTexture(blackRookTexture);
    SDL_DestroyTexture(knightTexture);
    SDL_DestroyTexture(blackKnightTexture);
    SDL_DestroyTexture(kingTexture);
    SDL_DestroyTexture(blackKingTexture);
    SDL_DestroyTexture(queenTexture);
    SDL_DestroyTexture(blackQueenTexture);
    SDL_DestroyTexture(bishopTexture);
    SDL_DestroyTexture(blackBishopTexture);

    IMG_Quit(); // Closes SDL_Images
    SDL_DestroyWindow(window); // Destroy the instance of the window
    SDL_Quit(); // Closes SDL

    return EXIT_SUCCESS;
}   

void gameInit() {
    // Resets the board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chessBoard[i][j] = EMPTY;
            playerPieces[i][j] = ' ';
            movedDouble[i][j] = EMPTY;
        }
    }

    // Resets the pieces back to their original position
    for (int i = 0; i < 8; i++) {
        chessBoard[0][i] = chessBoard[1][i] = 1;
        chessBoard[6][i] = chessBoard[7][i] = 2;
    }

    // Resets the name of the pieces back to their original position
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            playerPieces[i][j] = resetToThis[i][j];
        }
    }

    for (int i = 0; i < 6; i++) {
        playerOneTakenOutPieces[i] = 0;
        playerTwoTakenOutPieces[i] = 0;
    }

    game.player = PLAYER_ONE_ID;
    game.state = RUNNING_STATE;

    selectedBoxPos.x = -1;
    selectedBoxPos.y = -1;

    hasPlayer1KingMoved = hasPlayer2KingMoved = 'F';
    hasPlayer1KingSideRookMoved = hasPlayer1QueenSideRookMoved = hasPlayer2KingSideRookMoved = hasPlayer2QueenSideRookMoved = 'F';

    srand(time(NULL));
}

void changePlayerTurn() {
    if (game.player == PLAYER_ONE_ID) {
        game.player = PLAYER_TWO_ID;
    } else if (game.player == PLAYER_TWO_ID) {
        game.player = PLAYER_ONE_ID;
    }

    printf("Turn: Player %d\n", game.player);
}

pos getBoxCD(int row, int column, int playerID) {
    pos boxCd;
    boxCd.x = column - 6;
    boxCd.y = row - 3;

    return boxCd;
}

PossiblePositions * getPossibleMovesRowAndColumn(pos currPos, int playerID, char typeOfPiece, int BOX_HEIGHT, int BOX_WIDTH) {
    PossiblePositions * allMovesRowAndColumn = NULL;

    PossiblePositions * possibleMovesForPiece = getAllPossibleMoves(game.player, typeOfPiece, currPos);

    while (possibleMovesForPiece != NULL) {
        
        if (allMovesRowAndColumn == NULL) { // If START is NULL
            allMovesRowAndColumn = malloc(sizeof(PossiblePositions));
            allMovesRowAndColumn->boxLoc.x = (possibleMovesForPiece->boxLoc.x + 6) * BOX_WIDTH;
            allMovesRowAndColumn->boxLoc.y = (possibleMovesForPiece->boxLoc.y + 3) * BOX_HEIGHT;

            allMovesRowAndColumn->next = NULL; // Points to the next node, NULL since this is the last node
        }
        else { // If the list already has items in it
         // Insert implementation for linked list
            PossiblePositions * current = allMovesRowAndColumn;
            while (current->next != NULL) { // Gets the last node
                current = current->next;
            }

            current->next = malloc(sizeof(PossiblePositions));
            current->next->boxLoc.x = (possibleMovesForPiece->boxLoc.x + 6) * BOX_WIDTH;
            current->next->boxLoc.y = (possibleMovesForPiece->boxLoc.y + 3) * BOX_HEIGHT;
            current->next->next = NULL;
        }

        possibleMovesForPiece = possibleMovesForPiece->next;
    }

    return allMovesRowAndColumn;
}

void getTakenOutPieces(int playerID) {
    int qtyOfPieces[] = {8, 2, 2, 2, 1, 1}; // P, R, H, B, K, Q
    char typeOfPiece[] = {'P', 'R', 'H', 'B', 'K', 'Q'};
    int currentQtyOfPieces[] = { 0, 0, 0, 0, 0, 0 };

    // Get the current qty of all types of pieces
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                if (chessBoard[j][k] == playerID) {
                    if (playerPieces[j][k] == typeOfPiece[i]) {
                        currentQtyOfPieces[i] += 1;
                    }
                }
            }
        }
    }
    
    // For each type of piece just subtract the current qty from the original qty
    for (int i = 0; i < 6; i++) {
        if (playerID == PLAYER_ONE_ID) {
            playerOneTakenOutPieces[i] = qtyOfPieces[i] - currentQtyOfPieces[i];
        } else if (playerID == PLAYER_TWO_ID) {
            playerTwoTakenOutPieces[i] = qtyOfPieces[i] - currentQtyOfPieces[i];
        }
    }
}

// PIECES
void pawn(int currX, int currY, int moveToX, int moveToY, int playerID) {
    pos currPos;
    pos moveToPos;
    currPos.x = currX, currPos.y = currY;
    moveToPos.x = moveToX, moveToPos.y = moveToY;

    char typeOfPiece = 'P';
        
    if (moveToPos.y == 0) { // For upgrading pawn
        typeOfPiece = pawnUpgradeToQueen;
    }
        
    if (playerID == PLAYER_ONE_ID) {
        if (currPos.y - moveToPos.y == 2) { // For enpassant
            movedDouble[moveToPos.y][moveToPos.x] = playerID;
        }

        pieceMovement(PLAYER_ONE_ID, typeOfPiece, currPos, moveToPos);

        // Reset the array
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (movedDouble[i][j] == PLAYER_TWO_ID) {
                    movedDouble[i][j] = 0;
                }
            }
        }
    } else {
        if (currPos.y - moveToPos.y == 2) { // For enpassant
            movedDouble[moveToPos.y][moveToPos.x] = playerID;                
        }

        pieceMovement(PLAYER_TWO_ID, typeOfPiece, currPos, moveToPos);
            
        // Reset the array
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (movedDouble[i][j] == PLAYER_ONE_ID) {
                    movedDouble[i][j] = 0;
                }
            }
        }
    }
}

void king(int moveToX, int moveToY, int playerID) {
    pos kingCurrPos = getKingPos(playerID);
    pos moveToPos;
    moveToPos.x = moveToX, moveToPos.y = moveToY;

    // To update the moved status of the king
    if (playerID == PLAYER_ONE_ID) {
        hasPlayer1KingMoved = 'T';
        pieceMovement(PLAYER_ONE_ID, 'K', kingCurrPos, moveToPos);
    } else {
        hasPlayer2KingMoved = 'T';
        pieceMovement(PLAYER_TWO_ID, 'K', kingCurrPos, moveToPos);
    }
}

void rook(int currX, int currY, int moveToX, int moveToY, int playerID) {
    pos currPos;
    pos moveToPos;
    currPos.x = currX, currPos.y = currY;
    moveToPos.x = moveToX, moveToPos.y = moveToY;
    
    // To update the moved state of the rooks
    if (playerID == PLAYER_ONE_ID) {
        if (currPos.x == 0 && currPos.y == 0) {
            hasPlayer1KingSideRookMoved = 'T';
        } else if (currPos.x == 7 && currPos.y == 0) {
            hasPlayer1QueenSideRookMoved = 'T';
        }

        pieceMovement(PLAYER_ONE_ID, 'R', currPos, moveToPos);
    } else {
        if (currPos.x == 0 && currPos.y == 7) {
            hasPlayer2KingSideRookMoved = 'T';
        } else if (currPos.x == 7 && currPos.y == 7) {
            hasPlayer2QueenSideRookMoved = 'T';
        }

        pieceMovement(PLAYER_TWO_ID, 'R', currPos, moveToPos);
    }
}

void queen(int currX, int currY, int moveToX, int moveToY, int playerID) {
    pos currPos;
    pos moveToPos;
    
    currPos.x = currX, currPos.y = currY;
    moveToPos.x = moveToX, moveToPos.y = moveToY;

    if (playerID == PLAYER_ONE_ID) {
        pieceMovement(PLAYER_ONE_ID, 'Q', currPos, moveToPos);
    }
    else {
        pieceMovement(PLAYER_TWO_ID, 'Q', currPos, moveToPos);
    }
}

void bishop(int currX, int currY, int moveToX, int moveToY, int playerID) {
    pos currPos;
    pos moveToPos;

    currPos.x = currX, currPos.y = currY;
    moveToPos.x = moveToX, moveToPos.y = moveToY;

    if (playerID == PLAYER_ONE_ID) {
        pieceMovement(PLAYER_ONE_ID, 'B', currPos, moveToPos);
    } else {
        pieceMovement(PLAYER_TWO_ID, 'B', currPos, moveToPos);
    }
}

void knight(int currX, int currY, int moveToX, int moveToY, int playerID) {
    pos currPos;
    pos moveToPos;

    currPos.x = currX, currPos.y = currY;
    moveToPos.x = moveToX, moveToPos.y = moveToY;

    if (playerID == PLAYER_ONE_ID) {
        pieceMovement(PLAYER_ONE_ID, 'H', currPos, moveToPos);
    } else {
        pieceMovement(PLAYER_TWO_ID, 'H', currPos, moveToPos);
    }
}

// PIECE LOGIC
char legalMoveChecker(int playerID, char typeOfPiece, pos currPos, pos moveToPos) {
    // ONLY WRITTEN FOR PLAYER 2 SINCE THE BOARD IS FLIPPED
    if (typeOfPiece == 'P') { // For pawn
        char isMoveValid = 'F';

        char isAtRestPos = currPos.y == 6 ? 'T' : 'F'; // At starting pos
        int enemyId = (playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID);
        // If can beat an enemy piece to the left
        char canBeatLeft = chessBoard[currPos.y - 1][currPos.x - 1] == enemyId ? 'T' : 'F';
        // If can beat an enemy piece to the right
        char canBeatRight = chessBoard[currPos.y - 1][currPos.x + 1] == enemyId ? 'T' : 'F';

        char enPassantLeft = movedDouble[currPos.y][currPos.x - 1] == enemyId ? 'T' : 'F';
        char enPassantRight = movedDouble[currPos.y][currPos.x + 1] == enemyId ? 'T' : 'F';

        if (isAtRestPos == 'T') {
            if (((currPos.y - moveToPos.y) <= 2) && ((currPos.y - moveToPos.y) > 0)) { // At starting pos can either move 1 or 2 boxes
                if (moveToPos.x == currPos.x) { // Can not move diagonal unless an enemy is present diagonally 1 box away
                    for (int i = 1; i <= (currPos.y - moveToPos.y); i++) { // To check if any piece is infront of the pawn
                        if (chessBoard[currPos.y - i][currPos.x] != EMPTY) { // If not an empty space
                            isMoveValid = 'F';
                            break;
                        }
                        else {
                            isMoveValid = 'T';
                        }
                    }
                }
            }
        } else {
            if (((currPos.y - moveToPos.y) == 1) && (moveToPos.x == currPos.x)) { // Normally can only move 1 box
                if (chessBoard[moveToPos.y][moveToPos.x] == EMPTY) { // To check if there is no piece ahead
                    isMoveValid = 'T';
                }
            }
        }

        if (canBeatLeft == 'T' || enPassantLeft == 'T') {
            if ((moveToPos.y == (currPos.y - 1)) && (moveToPos.x == (currPos.x - 1))) { // If player wishes to move to a box with an enemy piece
                isMoveValid = 'T';
            }
        }
        if (canBeatRight == 'T' || enPassantRight == 'T') {
            if ((moveToPos.y == (currPos.y - 1)) && (moveToPos.x == (currPos.x + 1))) { // If player wishes to move to a box with an enemy piece
                isMoveValid = 'T';
            }
        }
        if (chessBoard[moveToPos.y][moveToPos.x] == playerID) { // If a friendly piece is not already at the move to position
            isMoveValid = 'F';
        }

        return isMoveValid;
    } else if (typeOfPiece == 'K') { // For King
        char isMoveValid = 'T';
        
        // King can move only 1 box in any direction
        if ((getAbsVal(moveToPos.x - currPos.x) <= 1 && getAbsVal(moveToPos.x - currPos.x) >= 0) && (getAbsVal(moveToPos.y - currPos.y) <= 1 && getAbsVal(moveToPos.y - currPos.y) >= 0)) {
            if (boundaryCheck(moveToPos) == 'F') { // If the box is not inside the bounds of the board
                isMoveValid = 'F';
            } else {
                if (chessBoard[moveToPos.y][moveToPos.x] == playerID) { // If the box that the king can move to has a friendly piece
                    isMoveValid = 'F';
                }
            }
        } else {
            isMoveValid = 'F';
        }

        return isMoveValid;
    } else if (typeOfPiece == 'R') { // For Rook
        char isMoveValid = 'F';
        char enemyPlayerID = playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID; // Enemy

        pos lastBox; // Holds the position of the very last allowed box
        lastBox.x = currPos.x, lastBox.y = currPos.y;

        if (currPos.y == moveToPos.y) { // Horizontal Movement
            if (moveToPos.x > 7) { // If boxes moved is within the bounds of the board
                isMoveValid = 'F';
            } else {
                if (currPos.x > moveToPos.x) { // Move to left
                    for (int i = 1; i <= (currPos.x - moveToPos.x); i++) {
                        if (chessBoard[currPos.y][currPos.x - i] != EMPTY) {
                            isMoveValid = 'F';
                            lastBox.x = currPos.x - i, lastBox.y = currPos.y;
                            break;
                        } else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }
                } else { // Move to Right
                    for (int i = 1; i <= (moveToPos.x - currPos.x); i++) {
                        if (chessBoard[currPos.y][currPos.x + i] != EMPTY) {
                            isMoveValid = 'F';
                            lastBox.x = currPos.x + i, lastBox.y = currPos.y;
                            break;
                        } else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }
                }
            }
        } else if (currPos.x == moveToPos.x) { // Vertical Movement
            if (moveToPos.y > 7) { // If boxes moved is within the bounds of the board
                isMoveValid = 'F';
            } else {
                if (currPos.y > moveToPos.y) { // Move up
                    for (int i = 1; i <= (currPos.y - moveToPos.y); i++) {
                        if (chessBoard[currPos.y - i][currPos.x] != EMPTY) { // Last Box at/after which the piece can not move
                            isMoveValid = 'F';
                            lastBox.x = currPos.x, lastBox.y = currPos.y - i;
                            break;
                        } else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') { // If the last box is the enemy then move is valid
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }

                } else { // Move down
                    for (int i = 1; i <= (moveToPos.y - currPos.y); i++) {
                        if (chessBoard[currPos.y + i][currPos.x] != EMPTY) { // Last Box at/after which the piece can not move
                            isMoveValid = 'F';
                            lastBox.x = currPos.x, lastBox.y = currPos.y + i;
                            break;
                        } else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') { // If the last box is the enemy then move is valid
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }
                }
            }
        } else { // Movement in any other direction is invalid
            isMoveValid = 'F';
        }

        return isMoveValid;
    } else if (typeOfPiece == 'Q') { // For Queen
        char isMoveValid = 'F';
        char enemyPlayerID = playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID; // Enemy

        pos lastBox; // Holds the position of the very last allowed box
        lastBox.x = currPos.x, lastBox.y = currPos.y;

        if (currPos.y == moveToPos.y) { // Horizontal Movement
            if (moveToPos.x > 7) { // If boxes moved is within the bounds of the board
                isMoveValid = 'F';
            }
            else {
                if (currPos.x > moveToPos.x) { // Move to left
                    for (int i = 1; i <= (currPos.x - moveToPos.x); i++) {
                        if (chessBoard[currPos.y][currPos.x - i] != EMPTY) {
                            isMoveValid = 'F';
                            lastBox.x = currPos.x - i, lastBox.y = currPos.y;
                            break;
                        }
                        else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }
                } else { // Move to right
                    for (int i = 1; i <= (moveToPos.x - currPos.x); i++) {
                        if (chessBoard[currPos.y][currPos.x + i] != EMPTY) {
                            isMoveValid = 'F';
                            lastBox.x = currPos.x + i, lastBox.y = currPos.y;
                            break;
                        } else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }
                }
            }
        } else if (currPos.x == moveToPos.x) { // Vertical Movement
            if (moveToPos.y > 7) { // If boxes moved is within the bounds of the board
                isMoveValid = 'F';
            } else {
                if (currPos.y > moveToPos.y) { // Move up
                    for (int i = 1; i <= (currPos.y - moveToPos.y); i++) {
                        if (chessBoard[currPos.y - i][currPos.x] != EMPTY) {
                            isMoveValid = 'F';
                            lastBox.x = currPos.x, lastBox.y = currPos.y - i;
                            break;
                        } else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }

                }
                else { // Move Down
                    for (int i = 1; i <= (moveToPos.y - currPos.y); i++) {
                        if (chessBoard[currPos.y + i][currPos.x] != EMPTY) {
                            isMoveValid = 'F';
                            lastBox.x = currPos.x, lastBox.y = currPos.y + i;
                            break;
                        }
                        else {
                            isMoveValid = 'T';
                        }
                    }

                    if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                        if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                            isMoveValid = 'T';
                        }
                    }
                }
            }
        } else if (isInPrimaryDiag(currPos, moveToPos) == 'T') { // Diagonal Movement - Primary
            if (moveToPos.y < currPos.y && moveToPos.x < currPos.x) { // Move to box in top Left
                pos currBox;
                currBox = currPos;

                while (currBox.x >= 0 && currBox.y >= 0) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x -= 1;
                    currBox.y -= 1;

                    if (chessBoard[currBox.y][currBox.x] != EMPTY) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }

            }
            else if (moveToPos.y > currPos.y  && moveToPos.x > currPos.x) { // Move to box in bottom right
                pos currBox;
                currBox = currPos;

                while (currBox.x <= 7 && currBox.y <= 7) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x += 1;
                    currBox.y += 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }
            }
            else {
                isMoveValid = 'F';
            }
        }
        else if (isInSecondaryDiag(currPos, moveToPos) == 'T') { // Diagonal Movement - Secondary

            if (moveToPos.y < currPos.y && moveToPos.x > currPos.x) { // Move to box in top right
                pos currBox;
                currBox = currPos;

                while (currBox.x <= 7 && currBox.y >= 0) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x += 1;
                    currBox.y -= 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }
            }
            else if (moveToPos.y > currPos.y && moveToPos.x < currPos.x) { // Move to box in bottom left
                pos currBox;
                currBox = currPos;

                while (currBox.x >= 0 && currBox.y <= 7) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x -= 1;
                    currBox.y += 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }
            }
        } else { // Movement in any other direction is invalid
            isMoveValid = 'F';
        }
        
        return isMoveValid;
    } else if(typeOfPiece == 'B') { // For Bishop
        char isMoveValid = 'F';
        char enemyPlayerID = playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID; // Enemy

        pos lastBox; // Holds the position of the very last allowed box
        lastBox.x = currPos.x, lastBox.y = currPos.y;

        if (isInPrimaryDiag(currPos, moveToPos) == 'T') { // Diagonal Movement - Primary
            if (moveToPos.y < currPos.y && moveToPos.x < currPos.x) { // Move to box in top Left
                pos currBox;
                currBox = currPos;

                while (currBox.x >= 0 && currBox.y >= 0) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x -= 1;
                    currBox.y -= 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    } else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }

            } else if (moveToPos.y > currPos.y  && moveToPos.x > currPos.x) { // Move to box in bottom right
                pos currBox;
                currBox = currPos;

                while (currBox.x <= 7 && currBox.y <= 7) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x += 1;
                    currBox.y += 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }
            } else {
                isMoveValid = 'F';
            }
        } else if (isInSecondaryDiag(currPos, moveToPos) == 'T') { // Diagonal Movement - Secondary

            if (moveToPos.y < currPos.y && moveToPos.x > currPos.x) { // Move to box in top right
                pos currBox;
                currBox = currPos;

                while (currBox.x <= 7 && currBox.y >= 0) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x += 1;
                    currBox.y -= 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }
            } else if (moveToPos.y > currPos.y && moveToPos.x < currPos.x) { // Move to box in bottom left
                pos currBox;
                currBox = currPos;

                while (currBox.x >= 0 && currBox.y <= 7) {
                    if (areSameBoxes(currBox, moveToPos) == 'T') {
                        break;
                    }

                    currBox.x -= 1;
                    currBox.y += 1;

                    if (chessBoard[currBox.y][currBox.x] != 0) {
                        isMoveValid = 'F';
                        lastBox = currBox;
                        break;
                    }
                    else {
                        isMoveValid = 'T';
                    }
                }

                if (isMoveValid == 'F' && areSameBoxes(moveToPos, lastBox) == 'T') {
                    if (chessBoard[lastBox.y][lastBox.x] == enemyPlayerID) {
                        isMoveValid = 'T';
                    }
                }
            }
        }

        return isMoveValid;
    } else if (typeOfPiece == 'H') { // For Knight
        char isMoveValid = 'F';
        char enemyPlayerID = playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID; // Enemy

        if (getAbsVal(moveToPos.y - currPos.y) == 1 && getAbsVal(moveToPos.x - currPos.x) == 2) { // 1 Y and 2X L shape
            if (chessBoard[moveToPos.y][moveToPos.x] == EMPTY || chessBoard[moveToPos.y][moveToPos.x] == enemyPlayerID) { // If not playerID
                isMoveValid = 'T';
            }   
        } else if (getAbsVal(moveToPos.y - currPos.y) == 2 && getAbsVal(moveToPos.x - currPos.x) == 1) { // 2Y and 1X L shape
            if (chessBoard[moveToPos.y][moveToPos.x] == EMPTY || chessBoard[moveToPos.y][moveToPos.x] == enemyPlayerID) { // If not playerID 
                isMoveValid = 'T';
            }
        }

        return isMoveValid;
    }

    return 'F';

}

PossiblePositions* getAllPossibleMoves(int playerID, char typeOfPiece, pos currPos) { // Returns a list of possible move to positions for a piece
    PossiblePositions * possiblePositions = NULL; // START of linked list    

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos moveToPos;
            moveToPos.x = j, moveToPos.y = i;
            pos tempCurrPos = currPos;
            char isBoardFlipped = 'F';

            if (playerID == PLAYER_ONE_ID) {
                flipBoard();
                isBoardFlipped = 'T';
                tempCurrPos.x = 7 - tempCurrPos.x, tempCurrPos.y = 7 - tempCurrPos.y;
                moveToPos.x = 7 - j, moveToPos.y = 7 - i;
            }
            char isMoveLegal = legalMoveChecker(playerID, typeOfPiece, tempCurrPos, moveToPos);
            if (isBoardFlipped == 'T') {
                flipBoard();
                isBoardFlipped = 'F';
                moveToPos.x = j;
                moveToPos.y = i;
            }
            
            if (isMoveLegal == 'T') {
                if (possiblePositions == NULL) { // If START is NULL
                    possiblePositions = malloc(sizeof(PossiblePositions));
                    possiblePositions->boxLoc = moveToPos;
                    possiblePositions->next = NULL; // Points to the next node, NULL since this is the last node
                } else { // If the list already has items in it
                    // Insert implementation for link list
                    PossiblePositions * current = possiblePositions;
                    while (current->next != NULL) { // Gets the last node
                        current = current->next;
                    }

                    current->next = malloc(sizeof(PossiblePositions));
                    current->next->boxLoc = moveToPos; // Appends to the list
                    current->next->next = NULL; // Sets the address of the next node to NULL since the current node is the last in the list
                }
            }
        }    
    }

    return possiblePositions;
}

char listContains(PossiblePositions *possiblePositions, pos searchFor) {
    PossiblePositions * allPossibleMoveToPos = possiblePositions;

    pos searchForUpdate;
    searchForUpdate.x = (searchFor.x + 6) * 50;
    searchForUpdate.y = (searchFor.y + 3) * 50;

    while (allPossibleMoveToPos != NULL) { // Gets the last node
        if(allPossibleMoveToPos->boxLoc.x != 0){
            if (areSameBoxes(allPossibleMoveToPos->boxLoc, searchForUpdate) == 'T') {
                return 'T';
            }
        }
        allPossibleMoveToPos = allPossibleMoveToPos->next;
    }

    return 'F';
}

void removePossiblePosition(PossiblePositions * possiblePositions, pos posToRemove) {
    PossiblePositions * temp = possiblePositions, * prevNode = NULL;

    if (temp != NULL && areSameBoxes(temp->boxLoc, posToRemove) == 'T') {
        possiblePositions = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && areSameBoxes(temp->boxLoc, posToRemove) == 'F') {
        prevNode = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    prevNode->next = temp->next;
    free(temp);
}

char checkForCheck(int playerID, pos posToCheckFor) {
    // Checks if the king for a player is under check at a specific location

    pos checkFor;
    checkFor = posToCheckFor;

    PossiblePositions * allEnemyMove = NULL;

    char isCheck = 'F';

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int enemyPlayerID = playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID;
            if (chessBoard[i][j] == enemyPlayerID) { // If king belongs to player1 then check for pieces belonging to player2
                char typeOfPiece = playerPieces[i][j];
                pos currPos;
                currPos.x = j, currPos.y = i;

                PossiblePositions * currentPiecePossibleMoves;

                currentPiecePossibleMoves = getAllPossibleMoves(enemyPlayerID, typeOfPiece, currPos);

                while (currentPiecePossibleMoves != NULL) { // Runs for every node in the list
                    // If the position of the king is found in the possible moves of any of the enemy pieces then the king is under check
                    
                    if (areSameBoxes(checkFor, currentPiecePossibleMoves->boxLoc) == 'T') {
                        return 'T';
                    }

                    currentPiecePossibleMoves = currentPiecePossibleMoves->next;
                    
                }
            }
        }
    }
    return isCheck;
}

pos getKingPos(int playerID) {
    // Returns the current position of the king for any player
    pos kingPos;
    kingPos.x = 0;
    kingPos.y = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == playerID && playerPieces[i][j] == 'K') {
                kingPos.x = j;
                kingPos.y = i;
                break;
            }
        }
    }

    return kingPos;
}

char isCheckmate(int playerID) {
    // For a player, checks for checkmate
    pos kingCurrPos = getKingPos(playerID); // Current position of the king
    PossiblePositions * allPossibleKingMovements = getAllPossibleMoves(playerID, 'K', kingCurrPos); // List of all possible moves of the king

    char isCheckmate = 'T';
    
    isCheckmate = checkForCheck(playerID, kingCurrPos); // If is under check at curr position

    while (allPossibleKingMovements != NULL) { // Checks for check at all positions the king can move to
        pos kingOldPos = kingCurrPos;
        char pieceAtNewKingPos = playerPieces[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x];
        int pieceIdAtNewKingPos = chessBoard[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x];

        playerPieces[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x] = 'K';
        playerPieces[kingOldPos.y][kingOldPos.x] = ' ';
        chessBoard[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x] = playerID;
        chessBoard[kingOldPos.y][kingOldPos.x] = EMPTY;
        
        kingCurrPos = getKingPos(playerID);

        // If at any possible position the king is not under check then it is not under checkmate
        if (checkForCheck(playerID, kingCurrPos) == 'F') {
            playerPieces[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x] = pieceAtNewKingPos;
            playerPieces[kingOldPos.y][kingOldPos.x] = 'K';
            chessBoard[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x] = pieceIdAtNewKingPos;
            chessBoard[kingOldPos.y][kingOldPos.x] = playerID;

            return 'F';
        } else {
            playerPieces[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x] = pieceAtNewKingPos;
            playerPieces[kingOldPos.y][kingOldPos.x] = 'K';
            chessBoard[allPossibleKingMovements->boxLoc.y][allPossibleKingMovements->boxLoc.x] = pieceIdAtNewKingPos;
            chessBoard[kingOldPos.y][kingOldPos.x] = playerID;

            kingCurrPos = kingOldPos;
        }

        

        allPossibleKingMovements = allPossibleKingMovements->next;
    }

    return isCheckmate; // Returns 'T' if is under check at all possible position
}

char isGameDraw() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (playerPieces[i][j] != 'K' && playerPieces[i][j] != ' ') {
                return 'F';
            }
        }
    }

    return 'T';

}

char isInStaleMate(int playerID) {
    // When there are no legal moves left for a player
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == playerID) {
                char typeOfPiece = playerPieces[i][j];
                pos pieceCurrPos;
                pieceCurrPos.x = j;
                pieceCurrPos.y = i;

                PossiblePositions * possiblePositionsForPiece = getAllPossibleMoves(playerID, typeOfPiece, pieceCurrPos);

                if (possiblePositionsForPiece != NULL) { // If a piece with some legal moves is left then false
                    return 'F';
                }
            }
        }
    }

    return 'T';
}

void pieceMovement(int playerID, char pieceType, pos currPos, pos newPos) {
    int enemyID = playerID == PLAYER_ONE_ID ? PLAYER_TWO_ID : PLAYER_ONE_ID;
    
    if (pieceType == 'P') {// For EnPassant       
        if (movedDouble[newPos.y + 1][newPos.x] == enemyID) {
            chessBoard[newPos.y + 1][newPos.x] = 0;
            playerPieces[newPos.y + 1][newPos.x] = ' ';
        }
    }

    chessBoard[currPos.y][currPos.x] = EMPTY;
    chessBoard[newPos.y][newPos.x] = playerID;
    playerPieces[currPos.y][currPos.x] = ' ';
    playerPieces[newPos.y][newPos.x] = pieceType;
}

char isInPrimaryDiag(pos inDiagOf, pos searchFor) { // Checks if a box is in the primary diagonal of a piece
    char isInPrimaryDiag = 'F';
    pos tempUpperBox, tempLowerBox;
    tempUpperBox = tempLowerBox = inDiagOf;

    while (boundaryCheck(tempUpperBox) == 'T') {
        tempUpperBox.x -= 1;
        tempUpperBox.y -= 1;

        if (areSameBoxes(tempUpperBox, searchFor) == 'T') {
            isInPrimaryDiag = 'T';
            break;
        }
    }

    if (isInPrimaryDiag == 'F') { // If not found in the top left part
        while (boundaryCheck(tempLowerBox) == 'T') {
            tempLowerBox.x += 1;
            tempLowerBox.y += 1;

            if (areSameBoxes(tempLowerBox, searchFor) == 'T') {
                isInPrimaryDiag = 'T';
                break;
            }
        }
    }

    return isInPrimaryDiag;
}

char isInSecondaryDiag(pos inDiagOf, pos searchFor) { // Checks if a box is in the secondary diagonal of a piece
    char isInSecondaryDiag = 'F';
    pos tempUpperBox, tempLowerBox;
    tempUpperBox = tempLowerBox = inDiagOf;

    while (boundaryCheck(tempUpperBox) == 'T') {
        tempUpperBox.x += 1;
        tempUpperBox.y -= 1;
        
        if (areSameBoxes(tempUpperBox, searchFor) == 'T') {
            isInSecondaryDiag = 'T';
            break;
        }
    }

    if (isInSecondaryDiag == 'F') { // If not found in the top right part
        while (boundaryCheck(tempLowerBox) == 'T') {
            tempLowerBox.x -= 1;
            tempLowerBox.y += 1;

            if (areSameBoxes(tempLowerBox, searchFor) == 'T') {
                isInSecondaryDiag = 'T';
                break;
            }
        }
    }

    return isInSecondaryDiag;
}

char boundaryCheck(pos boxLoc) { // Checks if a box is within the bound of the board
    char isWithinBoundary = 'T';

    if (boxLoc.x < 0 || boxLoc.x > 7 || boxLoc.y < 0 || boxLoc.y > 7) {
        isWithinBoundary = 'F';
    }

    return isWithinBoundary;
}

char areSameBoxes(pos pos1, pos pos2) {
    return ((pos1.x == pos2.x) && (pos1.y == pos2.y)) ? 'T': 'F';
}

int getAbsVal(int num) {
    return num < 0 ? num * -1 : num;
}

void flipBoard() { // Flips the board as to only logic for player2 is needed
    char tempPlayerPieces[8][8] = {' '};
    int tempChessBoard[8][8] = {EMPTY};
    int tempMovedDouble[8][8] = { EMPTY };

    // Hold the reversed state of the board in a temporary array
    for (int i = 7; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            tempPlayerPieces[7 - i][7 - j] = playerPieces[i][j];
            tempChessBoard[7 - i][7 - j] = chessBoard[i][j];
            tempMovedDouble[7 - i][7 - j] = movedDouble[i][j];
        }
    }

    // Make the original board equal to the reversed temporary board array
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            playerPieces[i][j] = tempPlayerPieces[i][j];
            chessBoard[i][j] = tempChessBoard[i][j];
            movedDouble[i][j] = tempMovedDouble[i][j];
        }
    }
}

void freeLinkedList(PossiblePositions * list) {
    PossiblePositions * temp;
    while (list != NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }
}