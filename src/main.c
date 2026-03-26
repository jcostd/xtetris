/**
 * @file   main.c
 * @author Jacopo Costantini (jcostd), Matteo Zambon, Alvise Silvestri
 * @brief  Core logic and audio playback initialization for XTETRIS
 */

#include "menu.h"
#include "commonConfig.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "singlePlayer.h"
#include "multiPlayer.h"
#include "cpu.h"

#include "colors.h"
#include <ncurses.h>

/**
 * Audio data callback for miniaudio playback
 */
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;

    if (pDecoder == NULL)
	return;

    /* Reading PCM frames will loop based on what we specified in ma_data_source_set_looping() */
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void paintExit()
{
    int starty, startx;
    WINDOW *w_byebye;

    clear();
    raw();

    starty = ((LINES - BYBY_H) / 2); // Assicurati che BYBY_H sia definito in commonConfig.h
    startx = ((COLS  - BYBY_W) / 2);

    w_byebye = newwin(BYBY_H, BYBY_W, starty, startx);
    box(w_byebye, BOX_VE, BOX_HO);
    wbkgd(w_byebye, COLOR_PAIR(2));

    mvwprintw(w_byebye, 0, 14, "+ BYE BYE, SEE YOU NEXT TIME +");
    mvwprintw(w_byebye, 2, 3, "Hope you enjoyed this simple game...");
    mvwprintw(w_byebye, 4, 3, "Have a nice day/night and keep coding.");

    wattrset(w_byebye, A_REVERSE);
    mvwprintw(w_byebye, 6, 15, "PRESS ANY KEY TO EXIT...");
    wattroff(w_byebye, A_REVERSE);
    mvwprintw(w_byebye, 8, 5, "+ By: @jcostd @Blast291 @mastrodeimastri +");

    refresh();
    wrefresh(w_byebye);

    getch();
}

int main(int argc, char** argv)
{
    int mod = 0;

    ma_result        result;
    ma_decoder       decoder;
    ma_device_config deviceConfig;
    ma_device        device;

    if (argc < 2)
    {
	printf("Usage: %s <audio_file.mp3>\n", argv[0]);
	return (-1);
    }

    result = ma_decoder_init_file(argv[1], NULL, &decoder);
    if (result != MA_SUCCESS)
    {
	printf("Failed to decode the audio file.\n");
	return -2;
    }

    /* Set background music to loop indefinitely */
    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig                   = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
    {
	printf("Failed to open playback device.\n");
	ma_decoder_uninit(&decoder);
	return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
	printf("Failed to start playback device.\n");
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);
	return -4;
    }

    /* Initialize ncurses environment */
    initscr();
    initializeColors();

    /* Main Application Loop */
    do
    {
	mod = menu();

	if (mod == 0)
	    singlePlayer();
	else if (mod == 1)
	    multiPlayer();
	else if (mod == 2)
	    CPU();

    } while (mod != 4);

    paintExit();

    /* Teardown ncurses */
    refresh();
    endwin();

    /* Teardown miniaudio */
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return 0;
}
