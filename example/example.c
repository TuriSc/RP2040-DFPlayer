/*
 * RP2040 DFPlayer example
 * RP2040 DFPlayer is a C library to control a DFPlayer mini (or clone) with Raspberry Pi Pico.
 * By Turi Scandurra – https://turiscandurra.com/circuits
*/

#include <stdio.h>
#include <pico/stdlib.h>
#include "dfplayer.h"

// Pin definitions.
// Another pair of TX/RX pins can be used, just
// be sure to address the relative uart instance
#define GPIO_TX         8       // To RX on the player
#define GPIO_RX         9       // To TX on the player
#define DFPLAYER_UART   uart1

// Create an instance of the player
dfplayer_t dfplayer;

int main() {
    stdio_init_all();

    // Initialize the player
    dfplayer_init(&dfplayer, DFPLAYER_UART, GPIO_TX, GPIO_RX);

    // Sleep 200ms between commands
    sleep_ms(200);

    // The many players on the market come with different chips.
    // Some of them require a checksum as part of the control message,
    // some don't. Enable the following line if your specific player
    // does not respond to all commands:
    // dfplayer_set_checksum_tx(false);

    // Set the volume
    dfplayer_set_volume(&dfplayer, 30);
    sleep_ms(200);

    // Apply an equalization preset
    dfplayer_set_eq(&dfplayer, EQ_BASS);
    sleep_ms(200);

    // Change the playback mode
    dfplayer_set_playback_mode(&dfplayer, MODE_FOLDER_REPEAT);
    sleep_ms(200);

    // Play the first track on the microSD
    dfplayer_play(&dfplayer, 1);

    while (true) {
        // Wait 30 seconds
        sleep_ms(5000); // TEST 30000

        // Skip to next track
        dfplayer_next(&dfplayer);
        sleep_ms(200);

        // Get the current track id
        uint16_t current_track = dfplayer_get_track_id(&dfplayer);
        if(current_track > 0) { printf("Playing track: %d\n", current_track);}
    }
}
