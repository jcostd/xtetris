/**
 * @file   singlePlayer.c
 * @author Jacopo Costantini Matteo Zambon Alvise Silvestri
 * @date   Thu Jan 27 22:42:18 2022
 *
 * @brief  Singleplayer routine
 *
 * Qui si definisce la routine che verrà eseguita per il single player
 *
 *
 */

#include "singlePlayer.h"
#include "commonConfig.h"
#include <ncurses.h>

/**
 * Questa è la funzione che crea le schermate di gioco
 * Quando termina si ritorna alla funzione main()
 *
 * @brief retrun score
 * @return  initReturnToMenu(pg.score)
 */
int singlePlayer()
{
    int choice;
    int pieces;
    int position_x;
    int countCurrentPiece;
    int tetPieces[T_NUM];

    tet current_piece = {0,0};  /**< Current piece contiene le opzioni del pezzo corrente */
    tet preview_piece = {0,0};  /**< Preview piece contiene le opzioni del pezzo mostrato in preview */

    /* declaring the game windows */
    WINDOW *w_title   = NULL, *w_field = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL; /**< Qui si inizializzano tutte le window statiche */
    WINDOW *s_preview = NULL, *s_score = NULL; /**< Qui si inizializzano tutte le window dinamiche */

    choice            = 0;
    position_x        = 0;
    countCurrentPiece = 0;
    pieces            = ( T_NUM * T_PIECES );
    player pg         = addPlayer();


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

    /* initialize the game matrix & init the base screen & creating the refreshable windows */

    /**
     * Inizializza la matrice di gioco tutta a 0
     *
     * @param gameField
     *
     */
    initGameMatrix( pg.gameField );

    /**
     * Inizializza il vettore del tetramino
     *
     * @param tetPieces
     * @param 0
     *
     */
    initTetVector( tetPieces, 0 );
    /**
     * Stampa l'intero campo di gioco con tutte le window statiche
     *
     * @param w_title
     * @param w_field
     * @param w_preview
     * @param w_score
     * @param w_save
     * @param w_cmds
     *
     */
    initField( w_title, w_field, w_preview, w_score, w_save, w_cmds );


    pg.window = initPlayerWindow( pg.window ); /**< Da le info per modellare la window, tutti i suoi parametri */
    s_preview = initPreviewWindow( s_preview ); /**<  Da le informazioni per modellare la window dinamica preview */
    s_score   = initScoreWindow( s_score ); /**< Da le info per modellare la window dinamica score */

    /* Single player routine */

    preview_piece.tet = ( current_piece.tet + 1 ); /**< Aggiorno la preview con il prossimo tetramino */

    refreshPreview( s_preview ,&preview_piece ); /**< Aggiorno i valori in preview e ridisegna la sua window dinamica */
    refreshGameField( &position_x, &current_piece, &pg ); /**< Aggiorna i valori nel gamefield e ridisegna la sua window dinamica */
    refreshScore( s_score ,tetPieces[ current_piece.tet ], pg.score ); /**< Aggiorna i valori dello score e ridisegna la sua window dinamica */

    //@{
    do                          /**< Inizia la sequenza di istruzioni da eseguire */
    {
        keypad( pg.window, TRUE ); /**< Inzializza la funzionalità di input da tastiera */
        choice = wgetch( pg.window ); /**< Prendo l'input da tastiera da parte dell'utente */

        /**
         * Coloro il campo con i valori salvati nella matrice di gioco
         *
         * @param pg
         *
         */
        colorField( &pg );
        countCurrentPiece = tetPieces[ current_piece.tet ]; /**< Conta i pezzi rimanenti dello stesso tetramino */

        refreshScore( s_score, tetPieces[ current_piece.tet ], pg.score );

        if ( countCurrentPiece == 0 )
            changePiece( s_score );

        switch ( choice )         /**< Casistica della scelta dell'utente */
        {
            case 'n':
                /**
                 * Cambio il pezzo corrente con il prossimo
                 *
                 * @param current_piece
                 * @param preview_piece
                 *
                 * @return next piece
                 */
                nextPiece( &current_piece, &preview_piece );

                refreshScore( s_score, tetPieces[ current_piece.tet ], pg.score ); /**< Riscrivo lo score con i nuovi valori */
                break;

            case 'b':
                /**
                 * Cambio pezzo con quello precedente nell'array di tetramini
                 *
                 * @param current_piece
                 * @param preview_piece
                 *
                 * @return previus piece
                 */
                backPiece( &current_piece, &preview_piece );
                refreshScore( s_score, tetPieces[ current_piece.tet ], pg.score ); /**< Riscrivo lo score con i nuovi valori */
                break;

            case 'r':
                /**
                 * Ruoto di 90 gradi il tetramino
                 *
                 * @param current_piece
                 *
                 */
                rotatingPiece( &current_piece );
                break;

            case 'h':
                paintHelp();

                initField( w_title, w_field, w_preview, w_score, w_save, w_cmds );
                initPlayerWindow( pg.window );
                break;

            case KEY_RIGHT:
                position_x += 2; /**< Muovo il tetramino nella preview di una posizione a destra */
                break;

            case KEY_LEFT:
                position_x -= 2; /**< Muovo il tetramino nella preview di una posizione a sinistra */
                break;

            case KEY_DOWN:
                if ( countCurrentPiece ) /**< Se la condizione è soddisfatta eseguo le seguenti azioni */
                {
                    /**
                     * Fa cadere il tetramino scelto
                     *
                     * @param pg
                     *
                     * @return
                     */
                    fallingPiece( &pg );

                    tetPieces[ current_piece.tet ] -= 1; /**< Decremento il tetraminno corrente di 1 */
                    pieces -= 1; /**< Decremento il numero generale di tetramini disponibili */
                    /**
                     * Controllo se è possibile eliminare una o più righe nel campo
                     *
                     * @param pg
                     *
                     * @return
                     */
                    pg.score += checkDeleteRows( &pg );

                    refreshScore( s_score, tetPieces[ current_piece.tet ], pg.score ); /**< Riscrivo lo score con i nuovi valori */
                }
                break;

            case 'q':
                initQuit( pg.score ); /**< Esco dal while */
                return ( 1 );
                break;

            default:
                break;
        }

        if ( checkGameOver( pg.gameField ) ) /**< Se la condizione è soddisfatta il gioco va in gameover */
        {
            initGameOver( pg.score );
            return ( 0 );
        }

        colorField( &pg );

        refreshPreview( s_preview, &preview_piece );
        refreshGameField( &position_x, &current_piece, &pg );

    } while( pieces );
    //@}

    return ( initReturnToMenu( pg.score ) ); /**< Ritorno al menu */
}

