#ifndef SINGLEPLAYER_H_
#define SINGLEPLAYER_H_

#include "commonConfig.h"
#include <stdbool.h>
#include <ncurses.h>

/* ---------------------------------------------------------- defining title parameters */
#define TITLE_H          3
#define TITLE_W          51

#define CMDS_H           3
#define CMDS_W           51


/*-- init field function ---*/
void initSinglePlayerPreview( WINDOW* preview );
void initSinglePlayerSave( WINDOW* save );
void initSinglePlayerScore( WINDOW* score );
void initSinglePlayerTitle( WINDOW* title );
void initSinglePlayerField( WINDOW* field );
void initSinglePlayerCmds( WINDOW* cmds );
void initField( WINDOW* title, WINDOW* field, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds );
void initGameOver( int score );
void initTopLine( WINDOW *field );
void initQuit( int score );
int  initReturnToMenu( int score );

WINDOW* initPlayerWindow( WINDOW* pgWindow );
WINDOW* initPreviewWindow( WINDOW* preview );
WINDOW* initScoreWindow( WINDOW* score );

void changePiece( WINDOW* score );

/*--- init field variable ---*/
void initGameMatrix( int gameField[][ MATRIX_W ] );
void initTetVector();

/*--- repaint */
void colorField( player *pg );
void resetPreview();

/*--- refresh function ---*/
void refreshPreview( WINDOW* s_preview, tet* preview_piece );
void refreshGameField( int* x, tet* current_piece, player *pg );
void refreshScore( WINDOW* s_score ,int pieces, int score );

/*--- main function for gamefield ---*/
int calculateScoring( int rows );
int checkDeleteRows( player *pg);
int checkGameOver( int gamefield[][ MATRIX_W ] );
int smallerIntervall( int row, int col, int gamefield[][ MATRIX_W ] );

void goDownTetramini( int row, int gamefield[][MATRIX_W] );
void fallingPiece( player *pg );

/*--- piece movement ---*/
void rotatingPiece( tet* current_piece );
void nextPiece( tet* current_piece, tet* preview_piece );
void backPiece( tet* current_piece, tet* preview_piece );

/*--- single Player function --*/
int singlePlayer();

#endif /* SINGLEPLAYER_H_ */
