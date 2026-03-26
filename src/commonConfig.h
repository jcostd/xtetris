#ifndef COMMONCONFIG_H_
#define COMMONCONFIG_H_

#include <ncurses.h>

/* box vertical & horizontal lines  */
#define V_LINES                     ACS_VLINE
#define H_LINES                     ACS_HLINE

#define HCENTER                     ( ( LINES - FIELD_H ) / 2 )
#define WCENTER                     ( ( COLS  - FIELD_W ) / 2 )

/* defining field parameters */
#define FIELD_Y                     10
#define FIELD_X                     10

#define FIELD_H                     27
#define FIELD_W                     22

/* defining matrix parameters */
#define MATRIX_H                    25
#define MATRIX_W                    10

#define MATRIX_H_PREV               5

#define HELP_H                      20
#define HELP_W                      64

#define GAMEOVER_H                  9
#define GAMEOVER_W                  54

/* defining scoring board parameters */
#define SCORE_H                     8
#define SCORE_W                     21

/* defining pieces preview parameters */
#define PREVIEW_H                   8
#define PREVIEW_W                   21

/* defining saveing window parameters */
#define SAVE_H                      5
#define SAVE_W                      21

#define MQUIT_H                     9
#define MQUIT_W                     54

#define RTMENU_H                    9
#define RTMENU_W                    54

/* tetramino cells */
#define T_CELL                      4

/* number of tetraminos */
#define T_NUM                       7

/* number of possible orientation of tetramino */
#define T_ORI                       4

/* the number of each tetramino */
#define T_PIECES                    20

#define TOP_LINE                    5

#define TETS_CELL                   4

typedef struct
{
    int tet;
    int ori;
} tet;

typedef struct
{
    int row;
    int col;
} tet_location;

typedef struct
{
    int score;
    int gameField[ MATRIX_H ][ MATRIX_W ];
    WINDOW* window;
} player;

extern tet_location TETROMINOS[ T_NUM ][ T_ORI ][ T_CELL ];
extern tet_location cell;
extern int previewGamefield[ MATRIX_H ][ MATRIX_W ];

WINDOW* initSecondPlayerWindow( WINDOW* pgWindow );

player addPlayer();

void refreshGameField( int* x, tet* current_piece, player *pg );

void changePiece( WINDOW* score );

void refreshPreview( WINDOW* preview , tet* preview_piece );

void resetPreview();

void initTopLine( WINDOW* field );

void colorField( player *pg );

void initGameMatrix(int gameField[][MATRIX_W]);

void initTetVector(int *tetPieces, int mod);

int calculateScoring( int rows );

void backPiece( tet* current_piece, tet* preview_piece );

void paintHelp();

void rotatingPiece( tet* current_piece );

void nextPiece( tet* current_piece, tet* preview_piece );

void goDownTetramini( int row, int gamefield[][ MATRIX_W ] );

int checkGameOver( int gamefield[][ MATRIX_W ] );

int checkDeleteRows( player *pg );

int smallerIntervall( int row, int col, int gamefield[][ MATRIX_W ] );

void fallingPiece( player *pg );

#endif