//* Init the base field funcs ************************************************/

/**
 * Inizializzo il titolo della window
 *
 * @param title
 */
void initSinglePlayerTitle( WINDOW* title )
{
    int titleY, titleX;

    titleY = ( ( HCENTER - TITLE_H ) - 1);
    titleX = ( ( WCENTER - 3 ) - ( SCORE_W / 2 ) );

    title = newwin( TITLE_H, TITLE_W, titleY, titleX );
    box( title, V_LINES, H_LINES );
    wbkgd( title, COLOR_PAIR( 2 ) );

    mvwprintw( title, 1, 20, "SINGLE PLAYER" );
    wrefresh( title );
}

/**
 * Inizializzo il field della window
 *
 * @param field
 */
void initSinglePlayerField( WINDOW* field )
{
    int fieldY, fieldX;

    fieldY = HCENTER;
    fieldX = ( WCENTER - ( SCORE_W / 2 ) );

    field = newwin( FIELD_H, FIELD_W, fieldY, fieldX );
    box( field, V_LINES, H_LINES );
    wbkgd( field, COLOR_PAIR( 2 ) );

    wrefresh(field);
}

/**
 * Inizializzo il cmds della window
 *
 * @param field
 */
void initSinglePlayerCmds( WINDOW* cmds )
{
    int cmdsY, cmdsX;

    cmdsY = ( ( HCENTER + FIELD_H ) + 1 );
    cmdsX = ( ( WCENTER - 3 ) - ( SCORE_W / 2 ) );

    cmds = newwin( CMDS_H, CMDS_W, cmdsY, cmdsX );
    box( cmds, V_LINES, H_LINES);
    wbkgd( cmds, COLOR_PAIR( 2 ) );

    mvwprintw( cmds, 0, 19, "| COMMANDS |" );
    mvwprintw( cmds, 1, 9, "[ 'R' ] [ 'N' ] [ < ] [ v ] [ > ]" );

    wrefresh( cmds );
}

