/**
 * @file   multiPlayer.c
 * @author Jacopo Costantini <jacopocostantini32@gmail.com>
 * @date   Thu Jan 27 23:33:50 2022
 *
 * @brief  Multiplayer routine
 *
 * Qui si definisce la routine che verrà eseguita per il multi player
 *
 */


#include "multiPlayer.h"
#include "commonConfig.h"
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

/**
 * Questa è la funzione che crea le schermate di gioco
 * Quando termina si ritorna alla funzione main()
 *
 * @brief retrun score
 * @return 1
 */
int multiPlayer()
{

    WINDOW *w_title = NULL, *w_field = NULL, *w_sfield = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL; /**< Qui si inizializzano tutte le window statiche */
    WINDOW *s_preview = NULL, *s_score = NULL; /**< Qui si inizializzano tutte le window dinamiche */

    /**
     * Pulisce lo schermo per visualizzare le nuove window
     *
     *
     * @return blank stdscr window
     */
    clear();

    /**
     * Disabilita la possibilità di uscire con un cambinazioni di tasti ctrl
     */
    cbreak();

    int turn;
    int lastAction;
    int pieces;
    int tetPieces[ T_NUM ];
    tet piece = {0,0};

    player pg1;   /**< Aggiungo il player 1 */
    player pg2;  /**< Aggiungo il player 2 */

    turn = 0;
    lastAction = 0;
    pieces = ( T_NUM * ( T_PIECES * 2 ) );

    pg1 = addPlayer();
    pg2 = addPlayer();

    /**
     * Inizializza la matrice di gioco tutta a 0
     *
     * @param gameField
     *
     */
    initGameMatrix( pg1.gameField );
    initGameMatrix( pg2.gameField );

    /**
     * Inizializza il vettore del tetramino
     *
     * @param tetPieces
     * @param 0
     *
     */
    initTetVector( tetPieces, 1 );
    /**
     * Stampa l'intero campo di gioco con tutte le window statiche
     *
     * @param w_title
     * @param w_field
     * @param w_preview
     * @param w_sfield
     * @param w_score
     * @param w_save
     * @param w_cmds
     *
     * @return
     */
    initMultiField( w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );

    pg1.window = initFirstPlayerWindow( pg1.window ); /**< Da le info per modellare la window, tutti i suoi parametri */
    pg2.window = initSecondPlayerWindow( pg2.window ); /**< Da le info per modellare la window, tutti i suoi parametri */

    s_preview = initMultiPreviewWindow( s_preview ); /**< Da le info per modellare la window dinamica preview */
    s_score = initMultiScoreWindow( s_score ); /**< Da le info per modellare la window dinamica score */

    /**
     * L'ultima azione
     *
     * @param pg1
     * @param pg2
     * @param piece
     * @param tetPieces
     * @param pieces
     * @param turn
     * @param s_preview
     * @param s_score
     *
     */
    lastAction = startTheGame( &pg1, &pg2, &piece, tetPieces, &pieces, &turn, s_preview, s_score, w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );

    /**
     * Vincitore
     *
     * @param pg1
     * @param pg2
     *
     * @return
     */
    if ( lastAction == 1 )
        initWinner( &pg1, &pg2 );

    refresh();
    getch();

    return ( 1 );
}

/**
 * Routine del gioco
 *
 * @param pg1
 * @param pg2
 * @param piece
 * @param tetPieces
 * @param pieces
 * @param turn
 * @param preview
 * @param score
 *
 * @return
 */
