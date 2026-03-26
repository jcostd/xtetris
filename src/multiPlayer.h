#ifndef MULTIPLAYER_H_
#define MULTIPLAYER_H_

#include "commonConfig.h"
#include <stdbool.h>
#include <ncurses.h>


/* ---------------------------------------------------------- defining title parameters */
/* Multi fields */
#define MTITLE_H         3
#define MTITLE_W        75

#define MPLAYER_X       31
#define PVSCPU_X        31

#define MCMDS_H          3
#define MCMDS_W         75
/* ---------------------------------------------------------- END defining title parameters */

#define FIRST_PLAYER     0

#define SECOND_PLAYER    1

void initMultiPlayerCmds( WINDOW* w_cmds );

void initMultiPlayerTitle( WINDOW* title );

void initMultiPlayerField( WINDOW* firstField, WINDOW* secondField );

void initMultiPlayerPreview( WINDOW* preview );

void initMultiPlayerScore( WINDOW* score );

void initMultiPlayerSave( WINDOW* save );

WINDOW* initFirstPlayerWindow( WINDOW* pgWindow );

WINDOW* initMultiPreviewWindow( WINDOW* preview );

WINDOW* initMultiScoreWindow( WINDOW* score );

void initMultiField( WINDOW* title, WINDOW* firstField, WINDOW* secondField, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds );

void refreshMultiScore( int pieces, int *pg1Score, int pg2Score, int *turn, WINDOW* score );

int multiPlayer();

int startTheGame( player* pg1, player* pg2, tet* piece, int* tetPieces, int* pieces, int* turn, WINDOW*s_preview, WINDOW* s_score, WINDOW* w_title, WINDOW* w_field, WINDOW* w_sfield, WINDOW* w_preview, WINDOW* w_score, WINDOW* w_save, WINDOW* w_cmds );

int checkAndReverseRows( player* pg1, player* pg2 );

void reverseRowsGameField( int secondGamefield[][ MATRIX_W ], int counterRows );

void multiGameOver( int score, int* turn );

int* changeTurn( int *turn );

void initMultiQuit();

void initWinner( player *pg1, player *pg2 );

#endif