/**
 * Inizializzo il preview della window
 *
 * @param field
 */
void initSinglePlayerPreview( WINDOW* preview )
{
    int previewY, previewX;

    previewY = HCENTER;
    previewX = ( ( ( WCENTER + FIELD_W ) - (SCORE_W / 2) )  + 2 );

    preview = newwin( PREVIEW_H, PREVIEW_W, previewY, previewX );
    box( preview, V_LINES, H_LINES );
    wbkgd( preview, COLOR_PAIR( 2 ) );

    mvwprintw( preview , 0 , 5 , "| PREVIEW |" );

    wrefresh( preview );
}

/**
 * Inizializzo il score della window
 *
 * @param field
 */
void initSinglePlayerScore( WINDOW* score )
{
    int scoreY, scoreX;

    scoreY = ( HCENTER +PREVIEW_H + 1 );
    scoreX = ( ( ( WCENTER + FIELD_W ) - ( SCORE_W / 2 ) ) + 2);

    score = newwin( SCORE_H, SCORE_W, scoreY, scoreX );
    box( score , V_LINES, H_LINES );
    wbkgd( score, COLOR_PAIR( 2 ) );

    mvwprintw( score , 0 , 6 , "| SCORE |" );

    wrefresh( score );
}

/**
 * Inizializzo il save della window
 *
 * @param field
 */
void initSinglePlayerSave( WINDOW *save )
{
    int saveY, saveX;

    saveY = ( HCENTER + PREVIEW_H + SCORE_H + 3);
    saveX = ( ( ( WCENTER + FIELD_W ) - ( SCORE_W / 2 ) )  + 2);

    save = newwin( SAVE_H, SAVE_W, saveY, saveX );
    box( save, V_LINES, H_LINES );
    wbkgd( save, COLOR_PAIR( 3 ) );

    mvwprintw( save, 0, 6, "| SAVE |" );
    mvwprintw( save, 1, 1, "'H' to help page" );
    mvwprintw( save, 2, 1, "'S' to save game" );
    mvwprintw( save, 3, 1, "'Q' return to menu" );

    wrefresh( save );
}

/**
 * Inizializzo il tutte le schermate della window
 *
 * @param field
 */
void initField( WINDOW* title, WINDOW* field, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds )
{
    refresh();

    initSinglePlayerTitle( title );
    initSinglePlayerField( field );
    initSinglePlayerPreview( preview );
    initSinglePlayerScore( score );
    initSinglePlayerSave( save );
    initSinglePlayerCmds( cmds );

    refresh();
}

/**
 * Creo il gameover  window
 *
 * @param field
 */
void initGameOver( int score )
{
    int starty, startx;
    WINDOW* w_gameover;

    starty = ( ( LINES - GAMEOVER_H ) / 2 );
    startx = ( ( COLS  - GAMEOVER_W ) / 2 );

    clear();
    refresh();

    w_gameover = newwin( GAMEOVER_H, GAMEOVER_W, starty, startx );
    box( w_gameover, V_LINES, H_LINES );
    wbkgd( w_gameover, COLOR_PAIR( 3 ) );

    mvwprintw( w_gameover, 0,  21, "| GAME OVER |" );
    mvwprintw( w_gameover, 3,  2, "You lose bro!! try again" );
    mvwprintw( w_gameover, 5,  2, "Your score is: " );
    mvwprintw( w_gameover, 5,  17, "%d", score );
    mvwprintw( w_gameover, 7,  21, "PRESS ANY KEY" );

    wrefresh( w_gameover );
    refresh();

    getch();
}

/**
 * Creo il quit  window
 *
 * @param field
 */