int startTheGame( player* pg1, player* pg2, tet* piece, int* tetPieces, int* pieces, int* turn, WINDOW*s_preview, WINDOW* s_score, WINDOW* w_title, WINDOW* w_field, WINDOW* w_sfield, WINDOW* w_preview, WINDOW* w_score, WINDOW* w_save, WINDOW* w_cmds )
{
    int choice;
    int action;
    int positionX;
    int countCurrentPiece;

    int *currentScore  = &pg1->score;
    tet  preview_piece = { piece->tet + 1, 0 };

    choice            = 0;
    action            = 0;
    positionX         = 0;
    countCurrentPiece = 0;

    if ( *pieces == 0 )
        return ( 1 );

    refreshPreview( s_preview, &preview_piece ); /**< Aggiorno i valori in preview e ridisegna la sua window dinamica */
    refreshGameField( &positionX, piece, pg1 ); /**< Aggiorno i valori in gamefield e ridisegna la sua window dinamica */
    refreshMultiScore( tetPieces[ piece->tet ], currentScore, pg2->score, turn, s_score ); /**< Aggiorno i valori in score e ridisegna la sua window dinamica */

    do /**< Inizia la sequenza di istruzioni da eseguire */
    {
        keypad( pg1->window, TRUE ); /**< Inzializza la funzionalità di input da tastiera */

         /**
         * Coloro il campo con i valori salvati nella matrice di gioco
         *
         * @param pg
         *
         */
        colorField( pg2 );

        choice = wgetch( pg1->window ); /**< Prendo l'input da tastiera da parte dell'utente */

        colorField( pg1 );

        countCurrentPiece = tetPieces[ piece->tet ];
        refreshMultiScore( tetPieces[ piece->tet ], currentScore, pg2->score, turn, s_score ); /**< Aggiorno i valori in score e ridisegna la sua window dinamica */

        if ( countCurrentPiece == 0 )
            changePiece( s_score );

        switch ( choice ) /**< Casistica della scelta dell'utente */
        {
            case 'n':
                /**
                 * Cambio il pezzo corrente con il prossimo
                 *
                 * @param preview_piece
                 * @param preview_piece
                 *
                 * @return next piece
                 */
                nextPiece( piece, &preview_piece );
                refreshMultiScore( tetPieces[ piece->tet ], currentScore, pg2->score, turn, s_score );
                break;

            case 'h':
                paintHelp();
                initMultiField( w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );
                initFirstPlayerWindow( pg1->window );
                initSecondPlayerWindow( pg2->window );
                refreshGameField( &positionX, piece, pg2 );
                refreshGameField( &positionX, piece, pg1 );
                break;

            /* case: back piece */
            case 'b':
                /**
                 * Cambio pezzo con quello precedente nell'array di tetramini
                 *
                 * @param piece
                 * @param preview_piece
                 *
                 * @return
                 */
                backPiece( piece, &preview_piece );
                refreshMultiScore( tetPieces[ piece->tet ], currentScore, pg2->score, turn, s_score );
                break;

            /* case: rotate piece */
            case 'r':
                 /**
                 * Ruoto di 90 gradi il tetramino
                 *
                 * @param current_piece
                 *
                 */
                rotatingPiece( piece );
                break;

            case KEY_LEFT:
                positionX -= 2; /**< Muovo il tetramino nella preview di una posizione a destra */
                break;

            case KEY_RIGHT:
                positionX += 2; /**< Muovo il tetramino nella preview di una posizione a sinistra */
                break;

            case KEY_DOWN:
                if ( countCurrentPiece ) /**< Se la condizione è soddisfatta faccio cadere il pezzo */
                {
                    /**
                     * Fa cadere il tetramino scelto
                     *
                     * @param pg1
                     *
                     * @return
                     */
                    fallingPiece( pg1 );

                    tetPieces[ piece->tet ] -= 1; /**< Decremento il tetraminno corrente di 1 */
                    *pieces                 -= 1; /**< Decremento il numero generale di tetramini disponibili */

                    *currentScore += checkAndReverseRows( pg1, pg2 );
                    refreshMultiScore( tetPieces[ piece->tet ], currentScore, pg2->score, turn, s_score ); /**< Riscrivo lo score con i nuovi valori */
                    action = 1;
                }
                break;

            case 'q':
                initMultiQuit(); /**< Esco dal while */
                return ( 0 );
                break;

            default:
                break;
        }

        colorField( pg1 );

        refreshPreview( s_preview, &preview_piece );
        refreshGameField( &positionX, piece, pg1 );
    } while ( !action );

    if ( checkGameOver( pg1->gameField ) )
    {
        multiGameOver( pg2->score, turn );
        return ( 0 );
    }

    colorField( pg1 );

    /**
     * Ricorsione della funzione startGame
     *
     * @param pg2
     * @param pg1
     * @param piece
     * @param tetPieces
     * @param pieces
     * @param changeTurn
     * @param preview
     * @param score
     *
     * @return
     */
    return startTheGame( pg2, pg1, piece, tetPieces, pieces, changeTurn( turn ), s_preview, s_score , w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds );

}

/**
 * Inizializzo il titolo della window
 *
 * @param title
 */
void initMultiPlayerTitle( WINDOW* title )
{
    int titleY, titleX;

    titleY = ( HCENTER -  MTITLE_H - 1  );
    titleX = (  (WCENTER - 3)  - SCORE_W );

    title = newwin( MTITLE_H, MTITLE_W, titleY, titleX );
    box( title, V_LINES, H_LINES );
    wbkgd( title, COLOR_PAIR( 2 ) );

    mvwprintw( title, 1, 31, "MULTI PLAYER" );
    wrefresh( title );
}

