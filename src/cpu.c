/**
 * @file   cpu.c
 * @author Jacopo Costantini Matteo Zambon Alvise Silvestri
 * @date   Fri Jan 28 02:21:51 2022
 *
 * @brief Questo è il file del grande cervello
 *
 *
 */


#include "multiPlayer.h"
#include "cpu.h"
#include "commonConfig.h"
#include "singlePlayer.h"
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

/**
 * Qui vengono colorati i tetramini della preview
 * sulla window della cpu
 *
 */
void colorFieldCPU( int gameField[][ MATRIX_W ], player *cpu )
{
    int i, j;
    werase( cpu->window );        /**< La funzione pulisce la window */

    for( i = 0; i < MATRIX_H; ++i )
    {
        for( j = 0; j < MATRIX_W; ++j )
        {
            if( gameField[ i ][ j ] != 0 )
            {
                wattron( cpu->window, COLOR_PAIR ( gameField[ i ][ j ] + 9 ) );
                mvwprintw( cpu->window, i, j*2, "  " );
                wattroff( cpu->window, COLOR_PAIR ( gameField[ i ][ j ] + 9 ) );
            }
        }
    }
    /**
     * Disegna la Top line che non si può superare
     * in game
     *
     * @param window
     *
     * @return
     */
    initTopLine ( cpu->window );
    wrefresh ( cpu->window );
}

/**
 * verifica se l'ultima riga è occupata per determinare
 * il tipo di mossa
 *
 *
 * @return
 */
int checkLastRow( int gamefield[][ MATRIX_W ] )
{
    int row, cols;

    cols = 0;
    row = ( MATRIX_H - 1);

    for ( cols = 0; cols < MATRIX_W; ++cols )
        if ( gamefield[ row ][ cols ] != 0 )
            return ( 1 );

    return ( 0 );
}

/**
 * Questa funzione fa cadere il tetramino
 * scelto dalla cpu
 *
 * @param cpu
 */
void fallingAI ( player *cpu )
{
    int row, col;
    int counter;

    counter = 0;

    for ( row = 0; row < MATRIX_H_PREV; ++row )
        for ( col = 0; col < MATRIX_W; ++col )
            if ( previewGamefield[ row ][ col ] != 0 )
                if ( ( counter == 0 ) || ( counter > smallerIntervall ( row, col, cpu->gameField ) ) )
                    counter = smallerIntervall ( row, col, cpu->gameField );

    for ( row = 0; row < MATRIX_H_PREV; ++row )
        for ( col = 0; col < MATRIX_W; ++col )
            if ( previewGamefield[ row ][ col ] != 0 )
                cpu->gameField[ ( row + counter ) ][ col ] = previewGamefield[ row ][ col ];
    /**
     * Colora la window basandosi sul
     * proprio campo di gioco
     *
     * @param cpu
     *
     * @return
     */
    colorField ( cpu );
}

/**
 * Questa è la mossa della CPU quando il campo è
 * completamente vuoto
 *
 * @param cpu
 * @param tetraminos
 */
void opening( player *cpu, int tetraminos[] )
{
    int i;
    int tet_type;
    int tet_ori;
    int offset;

    resetPreview ();
    wrefresh ( cpu->window );
    srand ( time ( NULL ) );

    tet_type = rand() % T_NUM;
    tet_ori  = rand() % T_ORI;
    offset   = rand() % 7;

    for ( i = 0; i < TETS_CELL; ++i)
    {
        cell = TETROMINOS[ tet_type ][ tet_ori ][ i ];
        previewGamefield[ cell.row ][ ( cell.col + offset ) ] = ( tet_type + 1 );
    }

    colorFieldCPU ( previewGamefield, cpu );
    fallingAI ( cpu );
    tetraminos[ tet_type ] -= 1;  /**< Sottrae all'array dei tetramini il tetramino scelto */

    wrefresh ( cpu->window );
}

