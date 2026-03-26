#include <stdbool.h>
#include "commonConfig.h"

int previewGamefield[ MATRIX_H ][ MATRIX_W ];

/* tridimensional array for tetramini */
tet_location cell = { 0, 0 };

tet_location TETROMINOS[ T_NUM][ T_ORI ][ T_CELL ] =
{
  // I
  { { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
    { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
    { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
    { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } } },
  // J
  { { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 2 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 } },
    { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
    { { 0, 1 }, { 1, 1 }, { 2, 0 }, { 2, 1 } } },
  // L
  { { { 1, 0 }, { 0, 0 }, { 0, 1 }, { 0, 2 } },
    { { 2, 1 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },
    { { 0, 2 }, { 1, 2 }, { 1, 1 }, { 1, 0 } },
    { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } } },
  // O
  { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } },
  // S
  { { { 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    { { 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },
  // T
  { { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } },
    { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
    { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 0 } } },
  // Z
  { { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
    { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
    { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } } },
};

player addPlayer()
{
    player player;
    player.score = 0;
    return player;
}

WINDOW* initSecondPlayerWindow( WINDOW* pgWindow )
{
    int fieldY, fieldX;

    fieldY   = HCENTER + 1;
    fieldX   =  WCENTER + SCORE_W + FIELD_W / 2  - 5 ;

    pgWindow = newwin( ( FIELD_H - 2 ), ( FIELD_W - 2 ), fieldY, fieldX );
    wbkgd( pgWindow, COLOR_PAIR( 0 ) );

    wrefresh( pgWindow );
    return pgWindow;
}

void nextPiece( tet* currentPiece, tet* previewPiece )
{
    currentPiece->tet += 1;

    if( currentPiece->tet < 6 )
        previewPiece->tet += 1;

    if(currentPiece->tet == 6)
        previewPiece->tet = 0;

    if(currentPiece->tet > 6)
    {
        currentPiece->tet = 0;
        previewPiece->tet = 1;
    }
}

void rotatingPiece( tet* currentPiece )
{
    if( currentPiece->ori == 3 )
        currentPiece->ori = 0;
    else
        currentPiece->ori += 1;
}

void backPiece( tet* currentPiece, tet* previewPiece )
{
    currentPiece->tet -= 1;

    if ( currentPiece->tet < 0 )
    {
        currentPiece->tet = 6;
        previewPiece->tet = 0;
    }
    else
        previewPiece->tet += 1;
}


void changePiece( WINDOW* score )
{
    mvwprintw( score, 5, ( (SCORE_W - 13) / 2 ), "CAMBIA PEZZO!" );
    wrefresh( score );
}

void refreshPreview( WINDOW* preview , tet* previewPiece )
{
    int i;
    int previewTet;
    int previewOri;

    previewTet = previewPiece->tet;
    previewOri = previewPiece->ori;

    /* Clearing the previeus box */
    werase(preview);

    for( i = 0; i < TETS_CELL; ++i )
    {
        wattron( preview, COLOR_PAIR( previewTet + 10 ) );

        cell = TETROMINOS[ previewTet ][ previewOri ][ i ];
        mvwprintw( preview, ( cell.row + 2 ), (1 + ( (cell.col + 2) * 2 ) ), "  " );

        wattroff( preview, COLOR_PAIR( previewTet + 10 ) );
    }

    wrefresh( preview );
}

void refreshGameField( int* x, tet* currentPiece, player *pg )
{
    int i;

    int currentTet = currentPiece->tet;
    int currentOri = currentPiece->ori;

    resetPreview();
    wrefresh( pg->window );

    for( i = 0; i < TETS_CELL; ++i )
    {
        wattron( pg->window, COLOR_PAIR( currentTet + 10 ) );

        cell = TETROMINOS[ currentTet ][ currentOri ][ i ];
        mvwprintw( pg->window, cell.row, ( ( ( cell.col ) * 2 ) + *x ), "  " );

        wattroff( pg->window, COLOR_PAIR( currentTet + 10 ) );

        if( cell.col + *x < 0 )
        {
            colorField( pg );
            *x += 2;
            refreshGameField( x, currentPiece, pg );
        }

        if( ( ( (cell.col) * 2 ) + *x ) > ( FIELD_W - 3 ) )
        {
            colorField( pg );
            *x -= 2;
            refreshGameField( x, currentPiece, pg );
        }

        previewGamefield[ cell.row ][ ( cell.col + ( *x / 2 ) ) ] = ( currentTet + 1 );
    }

    initTopLine( pg->window );
    wrefresh( pg->window );
}

void paintHelp()
{
    int starty, startx;
    WINDOW *w_help;

    starty = ( ( LINES - HELP_H) / 2 );
    startx = ( ( ( COLS  - HELP_W ) / 2 ) + 3 );

    refresh();
    w_help = newwin( HELP_H, HELP_W, starty, startx );
    wbkgd( w_help, COLOR_PAIR( 2 ) );

    mvwprintw( w_help, 0,  25, "| HELP PAGE |" );
    mvwprintw( w_help, 2,  14, "Vuoi una mano con i comandi del gioco??" );
    mvwprintw( w_help, 4,  24, "-- SELEZIONE --" );
    mvwprintw( w_help, 6,  10, "[ N ]  ==> prossimo tetramino" );
    mvwprintw( w_help, 7,  10, "[ B ]  ==> tetramino precedente" );
    mvwprintw( w_help, 8,  10, "[ R ]  ==> ruotare il tetramino" );
    mvwprintw( w_help, 10, 24, "-- MOVIMENTO --" );
    mvwprintw( w_help, 12, 10, "[ < ]  ==> muove il tetramino a sinistra" );
    mvwprintw( w_help, 13, 10, "[ > ]  ==> muove il tetramino a destra" );
    mvwprintw( w_help, 14, 10, "[ v ]  ==> la mossa desiderata" );

    wrefresh( w_help );
    wgetch( w_help );

    delwin(w_help);
    touchwin(stdscr);
}

void resetPreview()
{
    int row;
    int col;

    for( row = 0; row < MATRIX_H_PREV; row++ )
        for( col = 0; col < MATRIX_W; col++ )
            if(previewGamefield[ row ][ col ] != 0 )
                previewGamefield[ row ][ col ] = 0;
}

void initTopLine( WINDOW* field )
{
    int col;

    for (col = 0; col < ( FIELD_W - 2 ); ++col )
    {
        wattron( field, COLOR_PAIR( 19 ) );
        mvwprintw( field, ( TOP_LINE - 1 ), col, " " );
        wattroff( field, COLOR_PAIR( 19 ) );
    }

    wrefresh( field );
}

void colorField( player *pg )
{
    int i, j;

    werase( pg->window );

    for( i = 0; i < MATRIX_H; ++i )
    {
        for( j = 0; j < MATRIX_W; ++j )
        {
            if( pg->gameField[ i ][ j ] != 0 )
            {
                wattron( pg->window, COLOR_PAIR( pg->gameField[ i ][ j ] + 9 ) );
                mvwprintw( pg->window, i, j * 2, "  " );
                wattroff( pg->window, COLOR_PAIR( pg->gameField[ i ][ j ] + 9 ) );
            }
        }
    }

    initTopLine( pg->window );
    wrefresh( pg->window );
}

void initGameMatrix( int gameField[][ MATRIX_W ] )
{
    int row, cols;

    for( row = 0; row < MATRIX_H; ++row )
        for( cols = 0; cols < MATRIX_W; ++cols )
        {
            gameField[ row ][ cols ] = 0;
            if( row < MATRIX_H_PREV )
                previewGamefield[ row ][ cols ] = 0;
        }
}

void initTetVector( int *tetPieces, int mod )
{
    int i;

    if ( mod == 0 )
        for ( i = 0; i < T_NUM; ++i )
            tetPieces[ i ] = T_PIECES;

    if ( ( mod == 1 ) || ( mod == 2 ) )
        for (i = 0; i < T_NUM; ++i)
            tetPieces[ i ] = ( T_PIECES * 2 );
}

int calculateScoring( int rows )
{
    if ( rows == 0 )
        return ( 0 );

    if ( rows == 1 )
        return ( 1 );

    if ( rows == 2 )
        return ( 3 );

    if ( rows == 3 )
        return ( 6 );

    if ( rows == 4 )
        return ( 12 );

    return 0;
}

void fallingPiece( player *pg )
{

    int row, col;
    int counter;
    counter = 0;

    for( row = 0; row < TOP_LINE; ++row )
        for( col = 0; col < MATRIX_W; ++col )
            if( previewGamefield[ row ][ col ] != 0 )
                if( ( counter == 0 ) || ( counter > smallerIntervall( row, col, pg->gameField ) ) )
                    counter = smallerIntervall( row, col, pg->gameField );

    for( row = 0; row < MATRIX_H_PREV; ++row )
        for( col = 0; col < MATRIX_W; col++ )
            if( previewGamefield[ row ][ col ] != 0 )
                pg->gameField[ row + counter ][ col ] = previewGamefield[ row ][ col ];

    colorField( pg );
}

int smallerIntervall( int row, int col, int gamefield[][ MATRIX_W ] )
{
    int counter;
    int current_row;
    int current_col;

    counter     = 0;
    current_col = col;

    for( current_row = ( MATRIX_H - 1 ); current_row > row; --current_row )
    {
        if( gamefield[ current_row ][ current_col ] != 0 )
            counter = 0;
        else if( gamefield[ current_row ][ current_col ] == 0 )
            counter++;
    }

    return counter;
}

int checkDeleteRows( player *pg )
{
    int row, col;
    int counterRows;
    int isDeleteRow;
    int counterNumbers;


    counterRows = 0;
    isDeleteRow = false;
    keypad(pg->window, FALSE);

    for( row = ( MATRIX_H - 1 ); row >= TOP_LINE; --row )
    {
        isDeleteRow = false;
        counterNumbers = 0;

        for( col = 0; col < MATRIX_W; ++col )
            if( pg->gameField[ row ][ col ] != 0 )
                counterNumbers += 1;

        if( counterNumbers == MATRIX_W )
        {
            isDeleteRow = true;
            counterRows += 1;
            halfdelay( 10 );
            goDownTetramini( row, pg->gameField );
            colorField( pg );
        }

        if ( isDeleteRow == true )
            row += 1;
    }

    return calculateScoring( counterRows );
}

int checkGameOver( int gamefield[][ MATRIX_W ] )
{
    int col;

    for( col = 0; col < MATRIX_W; ++col )
        if( gamefield[ TOP_LINE - 1 ][ col ] != 0 )
            return ( 1 );

    return ( 0 );
}

void goDownTetramini( int row, int gamefield[][ MATRIX_W ] )
{
    int currentRow;
    int cols;
    int box;
    int nextBox;

    box = nextBox = 0;

    for( cols = 0; cols < MATRIX_W; ++cols )
        gamefield[ row ][ cols ] = 0;

    for( currentRow = ( row - 1 ); currentRow >= TOP_LINE; --currentRow)
        for( cols = 0; cols < MATRIX_W; ++cols )
        {
            box = gamefield[ currentRow ][ cols ];
            nextBox = gamefield[ currentRow + 1 ][ cols ];

            if( ( box != 0 ) && ( nextBox == 0 ) )
            {
                gamefield[ currentRow + 1 ][ cols ] = box;
                gamefield[ currentRow ][ cols ] = 0;
            }
        }
}
