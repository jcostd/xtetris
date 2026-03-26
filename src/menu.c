/**
 * @file   menu.c
 * @author Jacopo Costantini Matteo Zambon Alvise Silvestri
 * @date   Fri Jan 28 00:14:30 2022
 *
 * @brief  Menu interattivo del gioco
 *
 *
 */

#include "menu.h"
#include "colors.h"
#include <ncurses.h>
#include <string.h>


/* define the center of the screen */
#define HCENTER ( ( LINES - MENU_TITLE_H ) / 2 )
#define WCENTER ( ( COLS  - MENU_TITLE_W ) / 2 )

//* Prototype all the functions **************************************************/

/* Prototype the init functions for painting the objects */
//@{

/// Prototipo tutte le funzioni
void initTitle( WINDOW* title );
void initSwitcher( WINDOW* menu );
void initDescription( WINDOW* desc );
void initCmds( WINDOW* cmds );
void initMenu( WINDOW* menu, WINDOW* title, WINDOW* cmds, WINDOW* desc );

/* Prototype the text based functions for the highlight and the description */
void describeMod( WINDOW* desc, int mod );
void highlightMenu( WINDOW* menu, WINDOW* desc, int mod );

//@}
//* End of prototyping *******************************************************/

//* Main function ************************************************************/

/**
 * Funzione di menu, qui il giocatore sceglie la modalità
 *
 *
 */
int menu()
{
    /* declaring windows */
    WINDOW *w_menu = NULL, *w_title = NULL, *w_cmds = NULL, *w_desc = NULL; /**< Qui si inizializzano tutte le window statiche */
    WINDOW *s_menu, *s_desc; /**< Qui si inizializzano tutte le window dinamiche */

    int menuY, descY;
    int menuX, descX;
    int modCounter;
    int userChoice;

    /* initialize modCounter & userChoice */
    modCounter = userChoice = 0; /**< Inizializzo modCounter && userChoice */

    /* defining parameter for creating menu box */
    menuY = ( HCENTER - ( MENU_TITLE_Y ) + 1 ); /**< Definisco i parametri per creare la menu box */
    menuX = ( WCENTER + 1 ); /**< Definisco i parametri per creare la menu box */

    /* defining parameters for creating description box*/
    descY = ( HCENTER - ( MENU_TITLE_Y ) + 1 ); /**< Definisco i parametri per creare la menu box */
    descX = ( WCENTER + MENU_W + 3 ); /**< Definisco i parametri per creare la menu box */

    /* background color & initialize the menu */
    bkgd( COLOR_PAIR( 6 ) );        /**< Coloro il background con la palette 6 */

    /**
     * Inizializzo il menu di gioco
     *
     * @param w_menu
     * @param w_title
     * @param w_cmds
     * @param w_desc
     *
     * @return
     */
    initMenu( w_menu, w_title, w_cmds, w_desc );

    /* creating the 2 little box that contain the text */
    s_desc = newwin( ( DESC_H - 2 ), ( DESC_W - 2 ), descY, descX ); /**< Creo la window dinamica contenente la descrizione delle modalità */
    s_menu = newwin( ( MENU_H - 2 ), ( DESC_W - 2 ), menuY, menuX ); /**< Creo la window dinamica contenente le varie delle modalità */

    wbkgd( s_desc, COLOR_PAIR( 2 ) );                                /**< Coloro la window selezionata con la seguente palette */
    wbkgd( s_menu, COLOR_PAIR( 3 ) );                                 /**< Coloro la window selezionata con la seguente palette */

    /* defining key input & echo disable */
    keypad( s_menu, TRUE );  /**< Inzializza la funzionalità di input da tastiera */
    noecho();                   /**< Questa funzione di ncurses fa si che non vengano disegnati nella schermata eventuali input da tastiera dell'utente */
    curs_set( 0 );

    do /**< Inizia la sequenza di istruzioni da eseguire */
    {
        /**
         * Evidenzia la voce selezionata
         *
         * @param s_menu
         * @param s_desc
         * @param modCounter
         *
         * @return
         */
        highlightMenu( s_menu, s_desc, modCounter );
        userChoice = wgetch( s_menu ); /**< Prendo l'input da tastiera da parte dell'utente */

        switch ( userChoice )  /**< Casistica della scelta dell'utente */
        {
            case KEY_UP:
                --modCounter;   /**< Salgo di modalità */
                highlightMenu( s_menu, s_desc, modCounter );

                if( modCounter == -1 )
                    modCounter = 4;  /**< Se scorro verso l'alto quando sono alla prima voce passo all'ultima */
                break;

            case KEY_DOWN:
                ++modCounter; /**< Scendo di modalità */
                highlightMenu( s_menu, s_desc, modCounter );

                if( modCounter == 5 )
                    modCounter = 0; /**< Se scorro verso il basso quando sono all'ultima voce passo alla prima */
                break;

            default:
                highlightMenu( s_menu, s_desc, modCounter );
                break;
        }

        /**
         * Ritorno la modalità selezionata dall'utente
         *
         *
         * @return
         */
        if( ( userChoice == 10 ) && ( modCounter != 3) )
            return modCounter;
    }
    while( userChoice != 'q' );

    return ( 4 );
}

