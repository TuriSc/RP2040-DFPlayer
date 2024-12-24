/**
 * @file example.c
 * @brief Example code for using the DFPlayer library.
 */

#include <stdio.h>
#include <pico/stdlib.h>
#include "dfplayer.h"

/**
 * @brief Pin definitions for the DFPlayer.
 * Another pair of TX/RX pins can be used, just
 * be sure to address the relative uart instance.
 */
#define GPIO_TX         8       // To RX on the player
#define GPIO_RX         9       // To TX on the player
#define DFPLAYER_UART   uart1

/**
 * @brief Create an instance of the DFPlayer.
 */
dfplayer_t dfplayer;

int main() {
    /**
     * @brief Initialize the standard input/output streams.
     */
    stdio_init_all();

    /**
     * @brief Initialize the DFPlayer instance.
     */
    dfplayer_init(&dfplayer, DFPLAYER_UART, GPIO_TX, GPIO_RX);

    /**
     * @brief Sleep for 200ms between commands
     */
    sleep_ms(200);

    // The many players on the market come with different chips.
    // Some of them require a checksum as part of the control message,
    // some don't. Enable the following line if your specific player
    // does not respond to all commands:
    // dfplayer_set_checksum_tx(false);

    /**
     * @brief Set the volume to 30.
     */
    dfplayer_set_volume(&dfplayer, 30);
    sleep_ms(200);

    /**
     * @brief Apply an equalization preset.
     */
    dfplayer_set_eq(&dfplayer, EQ_BASS);
    sleep_ms(200);

    /**
     * @brief Change the playback mode.
     */
    dfplayer_set_playback_mode(&dfplayer, MODE_FOLDER_REPEAT);
    sleep_ms(200);

    /**
     * @brief Play the first track on the microSD.
     */
    dfplayer_play(&dfplayer, 1);

    while (true) {
        /**
         * @brief Wait 30 seconds.
         */
        sleep_ms(5000); // TEST 30000

        /**
         * @brief Skip to next track.
         */
        dfplayer_next(&dfplayer);
        sleep_ms(200);

        /**
         * @brief Get the current track ID.
         */
        uint16_t current_track = dfplayer_get_track_id(&dfplayer);
        if(current_track > 0) { printf("Playing track: %d\n", current_track);}
    }
}

