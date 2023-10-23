/*
 * RP2040 DFPlayer
 * C library to control a DFPlayer mini (or clone) with Raspberry Pi Pico.
 * By Turi Scandurra – https://turiscandurra.com/circuits
 * 2023.10.23 - v1.2.1
*/

#ifndef INC_DFPLAYER_H_
#define INC_DFPLAYER_H_

#include <pico/stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BAUDRATE    9600

// Available commands. The device supports many more,
// but only the basic ones are implemented here.
#define CMD_NEXT            0x01
#define CMD_PREV            0x02
#define CMD_PLAY_TRACK      0x03
#define CMD_VOL_INC         0x04
#define CMD_VOL_DEC         0x05
#define CMD_VOL             0x06
#define CMD_EQ              0x07
#define CMD_PLAYBACK_MODE   0x08
#define CMD_RESUME          0x0D
#define	CMD_PAUSE           0x0E
#define	QUERY_STATUS        0x42
#define	QUERY_VOLUME        0x43
#define	QUERY_SD_TRACK      0x4C
#define QUERY_NUM_SD_TRACKS 0x47

// Playback modes
typedef enum {
    MODE_SINGLE_PLAY     = 0x0001,
    MODE_FOLDER_REPEAT   = 0x0001,
    MODE_SINGLE_REPEAT   = 0x0002,
    MODE_RANDOM          = 0x0003
} dfplayer_mode_t;

// Equalizer presets
typedef enum {
    EQ_NORMAL       = 0,
    EQ_POP          = 1,
    EQ_ROCK         = 2,
    EQ_JAZZ         = 3,
    EQ_CLASSIC      = 4,
    EQ_BASS         = 5
} dfplayer_eq_t;

typedef struct {
    uart_inst_t *uart;
    uint8_t max_volume;
} dfplayer_t;

int16_t calculate_checksum(uint8_t *buffer);
void dfplayer_set_checksum_tx(bool flag);
void dfplayer_write(dfplayer_t *dfplayer, uint8_t cmd, uint16_t arg);
bool dfplayer_query(dfplayer_t *dfplayer, uint8_t cmd, uint16_t param);
uint8_t dfplayer_get_status(dfplayer_t *dfplayer);
uint8_t dfplayer_get_volume(dfplayer_t *dfplayer);
uint16_t dfplayer_get_track_id(dfplayer_t *dfplayer);
uint16_t dfplayer_get_num_tracks(dfplayer_t *dfplayer);
void dfplayer_init(dfplayer_t *dfplayer, uart_inst_t *uart, uint8_t gpio_tx, uint8_t gpio_rx);
void dfplayer_next(dfplayer_t *dfplayer);
void dfplayer_previous(dfplayer_t *dfplayer);
void dfplayer_play(dfplayer_t *dfplayer, uint16_t track);
void dfplayer_increase_volume(dfplayer_t *dfplayer);
void dfplayer_decrease_volume(dfplayer_t *dfplayer);
void dfplayer_set_volume(dfplayer_t *dfplayer, uint16_t volume);
void dfplayer_set_max_volume(dfplayer_t *dfplayer, uint16_t volume);
void dfplayer_set_eq(dfplayer_t *dfplayer, dfplayer_eq_t eq);
void dfplayer_set_playback_mode(dfplayer_t *dfplayer, dfplayer_mode_t mode);
void dfplayer_resume(dfplayer_t *dfplayer);
void dfplayer_pause(dfplayer_t *dfplayer);

#ifdef __cplusplus
}
#endif

#endif // INC_DFPLAYER_H_