//* End of Main function *****************************************************/


//* Init Funcs ***************************************************************/

/**
 * Inizializzo il titolo della window
 *
 * @param title
 */
void initTitle( WINDOW* title )
{
    int t_starty, t_startx;

    /* defining parameters for centering title box*/
    t_starty = ( HCENTER  - ( MENU_TITLE_Y + MENU_Y ) );
    t_startx = WCENTER;

    /* creating title window */
    title = newwin( MENU_TITLE_H, MENU_TITLE_W, t_starty, t_startx ); /* parameters ( higth, width, start_y, start_x  ) */
    box( title, BOX_VE, BOX_HO ); /* creating title box window, parameters ( *window, vertical_line, horizontal_lines ) */
    wbkgd (title, COLOR_PAIR( 2 ) );

    mvwprintw( title, XTPP_Y, XTPP_X, "XTETRIS PROJECT" );
    wrefresh( title );
}

/**
 * Inizializzo il menu della window
 *
 * @param menu
 */
void initSwitcher( WINDOW* menu )
{
    int m_starty, m_startx;

    /* defining parameters for centering menu box*/
    m_starty =  ( HCENTER - ( MENU_TITLE_Y ) );
    m_startx =  WCENTER;

    /* creating menu window and window borders */
    menu = newwin( MENU_H, MENU_W, m_starty, m_startx );
    box( menu, BOX_VE, BOX_HO );
    wbkgd( menu, COLOR_PAIR( 3 ) );

    mvwprintw( menu, 0, PCENTER_M, "| GAME MENU |" );
    wrefresh( menu );
}

/**
 * Inizializzo la descrizione della window
 *
 * @param desc
 */
void initDescription( WINDOW* desc )
{
    int d_starty, d_startx;

    /* defining parameters for creating description box*/
    d_starty =  ( HCENTER - ( MENU_TITLE_Y ) );
    d_startx =  ( WCENTER + MENU_W + 2 );

    /* creating description window and window borders */
    desc = newwin( DESC_H, DESC_W, d_starty, d_startx );
    box( desc, BOX_VE, BOX_HO );
    wbkgd( desc, COLOR_PAIR( 4 ) );

    mvwprintw( desc, 0, PCENTER_D, "| DESCRIPTION |" );
    wrefresh( desc );
}

/**
 * Inizializzo il cmds della window
 *
 * @param cmds
 */
void initCmds( WINDOW* cmds )
{
    int c_starty, c_startx;

    /* defining parameters for centering cmds box*/
    c_starty = ( ( HCENTER - (MENU_TITLE_Y + MENU_Y) ) + ( MENU_CMDS_Y + 1 ) );
    c_startx = WCENTER;

    /* creating cmds window and window borders */
    cmds = newwin( MENU_CMDS_H, MENU_CMDS_W, c_starty, c_startx );
    box( cmds, BOX_VE, BOX_HO );
    wbkgd( cmds, COLOR_PAIR( 5 ) );

    mvwprintw( cmds, 0, PCENTER_C, "| COMMANDS |" );
    mvwprintw( cmds, CMDSP_Y, CMDSP_X, "[ UP ]  [ DOWN ]  [ ENTER to select ]" );
    wrefresh( cmds );
}