/**
 * Inizializzo il field della window
 *
 * @param firstField
 * @param secondField
 */
void initMultiPlayerField( WINDOW* firstField, WINDOW* secondField )
{
    int fieldY, fieldX;
    int sfieldY, sfieldX;

    fieldY = HCENTER;
    fieldX = ( WCENTER - SCORE_W );

    sfieldY = ( ( LINES  - FIELD_H ) / 2);
    sfieldX = ( ( fieldX + FIELD_W ) + ( SCORE_W + 4 ) );

    firstField = newwin( FIELD_H, FIELD_W, fieldY, fieldX );
    box( firstField, V_LINES, H_LINES );
    wbkgd( firstField, COLOR_PAIR( 2 ) );

    secondField = newwin( FIELD_H, FIELD_W, sfieldY, sfieldX );
    box( secondField, V_LINES, H_LINES );
    wbkgd( secondField, COLOR_PAIR( 2 ) );

    wrefresh(firstField);
    wrefresh( secondField );
}


/**
 * Inizializzo il cmds della window
 *
 * @param cmds
 */
void initMultiPlayerCmds( WINDOW *cmds )
{
    int cmdsY, cmdsX;

    cmdsY = ( ( HCENTER +  FIELD_H ) + 1 );
    cmdsX = ( ( WCENTER -  SCORE_W ) - 3 );

    cmds = newwin( MCMDS_H, MCMDS_W, cmdsY, cmdsX );
    box(cmds, V_LINES, H_LINES);
    wbkgd( cmds, COLOR_PAIR( 2 ) );

    mvwprintw( cmds, 0, 31, "| COMMANDS |" );
    mvwprintw( cmds, 1, 10, "[ 'R' rotation ] [ 'N' next piece ] [ < ] [ v ] [ > ]" );

    wrefresh( cmds );
}


/**
 * Inizializzo il preview della window
 *
 * @param preview
 */
void initMultiPlayerPreview( WINDOW* preview )
{
    int previewY, previewX;

    previewY = HCENTER;
    previewX = ( ( WCENTER -  SCORE_W ) +  ( FIELD_W + 2 ) );

    preview = newwin( PREVIEW_H, PREVIEW_W, previewY, previewX );
    box( preview, V_LINES, H_LINES );
    wbkgd( preview, COLOR_PAIR( 2 ) );

    mvwprintw( preview , 0 , 5 , "| PREVIEW |" );

    wrefresh( preview );
}


/**
 * Inizializzo il score della window
 *
 * @param score
 */
void initMultiPlayerScore( WINDOW* score )
{
    int scoreY, scoreX;

    scoreY = ( ( HCENTER +  PREVIEW_H ) + 1  );
    scoreX = ( ( WCENTER -  SCORE_W ) + ( FIELD_W + 2 ) );

    score = newwin( SCORE_H, SCORE_W, scoreY, scoreX );
    box( score , V_LINES, H_LINES );
    wbkgd( score, COLOR_PAIR( 2 ) );

    mvwprintw( score , 0 , 6 , "| SCORE |" );

    wrefresh( score );
}

/**
 * Inizializzo il save della window
 *
 * @param save
 */
void initMultiPlayerSave( WINDOW* save )
{
    int saveY, saveX;

    saveY = ( ( HCENTER +  PREVIEW_H ) + ( SCORE_H + 3 ) );
    saveX = ( ( WCENTER -  SCORE_W   ) + ( FIELD_W + 2 ) );

    save = newwin( SAVE_H, SAVE_W, saveY, saveX );
    box( save, V_LINES, H_LINES );
    wbkgd( save, COLOR_PAIR ( 3 ) );

    mvwprintw( save, 0, 6, "| SAVE |" );
    mvwprintw( save, 1, 1, "'H' to help page" );
    mvwprintw( save, 2, 1, "'S' to save game" );
    mvwprintw( save, 3, 1, "'Q' return to menu" );

    wrefresh( save );
}

/**
 * Inizializzo il tutte le schermate del player
 *
 * @param pgWindow
 */
