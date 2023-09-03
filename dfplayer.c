/*
 * RP2040 DFPlayer
 * C library to control a DFPlayer mini (or clone) with Raspberry Pi Pico.
 * By Turi Scandurra – https://turiscandurra.com/circuits
 * v1.0.1 - 2023.09.03
*/

#include "dfplayer.h"
#include <string.h>

uint8_t dfplayer_status;
uint16_t dfplayer_current_track;

uint8_t init_cmd_buf[10] = {
    0x7e, // start byte
    0xff, // version
    0x06, // data length
    0x00, // command
    0x00, // feedback
    0x00, // parameter, high byte
    0x00, // parameter, low byte
    0x00, // checksum, MSB
    0x00, // checksum, LSB
    0xef  // end byte
    };

int16_t calculate_checksum(uint8_t *buffer){
    int16_t checksum;
    checksum = 0 - (buffer[1] + buffer[2] + buffer[3] + buffer[5] + buffer[6]);
    return checksum;
}

void dfplayer_write(dfplayer_t *dfplayer, uint8_t cmd, uint16_t param){
    uint8_t buffer[10];
    int16_t checksum;
    memcpy(buffer, init_cmd_buf, 10);
    buffer[3] = cmd;
    buffer[5] = (uint8_t) (param >> 8);
    buffer[6] = (uint8_t) param;
    checksum = calculate_checksum(buffer);
    buffer[7] = (uint8_t) (checksum >> 8);
    buffer[8] = (uint8_t) checksum;
    
    uart_write_blocking(dfplayer->uart, buffer, 10);
}

bool dfplayer_query(dfplayer_t *dfplayer, uint8_t cmd, uint16_t param){
    uint8_t buffer[10];
    dfplayer_write(dfplayer, cmd, param);
    sleep_ms(200); // TODO make non-blocking
    uart_read_blocking(dfplayer->uart, buffer, 10);
	if(buffer[0] != init_cmd_buf[0]) return false;
	if(buffer[2] != init_cmd_buf[2]) return false;
	if(buffer[9] != init_cmd_buf[9]) return false;

    switch(buffer[3]){
		case QUERY_STATUS:
		    dfplayer_status = buffer[6];
		break;
		case QUERY_SD_TRACK:
		    dfplayer_current_track = (buffer[5] << 8 | buffer[6]);
		break;
    }

    return true;
}

uint8_t dfplayer_get_status(dfplayer_t *dfplayer){
    dfplayer_status = 0;
    dfplayer_query(dfplayer, QUERY_STATUS, 0x0000);
    return dfplayer_status;
}

uint16_t dfplayer_get_track(dfplayer_t *dfplayer){
    dfplayer_current_track = 0;
    dfplayer_query(dfplayer, QUERY_SD_TRACK, 0x0000);
    return dfplayer_current_track;
}

void dfplayer_init(dfplayer_t *dfplayer, uart_inst_t *uart, uint8_t gpio_tx, uint8_t gpio_rx){
    dfplayer->uart = uart;
    uart_init(uart, BAUDRATE);
    gpio_set_function(gpio_tx, GPIO_FUNC_UART);
    gpio_set_function(gpio_rx, GPIO_FUNC_UART);
}

void dfplayer_next(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_NEXT, 0);
}

void dfplayer_previous(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_PREV, 0);
}

void dfplayer_play(dfplayer_t *dfplayer, uint16_t track){
    dfplayer_write(dfplayer, CMD_PLAY_TRACK, track);
}

void dfplayer_increase_volume(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_VOL_INC, 0);
}

void dfplayer_decrease_volume(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_VOL_DEC, 0);
}

void dfplayer_set_volume(dfplayer_t *dfplayer, uint16_t volume) {
    if (volume > 30) {volume = 30;}
    dfplayer_write(dfplayer, CMD_VOL_DEC, volume);
}

void dfplayer_set_eq(dfplayer_t *dfplayer, dfplayer_eq_t eq){
    dfplayer_write(dfplayer, CMD_EQ, eq);
}

void dfplayer_set_playback_mode(dfplayer_t *dfplayer, dfplayer_mode_t mode){
    dfplayer_write(dfplayer, CMD_PLAYBACK_MODE, mode);
}

void dfplayer_resume(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_RESUME, 0);
}

void dfplayer_pause(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_PAUSE, 0);
}