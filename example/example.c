/*
 * RP2040 DFPlayer
 * C library to control a DFPlayer mini (or clone) with Raspberry Pi Pico.
 * By Turi Scandurra – https://turiscandurra.com/circuits
 * v1.0.0 - 2023.08.29
*/

#include <stdio.h>
#include <pico/stdlib.h>
#include "dfplayer.h"

// Pin definitions.
// Another pair of TX/RX pins can be used, just
// be sure to address the relative uart instance
#define GPIO_TX         8
#define GPIO_RX         9
#define DFPLAYER_UART   uart1

// Create an instance of the player
dfplayer_t dfplayer;

int main() {
    stdio_init_all();

    // Initialize the player
    dfplayer_init(&dfplayer, DFPLAYER_UART, GPIO_TX, GPIO_RX);

    // Set the volume
    dfplayer_set_volume(&dfplayer, 30);

    // Apply an equalization preset
    dfplayer_set_eq(&dfplayer, EQ_BASS);

    // Change the playback mode
    dfplayer_set_playback_mode(&dfplayer, MODE_FOLDER_REPEAT);

    // Play the first track on the microSD
    dfplayer_play(&dfplayer, 1);

    while (true) {
        // Wait 30 seconds
        sleep_ms(30000);

        // Skip to next track
        dfplayer_next(&dfplayer);
    }
}