WINDOW* initFirstPlayerWindow( WINDOW* pgWindow )
{
    int fieldY, fieldX;

    fieldY   = ( HCENTER + 1 );
    fieldX   = ( ( WCENTER -  SCORE_W ) + 1 );

    pgWindow = newwin( ( FIELD_H - 2 ), ( FIELD_W - 2 ), fieldY, fieldX );
    wbkgd( pgWindow, COLOR_PAIR( 0 ) );

    wrefresh( pgWindow );

    return ( pgWindow );
}

/**
 * Inizializzo la preview
 *
 * @param preview
 *
 * @return
 */
WINDOW* initMultiPreviewWindow( WINDOW* preview )
{
    int previewY, previewX;

    previewY = ( HCENTER + 1 );
    previewX = ( ( WCENTER -  SCORE_W ) + ( FIELD_W + 3 ) );

    preview = newwin( ( PREVIEW_H - 2 ), ( PREVIEW_W - 2 ), previewY, previewX );
    wbkgd( preview, COLOR_PAIR( 0 ) );

    wrefresh( preview );

    return ( preview );
}

/**
 * Inizializzo lo score
 *
 * @param score
 *
 * @return
 */
WINDOW* initMultiScoreWindow( WINDOW* score )
{
    int scoreY, scoreX;

    scoreY   = ( ( HCENTER +  PREVIEW_H ) + 2 );
    scoreX   = ( ( WCENTER - SCORE_W ) + ( FIELD_W + 3 ) );

    score = newwin( ( SCORE_H - 2 ), ( SCORE_W - 2 ), scoreY, scoreX );
    wbkgd( score, COLOR_PAIR( 0 ) );

    wrefresh( score );

    return ( score );
}

/**
 * Inizializzo tutti i field
 *
 * @param title
 * @param firstField
 * @param secondField
 * @param preview
 * @param score
 * @param save
 * @param cmds
 */
void initMultiField( WINDOW* title, WINDOW* firstField, WINDOW *secondField, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds )
{
    refresh();

    initMultiPlayerTitle( title );
    initMultiPlayerField( firstField, secondField );
    initMultiPlayerPreview( preview );
    initMultiPlayerScore( score );
    initMultiPlayerSave( save );
    initMultiPlayerCmds( cmds );

    refresh();
}

/**
 * Refresh the multiscore
 *
 * @param pieces
 * @param pg1Score
 * @param pg2Score
 * @param turn
 * @param score
 */
void refreshMultiScore( int pieces, int *pg1Score, int pg2Score,int *turn, WINDOW* score )
{
    werase( score );

    mvwprintw( score, 1, 1, "Disponibili: " );
    mvwprintw( score, 1, 14, "%d", pieces );
    mvwprintw( score, 3, 1, "Punteggio PG1: " );
    mvwprintw( score, 4, 1, "Punteggio PG2: " );

    if ( *turn == FIRST_PLAYER )
    {
        mvwprintw( score, 3, 16, "%d", *pg1Score );
        mvwprintw( score, 4, 16, "%d",  pg2Score );
    }
    if ( *turn == SECOND_PLAYER )
    {
        mvwprintw( score, 4, 16, "%d", *pg1Score );
        mvwprintw( score, 3, 16, "%d",  pg2Score );
    }

    wrefresh( score );
}


/**
 * Inverte le righe se il numero di righe è maggiore o uguale a 3 righe
 *
 */
void reverseRowsGameField( int secondGamefield[][ MATRIX_W ], int counterRows )
{
    int row, col;
    int cell;

    for ( row = ( MATRIX_H - 1 ); row > ( (MATRIX_H - 1) - counterRows ); --row)
        for ( col = 0; col < MATRIX_W; ++col )
        {
            cell = secondGamefield[ row ][ col ];

            if ( cell == 0 )
                secondGamefield[ row ][ col ] = ( ( rand() % ( T_NUM - 1 ) ) + 1 );
            else
                secondGamefield[ row ][ col ] = 0;
        }
}

/**
 * Inverto le righe del campo avversario
 *
 * @param pg1
 * @param pg2
 *
 * @return
 */
int checkAndReverseRows( player* pg1, player* pg2 )
{
    int row, col;
    int counterRows;
    int isDeleteRow;
    int counterNumbers;

    counterRows = 0;
    isDeleteRow = false;

    keypad(pg1->window, FALSE);

    for ( row = ( MATRIX_H - 1 ); row >= TOP_LINE; --row)
    {
        isDeleteRow = false;
        counterNumbers = 0;

        for ( col = 0; col < MATRIX_W; ++col )
            if ( pg1->gameField[ row ][ col ] != 0 )
                counterNumbers += 1;

        if ( counterNumbers == MATRIX_W )
        {
            isDeleteRow = true;
            counterRows += 1;
            halfdelay(10);
            goDownTetramini( row, pg1->gameField );

            colorField( pg1 );
        }

        if ( isDeleteRow == true)
            row += 1;
    }

    if ( counterRows >= 3 )
    {
        reverseRowsGameField( pg2->gameField, counterRows );

        colorField( pg2 );
    }

    return calculateScoring( counterRows );
}