void initQuit( int score )
{
    int starty, startx;
    WINDOW* w_quit;

    starty = ( (LINES - MQUIT_H) / 2 );
    startx = ( (COLS  - MQUIT_W) / 2 );

    clear();
    refresh();

    w_quit = newwin( MQUIT_H, MQUIT_W, starty, startx );
    box( w_quit, V_LINES, H_LINES );
    wbkgd( w_quit, COLOR_PAIR( 3 ) );

    mvwprintw( w_quit, 0,  20, "| QUIT |" );
    mvwprintw( w_quit, 3,  2, "Your score is: " );
    mvwprintw( w_quit, 3,  17, "%d", score );
    mvwprintw( w_quit, 7,  21, "PRESS ANY KEY" );

    wrefresh( w_quit );
    refresh();

    getch();
}

/**
 * Creo player  window
 *
 * @param field
 */
WINDOW* initPlayerWindow( WINDOW* pgWindow )
{
    int fieldY, fieldX;

    fieldY   = ( HCENTER + 1 );
    fieldX   = ( ( WCENTER - ( SCORE_W / 2 ) ) + 1 );

    pgWindow = newwin( ( FIELD_H - 2 ), ( FIELD_W - 2 ), fieldY, fieldX );
    wbkgd( pgWindow, COLOR_PAIR( 0 ) );

    wrefresh( pgWindow );
    return ( pgWindow );
}

/**
 * Creo il preview  window
 *
 * @param field
 */
WINDOW* initPreviewWindow( WINDOW* preview )
{
    int previewY, previewX;

    previewY = ( HCENTER + 1 );
    previewX = ( ( ( ( WCENTER + FIELD_W ) - ( SCORE_W / 2 ) ) + 3 ) );

    preview = newwin( ( PREVIEW_H - 2 ), ( PREVIEW_W - 2 ), previewY, previewX );
    wbkgd( preview, COLOR_PAIR( 0 ) );

    wrefresh( preview );
    return ( preview );
}

/**
 * Creo il score  window
 *
 * @param field
 */
WINDOW* initScoreWindow( WINDOW* score )
{
    int scoreY, scoreX;

    scoreY   = ( ( HCENTER + PREVIEW_H ) + 2 );
    scoreX   = ( ( ( WCENTER + FIELD_W ) - ( SCORE_W / 2 ) ) + 3);

    score = newwin( ( SCORE_H - 2 ), ( SCORE_W - 2 ), scoreY, scoreX );
    wbkgd( score, COLOR_PAIR( 0 ) );

    wrefresh( score );
    return ( score );
}

/**
 * Creo il return  window
 *
 * @param field
 */
int initReturnToMenu( int score )
{

    WINDOW* returnToMenu;
    int starty, startx;

    starty = ( ( LINES - MQUIT_H ) / 2 );
    startx = ( ( COLS  - MQUIT_W ) / 2 );

    clear();
    refresh();

    returnToMenu = newwin( MQUIT_H, MQUIT_W, starty, startx );
    box( returnToMenu, V_LINES, H_LINES );
    wbkgd( returnToMenu, COLOR_PAIR( 3 ) );

    mvwprintw( returnToMenu, 0,  20, "| RETURN TO MENU |" );
    mvwprintw( returnToMenu, 3,  2, "Sorry Bro! You have finisched all pieces! " );
    mvwprintw( returnToMenu, 4,  2, "Your score is: " );
    mvwprintw( returnToMenu, 4,  17, "%d", score );
    mvwprintw( returnToMenu, 7,  21, "PRESS ANY KEY" );

    wrefresh( returnToMenu );
    getch();

    return ( 1 );
}

/**
 * Refresho la score  window
 *
 * @param field
 */
void refreshScore( WINDOW* s_score ,int pieces, int score )
{
    werase( s_score );

    mvwprintw( s_score, 1, 1, "Disponibili: " );
    mvwprintw( s_score, 1, 14, "%d", pieces );
    mvwprintw( s_score, 3, 1, "Punteggio: " );
    mvwprintw( s_score, 3, 14, "%d", score );

    wrefresh( s_score );
}
