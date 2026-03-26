#ifndef CPU_H_
#define CPU_H_

#include "commonConfig.h"
#include "singlePlayer.h"
#include "multiPlayer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

void fallingAi( player* cpu );

void refreshCpuScore( int pieces, int *pg1Score, int *cpuScore, WINDOW* score );

void opening( player* cpu, int tetraminos[] );

void fillPossibleChoices( int zeros, int* tet_type, int* tet_ory, int start,int* offset, int tetLeft[] );

int fullConsecutive( int row, int gamefield[][ MATRIX_W ] );

int isCompletable( int row, int gamefield[][ MATRIX_W ] );

int belowCotrol( int bestRow, int start, int zero, int gamefield[][ MATRIX_W ] );

int analizeRows( player* cpu );

int checkLastRow( int gamefield[][ MATRIX_W ] );

int completionSpot( int bastRow, int gamefield[][ MATRIX_W ], int* start );

void bigBrain( player* cpu, int tetraminos[] );

void colorFieldCPU( int gameField[][ MATRIX_W ], player *cpu );

void startCpuGame( player* pg1, player* cpu, tet* piece, int* tetPieces,
                   WINDOW*s_preview, WINDOW* s_score, WINDOW* w_title, WINDOW* w_field,
                   WINDOW* w_sfield, WINDOW* w_preview, WINDOW* w_score, WINDOW* w_save, WINDOW* w_cmds );

int CPU();


#endif