/**
 * Inizializzo il gameover
 *
 * @param score
 * @param turn
 */
void multiGameOver( int score, int* turn )
{
    int starty, startx;
    WINDOW* w_gameover;

    starty = ( (LINES - GAMEOVER_H) / 2 );
    startx = ( (COLS  - GAMEOVER_W) / 2 );

    clear();
    refresh();

    w_gameover = newwin( GAMEOVER_H, GAMEOVER_W, starty, startx );
    box( w_gameover, V_LINES, H_LINES );
    wbkgd( w_gameover, COLOR_PAIR( 3 ) );

    mvwprintw( w_gameover, 0, 21, "| GAME OVER |" );

    if ( *turn == FIRST_PLAYER )
    {
        mvwprintw( w_gameover, 3,  2, "Second player win the game !!" );
        mvwprintw( w_gameover, 5,  2, "Player's score: " );
        mvwprintw( w_gameover, 5,  17, "%d", score );
    }
    else if ( *turn == SECOND_PLAYER )
    {
        mvwprintw( w_gameover, 3,  2, "First player win the game !!" );
        mvwprintw( w_gameover, 5,  2, "Player's score: " );
        mvwprintw( w_gameover, 5,  17, "%d", score );
    }

    mvwprintw( w_gameover, 7, 21, "PRESS ANY KEY" );

    wrefresh( w_gameover );
    refresh();

    getch();
}

/**
 * Funzione di cambio turno
 *
 * @param turn
 *
 * @return
 */
int* changeTurn( int *turn )
{
    if ( *turn == FIRST_PLAYER )
    {
        *turn = SECOND_PLAYER;
        return ( turn );
    }

    if ( *turn == SECOND_PLAYER )
    {
        *turn = FIRST_PLAYER;
        return ( turn );
    }

    return ( turn );
}

/**
 * Inizializzo il quit
 *
 */
void initMultiQuit()
{
    int starty, startx;
    WINDOW* w_quit;

    starty =  ( ( LINES - MQUIT_H ) / 2 );
    startx =  ( ( COLS  - MQUIT_W ) / 2 );

    clear();
    refresh();

    w_quit = newwin( MQUIT_H, MQUIT_W, starty, startx );
    box( w_quit, V_LINES, H_LINES );
    wbkgd( w_quit, COLOR_PAIR( 3 ) );

    mvwprintw( w_quit, 0, 20, "| QUIT |" );
    mvwprintw( w_quit, 3,  2, "IT WAS GREAT PLAY WITH YOU!" );

    wrefresh( w_quit );
    refresh();

    getch();
}

/**
 * Inizializzo il winner
 *
 * @param pg1
 * @param pg2
 */
void initWinner( player *pg1, player *pg2 )
{
    int starty, startx;
    WINDOW* w_winner;

    starty = ( (LINES - MQUIT_H) / 2 );
    startx = ( (COLS  - MQUIT_W) / 2 );

    clear();
    refresh();

    w_winner = newwin( MQUIT_H, MQUIT_W, starty, startx );
    box( w_winner, V_LINES, H_LINES );
    wbkgd( w_winner, COLOR_PAIR( 3 ) );

    mvwprintw( w_winner, 0, 20, "| WINNER |" );

    if ( pg1->score > pg2->score )
    {
        mvwprintw( w_winner, 3,  2, "First player win the game !!" );
        mvwprintw( w_winner, 5,  2, "Player's score: " );
        mvwprintw( w_winner, 5,  17, "%d", pg1->score );
    }
    else if ( pg1->score < pg2->score )
    {
        mvwprintw( w_winner, 3,  2, "Second player win the game !!" );
        mvwprintw( w_winner, 5,  2, "Player's score: " );
        mvwprintw( w_winner, 5,  17, "%d", pg2->score );
    }
    else
        mvwprintw(w_winner, 3,  2, "Congratulations to both of you, no one wins" );

    wrefresh( w_winner );
    refresh();

    getch();
}