/**
 * Questa funzione sceglie grazie ai parametri passati
 * la scelta migliore del tetramino con la corrispondente
 * orientazione
 *
 * @param zeros
 * @param tet_type
 * @param tet_ori
 * @param start
 * @param offset
 * @param tetLeft
 */
void fillPossibleChoices( int zeros, int* tet_type, int* tet_ori, int start, int* offset, int tetLeft[] )
{
    tet possible_choiches[ 7 ];

    if ( zeros == 1 )
    {
        int isSelected = false;

        while ( isSelected != true )
        {
            int randChoice;
            randChoice = rand() % 7;

            if ( ( randChoice == 0 ) && ( tetLeft[0] > 0 ) )
            {
                *tet_type  = possible_choiches[ 0 ].tet = 0;
                *tet_ori   = possible_choiches[ 0 ].ori = 1;
                isSelected = true;
            }

            if ( ( randChoice == 1 ) && ( start != 9 ) && ( tetLeft[ 1 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 1 ].tet = 1;
                *tet_ori   = possible_choiches[ 1 ].ori = 1;
                isSelected = true;
            }

            if ( ( randChoice == 2 ) && ( start <= 7 ) && ( tetLeft[ 2 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 2 ].tet = 2;
                *tet_ori   = possible_choiches[ 2 ].ori = 0;
                isSelected = true;
            }

            if ( ( randChoice == 3 ) && ( start != 0 ) && ( tetLeft[ 4 ] > 0 ) )
            {
                *tet_type   = possible_choiches[ 3 ].tet = 4;
                *tet_ori    = possible_choiches[ 3 ].ori = 1;
                *offset    -= 1;
                isSelected  = true;
            }

            if ( ( randChoice == 4 ) && ( start <= 8 ) && ( tetLeft[ 6 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 4 ].tet = 6;
                *tet_ori   = possible_choiches[ 4 ].ori = 1;
                isSelected = true;
            }

            if ( ( randChoice == 5 ) && ( start != 0 ) && ( tetLeft[ 5 ] > 0 ) )
            {
                *tet_type   = possible_choiches[ 5 ].tet = 5;
                *tet_ori    = possible_choiches[ 5 ].ori = 1;
                *offset    -= 1;
                isSelected  = true;
            }

            if ( ( randChoice == 6 ) &&  (start != 9 ) && ( tetLeft[ 5 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 6 ].tet = 5;
                *tet_ori   = possible_choiches[ 6 ].ori = 3;
                isSelected = true;
            }
        }
    }

    if ( zeros == 2 )
    {
        int isSelected = false;

        while ( isSelected != true )
        {
            int randChoice;
            randChoice =rand() % 5;

            if ( ( randChoice == 0 ) && ( tetLeft[ 0 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 0 ].tet = 1;
                *tet_ori   = possible_choiches[ 0 ].ori = 3;
                isSelected = true;
            }

            if ( ( randChoice == 1 ) && ( tetLeft[ 1 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 1 ].tet = 2;
                *tet_ori   = possible_choiches[ 1 ].ori = 1;
                isSelected = true;
            }

            if ( ( randChoice == 2 ) && ( tetLeft[ 2 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 2 ].tet = 3;
                *tet_ori   = possible_choiches[ 2 ].ori = 0;
                isSelected = true;
            }

            if ( ( randChoice == 3 ) && ( start <= 7 ) && ( tetLeft[ 4 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 3 ].tet = 4;
                *tet_ori   = possible_choiches[ 3 ].ori = 0;
                isSelected = true;
            }

            if ( ( randChoice == 4 ) && ( start != 0 ) && ( tetLeft[ 6 ] > 0 ) )
            {
                *tet_type   = possible_choiches[ 4 ].tet = 6;
                *tet_ori    = possible_choiches[ 4 ].ori = 0;
                *offset    -= 1;
                isSelected  = true;
            }
        }
    }

    if ( zeros == 3 )
    {
        int isSelected = false;

        while ( isSelected != true )
        {
            int randChoice;
            randChoice = rand() % 3;

            if ( ( randChoice == 0 ) && ( tetLeft[ 1 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 0 ].tet = 1;
                *tet_ori   = possible_choiches[ 0 ].ori = 2;
                isSelected = true;
            }

            if ( ( randChoice == 1 ) && ( tetLeft[ 2 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 1 ].tet = 2;
                *tet_ori   = possible_choiches[ 1 ].ori = 2;
                isSelected = true;
            }

            if ( ( randChoice == 2 ) && ( tetLeft[ 5 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 2 ].tet = 5;
                *tet_ori   = possible_choiches[ 2 ].ori = 2;
                isSelected = true;
            }
        }
    }

    if ( zeros >= 4 )
    {
        int isSelected = false;

        while ( isSelected != true )
        {
            int randChoice;
            randChoice = rand() % 4;

            if ( ( randChoice == 0 ) && ( tetLeft[ 1 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 0 ].tet = 1;
                *tet_ori   = possible_choiches[ 0 ].ori = 2;
                isSelected = true;
            }

            if ( ( randChoice == 1 ) && ( tetLeft[ 2 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 1 ].tet = 2;
                *tet_ori   = possible_choiches[ 1 ].ori = 2;
                isSelected = true;
            }
            if ( ( randChoice == 2 ) && ( tetLeft[ 5 ] > 0) )
            {
                *tet_type  = possible_choiches[ 2 ].tet = 5;
                *tet_ori   = possible_choiches[ 2 ].ori = 2;
                isSelected = true;
            }
            if ( ( randChoice == 3 ) && ( tetLeft[ 0 ] > 0 ) )
            {
                *tet_type  = possible_choiches[ 0 ].tet = 0;
                *tet_ori   = possible_choiches[ 0 ].ori = 0;
                isSelected = true;
            }
        }
    }
}

/**
 * Questa funzione controlla il numero numero maggiore
 * di 1 consecutivi
 *
 * @param row
 *
 * @return
 */
int fullConsecutive( int row, int gamefield[][ MATRIX_W ] )
{
    int cols;
    int counter;
    int max;

    counter = 0;
    max     = 0;

    for ( cols = 0; cols < ( MATRIX_W - 1 ); ++cols )
        if ( ( gamefield[ row ][ cols ] != 0 ) && ( gamefield[ row ][ ( cols + 1 ) ] != 0 ) )
            ++counter;

    if ( counter > max )
        return counter;

    return max;
}

/**
 * Questa funzione controlla se le righe sopra la
 * colonna più completabile siano vuote, se no
 * restituisce che la righa più completabile non
 * è la selezionata
 *
 * @param row
 *
 * @return
 */
int isCompletable( int row, int gamefield[][ MATRIX_W ] )
{
    int cols;
    int currentRow;

    for ( currentRow = row; currentRow > 5; --currentRow )
        for ( cols = 0; cols < MATRIX_W; ++cols )
            if ( ( gamefield[ ( currentRow - 1 ) ][ cols ] != 0 ) && ( gamefield[ currentRow ][ cols ] == 0 ) )
                return ( false );
    return ( true );
}

/**
 * Questa funzione trova lo spot migliore dove inserire il tetramino
 *
 * @param bestRow
 *
 * @return
 */
int completionSpot( int bestRow, int gamefield[][ MATRIX_W ], int* start )
{
    int cols;
    int counterMinZeros;
    int minZeros;

    counterMinZeros = minZeros = 0;

    for ( cols = 0; cols < MATRIX_W; ++cols )
    {
        if ( gamefield[ bestRow ][ cols ] == 0 )
            ++counterMinZeros;

        if ( ( gamefield[ bestRow ][ cols ] != 0 ) || ( cols == ( MATRIX_W - 1 ) ) )
        {
            if ( ( !minZeros ) || ( ( counterMinZeros < minZeros ) && ( counterMinZeros > 0 ) ) )
            {
                minZeros = counterMinZeros;

                if ( ( cols == ( MATRIX_W - 1 ) ) && ( gamefield[ bestRow ][ cols ] == 0 ) )
                    *start = ( MATRIX_W - minZeros );
                else
                    *start = ( cols - minZeros );
            }

            counterMinZeros = 0;
        }
    }

    return ( minZeros );
}

/**
 * Analizza la riga per trovare se sia la più completabile
 *
 * @param cpu
 *
 * @return
 */
int analizeRows( player *cpu )
{
    int row;
    int counter;
    int best_row;
    int firstCompletable;

    counter = best_row = 0;
    firstCompletable = 6;

    for ( row = 24; row >= 5; --row )
        if( ( isCompletable( row, cpu->gameField ) ) && ( counter <= fullConsecutive( row, cpu->gameField ) ) )
        {
            if ( firstCompletable == 6 )
                firstCompletable = row;

            counter = fullConsecutive( row, cpu->gameField );
            best_row = row;
        }

    return ( best_row );
}

/**
 * Il grande cervello della nostra CPU
 * Con il suo algoritmo cerca di essere competitiva
 *
 *
 * @param cpu
 * @param bestRow
 * @param zeroInterval
 * @param tetraminos
 */
void bigBrain( player *cpu, int tetraminos[] )
{
    /* init some handy variable */
    int i;
    int start;
    int firstBestRow;
    int firstClearSpots;

    /*rand*/
    int tet_type;
    int tet_ori;
    int offset;

    start           = 0;
    firstBestRow    = 0;
    firstClearSpots = 0;
    tet_type        = 0;
    tet_ori         = 0;
    offset          = 0;

    if ( !checkLastRow( cpu->gameField ) )
        opening( cpu, tetraminos );
    else
    {
        resetPreview();

        {
            firstBestRow    = analizeRows( cpu );
            firstClearSpots = completionSpot( firstBestRow, cpu->gameField, &start );

            offset = start;
            fillPossibleChoices( firstClearSpots, &tet_type, &tet_ori, start, &offset, tetraminos );

            {
                resetPreview();
                wrefresh( cpu->window );
                for( i = 0; i < TETS_CELL; ++i)
                {
                    cell = TETROMINOS[ tet_type ][ tet_ori ][ i ];
                    previewGamefield[ cell.row ][ ( cell.col + offset ) ] = ( tet_type + 1 );
                }

                colorFieldCPU( previewGamefield, cpu );
                fallingAI( cpu );
                tetraminos[ tet_type ] -= 1;
            }
        }
    }
}

/**
 * Questa funzione aggiorna il valore degli score
 *
 * @param pieces
 * @param pg1Score
 * @param pg2Score
 * @param score
 */
void refreshCpuScore( int pieces, int *pg1Score, int *pg2Score, WINDOW* score )
{
    werase( score );

    mvwprintw( score, 1, 1, "Disponibili: "   );
    mvwprintw( score, 1, 14, "%d", pieces     );
    mvwprintw( score, 3, 1, "Punteggio PG1: " );
    mvwprintw( score, 4, 1, "Punteggio CPU: " );
    mvwprintw( score, 3, 16, "%d", *pg1Score  );
    mvwprintw( score, 4, 16, "%d", *pg2Score  );

    wrefresh( score );
}

/**
 * Questo è la routine della nostra modalità player vs cpu
 *
 * @param pg1
 * @param cpu
 * @param piece
 * @param tetPieces
 * @param s_preview
 * @param s_score
 */
void startCpuGame( player *pg1, player* cpu, tet *piece, int* tetPieces,
                   WINDOW* s_preview, WINDOW* s_score,
                   WINDOW* w_title, WINDOW* w_field, WINDOW* w_sfield, WINDOW* w_preview,
                   WINDOW* w_score, WINDOW* w_save, WINDOW* w_cmds )
{
    int pieces;
    int quit;
    int choice;
    int countCurrentPiece;
    int position_x;
    int *currentScore = &pg1->score;
    int *cpuScore     = &cpu->score;

    tet preview_piece = { ( piece->tet + 1 ), 0 };

    quit          = false;
    choice        = 0;
    position_x    = 0;
    pieces        = ( T_NUM * ( T_PIECES * 2 ) );

    refreshPreview( s_preview, &preview_piece );
    refreshGameField( &position_x, piece, pg1 );
    refreshCpuScore( tetPieces[ piece->tet ], currentScore, cpuScore, s_score );

    do
    {
        keypad( s_preview, TRUE );
        choice = wgetch( s_preview );
        colorField( pg1 );

        countCurrentPiece = tetPieces[ piece->tet ];
        refreshCpuScore( tetPieces[ piece->tet ], currentScore, cpuScore, s_score );

        if ( countCurrentPiece == 0 )
            changePiece( s_score );

        switch ( choice )
        {
            case 'n':
                nextPiece( piece, &preview_piece );
                refreshCpuScore( tetPieces[ piece->tet ], currentScore, cpuScore, s_score );
                break;

             case 'h':
                paintHelp();
                initMultiField( w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );
                initFirstPlayerWindow( pg1->window );
                initSecondPlayerWindow( cpu->window );
                break;

            /* case: back piece */
            case 'b':
                backPiece( piece, &preview_piece );
                refreshCpuScore( tetPieces[ piece->tet ], currentScore, cpuScore, s_score );
                break;

            /* case: rotate piece */
            case 'r':
                rotatingPiece( piece );
                break;

            case KEY_LEFT:
                position_x -= 2;
                break;

            case KEY_RIGHT:
                position_x += 2;
                break;

            case KEY_DOWN:
                if ( countCurrentPiece )
                {
                    fallingPiece( pg1 );

                    tetPieces[ piece->tet ] -= 1;
                    pieces                  -= 1;

                    bigBrain( cpu, tetPieces );
                    *currentScore += checkAndReverseRows( pg1, cpu );
                    *cpuScore     += checkAndReverseRows( cpu, pg1 );

                    wrefresh( cpu->window );
                    refreshCpuScore( tetPieces[ piece->tet ], currentScore, cpuScore, s_score );
                }
                break;

            case 'q':
                quit = true;
                break;

            default:
                break;
        }

        if ( checkGameOver( pg1->gameField ) )
        {
            initGameOver( pg1->score );
            return ;
        }
        else if ( checkGameOver( cpu->gameField ) )
        {
            initGameOver( cpu->score );
            return ;
        }

        if ( ( pieces == 0 ) || ( quit ) )
        {
            initGameOver( pg1->score );
            return ;
        }

        colorField( pg1 );
        colorField( cpu );

        refreshPreview( s_preview, &preview_piece );
        refreshGameField( &position_x, piece, pg1 );

    } while ( ( pieces ) && ( !quit ) );

    return ;
}

/**
 * La funzione principale che inizializza i principali parametri
 *
 *
 * @return
 */
int CPU()
{
    WINDOW *w_title = NULL, *w_field = NULL, *w_sfield = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview = NULL, *s_score = NULL;

    int tetPieces[T_NUM];
    player pg1;
    player cpu;

    tet piece = { 0, 0 };
    pg1   = addPlayer();
    cpu   = addPlayer();

    /* clear the screen to visualize a new windows */
    clear();
    cbreak();

    initGameMatrix( pg1.gameField );
    initGameMatrix( cpu.gameField );

    initTetVector( tetPieces, 1 );
    initMultiField( w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );

    pg1.window = initFirstPlayerWindow( pg1.window );
    cpu.window = initSecondPlayerWindow( cpu.window );
    s_preview = initMultiPreviewWindow( s_preview );
    s_score = initMultiScoreWindow( s_score );

    startCpuGame( &pg1, &cpu, &piece, tetPieces, s_preview, s_score, w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );

    refresh();
    getch();

    return ( 3 );
}
