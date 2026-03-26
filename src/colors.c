#include "colors.h"
#include <ncurses.h>

void initializeColors()
{
	start_color();

	init_pair ( 0, COLOR_BLACK, COLOR_WHITE );
	/* BACKGROUND COLORS */
	init_pair ( 1, COLOR_WHITE, COLOR_BLUE );

	/* TITLE COLORS */
	init_pair ( 2, COLOR_RED, COLOR_WHITE );

	/* MENU COLORS */
	init_pair ( 3, COLOR_WHITE, COLOR_BLUE );

	/* DESC COLORS */
	init_pair( 4, COLOR_WHITE, COLOR_BLUE );

	/* CMDS COLORS*/
	init_pair ( 5, COLOR_RED, COLOR_WHITE );

	init_pair ( 6, COLOR_WHITE, COLOR_RED );

	/* COLOR'S PIECIES */
	init_pair ( 10, COLOR_BLACK, COLOR_BLUE    ); 	/* I - BLU	   */
	init_pair ( 11, COLOR_BLACK, COLOR_CYAN    ); 	/* J - CIANO   */
	init_pair ( 12, COLOR_BLACK, COLOR_GREEN   ); 	/* L - VERDE   */
	init_pair ( 13, COLOR_BLACK, COLOR_RED     );   /* O - ROSSO   */
	init_pair ( 14, COLOR_BLACK, COLOR_YELLOW  ); 	/* S - GIALLO  */
	init_pair ( 15, COLOR_BLACK, COLOR_WHITE   ); 	/* T - BIANCO  */
	init_pair ( 16, COLOR_BLACK, COLOR_MAGENTA ); 	/* Z - MAGENTA */
	init_pair ( 19, COLOR_WHITE, COLOR_WHITE   );
}