/**
 * Inizializzo tutte le windows
 *
 * @param menu
 * @param title
 * @param cmds
 * @param desc
 */
void initMenu( WINDOW* menu, WINDOW* title, WINDOW* cmds, WINDOW* desc )
{
    refresh();
    initTitle( title );
    initSwitcher( menu );
    initDescription( desc );
    initCmds( cmds );
}

//* End Init Funcs **********************************************************/

//* Text based funcs *********************************************************/
//
/**
 * Casistiche di overing da parte dell'utente
 *
 * @param desc
 * @param mod
 */
void describeMod( WINDOW* desc, int mod )
{
    werase( desc );
    /* SINGLE PLAYER */
    if( mod == 0 )
    {
        mvwprintw( desc, 0, 2, "The player has 20 pieces" );
        mvwprintw( desc, 1, 2, "for each tetramino." );
        mvwprintw( desc, 2, 2, "Rules for making points:" );
        mvwprintw( desc, 3, 2, "1) One   row   1 pt" );
        mvwprintw( desc, 4, 2, "2) Two   rows  3 pt" );
        mvwprintw( desc, 5, 2, "3) Three rows  6 pt" );
        mvwprintw( desc, 6, 2, "4) Four  rows 12 pt" );
    }

    /* MULTI PLAYER*/
    if( mod == 1 )
    {
        mvwprintw( desc, 0, 2, "Player share 20 pieces" );
        mvwprintw( desc, 1, 2, "for each tetramino." );
        mvwprintw( desc, 2, 2, "Rules for making points:" );
        mvwprintw( desc, 3, 2, "1) One   row   1 pt" );
        mvwprintw( desc, 4, 2, "2) Two   rows  3 pt" );
        mvwprintw( desc, 5, 2, "3) Three rows  6 pt" );
        mvwprintw( desc, 6, 2, "4) Four  rows 12 pt" );
    }

    /* PLAYER VS CPU */
    if( mod == 2 )
    {
        mvwprintw( desc, 0, 2, "Player and CPU share 20" );
        mvwprintw( desc, 1, 2, "pieces for each tetramino." );
        mvwprintw( desc, 2, 2, "Rules for making points:" );
        mvwprintw( desc, 3, 2, "1) One   row   1 pt" );
        mvwprintw( desc, 4, 2, "2) Two   rows  3 pt" );
        mvwprintw( desc, 5, 2, "3) Three rows  6 pt" );
        mvwprintw( desc, 6, 2, "4) Four  rows 12 pt" );
    }

    /* ABOUT DEVELOPERS */
    if( mod == 3 )
    {
        mvwprintw( desc, 0, 2, "GITHUB:" );
        mvwprintw( desc, 1, 2, "Jacopo Costantini" );
        mvwprintw( desc, 2, 2, "-> @jcostd" );
        mvwprintw( desc, 3, 2, "Matteo Zambon" );
        mvwprintw( desc, 4, 2, "-> @Blast291" );
        mvwprintw( desc, 5, 2, "Alvise Silvestri" );
        mvwprintw( desc, 6, 2, "-> @mastrodeimastri" );
    }

    /* EXIT */
    if( mod == 4 )
    {
        mvwprintw( desc, 1, 2, "EXIT" );
        mvwprintw( desc, 2, 2, " " );
        mvwprintw( desc, 3, 2, "BY BY !! XD" );
    }

    wrefresh( desc );
}

/**
 * Funzione di evidenziazione delle voci selezionate
 *
 * @param menu
 * @param desc
 * @param mod
 */
void highlightMenu( WINDOW* menu, WINDOW* desc, int mod )
{
    int i;
    char mod_list[ NMOD ][ MOD_LENGHT ] =
    {
        "Single Player",
        "Multi Player",
        "Player VS CPU",
        "About Developers",
        "Exit"
    };

    for( i = 0; i < NMOD; i++ )
    {
        if( i == mod )
        {
            wattrset( menu, A_STANDOUT );
            describeMod( desc, mod );
            wrefresh( desc );
        }

        mvwprintw( menu, i + 1, 2, "%s", mod_list[ i ] );
        wattroff( menu, A_STANDOUT );
        wrefresh( menu );
    }
}

//* End of Text based funcs **************************************************/
