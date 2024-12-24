/**
 * @file dfplayer.c
 * @brief Library to control a DFPlayer mini (or clone) with Raspberry Pi Pico.
 * @author Turi Scandurra – https://turiscandurra.com/circuits
 */

#include "dfplayer.h"
#include <string.h>

/**
 * @brief The current status of the DFPlayer.
 */
uint8_t dfplayer_status;

/**
 * @brief The current volume of the DFPlayer.
 */
uint8_t dfplayer_volume;

/**
 * @brief The current track ID of the DFPlayer.
 */
uint16_t dfplayer_current_track;

/**
 * @brief The number of tracks on the SD card.
 */
uint16_t dfplayer_num_tracks;

/**
 * @brief Whether to transmit checksums.
 */
bool dfplayer_checksum_tx = true;

/**
 * @brief The initial command buffer.
 */
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

/**
 * @brief Sets whether to transmit checksums.
 *
 * @param flag True to transmit checksums, false to not transmit.
 * @note The many players on the market come with different chips.
 * Some of them require a checksum as part of the control message,
 * some don't.
 */
void dfplayer_set_checksum_tx(bool flag){
    dfplayer_checksum_tx = flag;
}

/**
 * @brief Calculates the checksum for a given buffer.
 *
 * @param buffer The buffer to calculate the checksum for.
 * @return The calculated checksum.
 */
int16_t calculate_checksum(uint8_t *buffer){
    int16_t checksum;
    checksum = 0 - (buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6]);
    return checksum;
}

/**
 * @brief Writes a command to the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to write to.
 * @param cmd The command to write.
 * @param arg The argument for the command.
 */
void dfplayer_write(dfplayer_t *dfplayer, uint8_t cmd, uint16_t arg){
    uint8_t buffer[10];
    int16_t checksum;
    memcpy(buffer, init_cmd_buf, 10);
    buffer[3] = cmd;
    buffer[5] = (uint8_t) (arg >> 8);
    buffer[6] = (uint8_t) arg;
    checksum = calculate_checksum(buffer);
    buffer[7] = (uint8_t) (checksum >> 8);
    buffer[8] = (uint8_t) checksum;
    
    if(dfplayer_checksum_tx){
        uart_write_blocking(dfplayer->uart, buffer, 10);
    } else {
        buffer[7] = 0xef; // End byte
        uart_write_blocking(dfplayer->uart, buffer, 8);
    }
}

/**
 * @brief Queries the DFPlayer for information.
 *
 * @param dfplayer The DFPlayer instance to query.
 * @param cmd The command to query.
 * @param param The parameter for the query.
 * @return True if the query was successful, false otherwise.
 */
bool dfplayer_query(dfplayer_t *dfplayer, uint8_t cmd, uint16_t param){
    uint8_t buffer[10];
    dfplayer_write(dfplayer, cmd, param);
    // TODO add non-blocking delay
    uart_read_blocking(dfplayer->uart, buffer, 10);
	if(buffer[0] != init_cmd_buf[0]) return false;
	if(buffer[2] != init_cmd_buf[2]) return false;
	if(buffer[9] != init_cmd_buf[9]) return false;

    switch(buffer[3]){
		case QUERY_STATUS:
		    dfplayer_status = buffer[6];
		break;
        case QUERY_VOLUME:
		    dfplayer_volume = buffer[6];
		break;
		case QUERY_SD_TRACK:
		    dfplayer_current_track = (buffer[5] << 8 | buffer[6]);
		break;
        case QUERY_NUM_SD_TRACKS:
		    dfplayer_num_tracks = (buffer[5] << 8 | buffer[6]);
		break;
    }

    return true;
}

/**
 * @brief Gets the current status of the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to get the status for.
 * @return The current status of the DFPlayer.
 */
uint8_t dfplayer_get_status(dfplayer_t *dfplayer){
    dfplayer_status = 0;
    dfplayer_query(dfplayer, QUERY_STATUS, 0x0000);
    return dfplayer_status;
}

/**
 * @brief Gets the current volume of the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to get the volume for.
 * @return The current volume of the DFPlayer.
 */
uint8_t dfplayer_get_volume(dfplayer_t *dfplayer){
    dfplayer_volume = 0;
    dfplayer_query(dfplayer, QUERY_VOLUME, 0x0000);
    return dfplayer_volume;
}

/**
 * @brief Gets the current track ID of the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to get the track ID for.
 * @return The current track ID of the DFPlayer.
 */
uint16_t dfplayer_get_track_id(dfplayer_t *dfplayer){
    dfplayer_current_track = 0;
    dfplayer_query(dfplayer, QUERY_SD_TRACK, 0x0000);
    return dfplayer_current_track;
}

/**
 * @brief Gets the number of tracks on the SD card.
 *
 * @param dfplayer The DFPlayer instance to get the number of tracks for.
 * @return The number of tracks on the SD card.
 */
uint16_t dfplayer_get_num_tracks(dfplayer_t *dfplayer){
    dfplayer_num_tracks = 0;
    dfplayer_query(dfplayer, QUERY_NUM_SD_TRACKS, 0x0000);
    return dfplayer_num_tracks;
}

/**
 * @brief Initializes a DFPlayer instance.
 *
 * @param dfplayer The DFPlayer instance to initialize.
 * @param uart The UART instance to use for communication.
 * @param gpio_tx The GPIO pin to use for transmission.
 * @param gpio_rx The GPIO pin to use for reception.
 */
void dfplayer_init(dfplayer_t *dfplayer, uart_inst_t *uart, uint8_t gpio_tx, uint8_t gpio_rx){
    dfplayer->uart = uart;
    dfplayer->max_volume = 30; // Allowed values are 1 to 30
    uart_init(uart, BAUDRATE);
    gpio_set_function(gpio_tx, GPIO_FUNC_UART);
    gpio_set_function(gpio_rx, GPIO_FUNC_UART);
}

/**
 * @brief Plays the next track.
 *
 * @param dfplayer The DFPlayer instance to play the next track on.
 */
void dfplayer_next(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_NEXT, 0);
}

/**
 * @brief Plays the previous track.
 *
 * @param dfplayer The DFPlayer instance to play the previous track on.
 */
void dfplayer_previous(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_PREV, 0);
}

/**
 * @brief Plays a specific track.
 *
 * @param dfplayer The DFPlayer instance to play the track on.
 * @param track The track ID to play.
 */
void dfplayer_play(dfplayer_t *dfplayer, uint16_t track){
    dfplayer_write(dfplayer, CMD_PLAY_TRACK, track);
}

/**
 * @brief Increases the volume.
 *
 * @param dfplayer The DFPlayer instance to increase the volume on.
 */
void dfplayer_increase_volume(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_VOL_INC, 0);
}

/**
 * @brief Decreases the volume.
 *
 * @param dfplayer The DFPlayer instance to decrease the volume on.
 */
void dfplayer_decrease_volume(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_VOL_DEC, 0);
}

/**
 * @brief Sets the volume.
 *
 * @param dfplayer The DFPlayer instance to set the volume on.
 * @param volume The volume to set.
 */
void dfplayer_set_volume(dfplayer_t *dfplayer, uint16_t volume){
    if (volume > dfplayer->max_volume) {volume = dfplayer->max_volume;}
    dfplayer_write(dfplayer, CMD_VOL, volume);
}

/**
 * @brief Sets the maximum allowed volume.
 *
 * @param dfplayer The DFPlayer instance to set the maximum volume on.
 * @param volume The maximum volume to set.
 */
void dfplayer_set_max_volume(dfplayer_t *dfplayer, uint16_t volume) {
    if (volume > 30) {volume = 30;}
    dfplayer->max_volume = volume;
    dfplayer_get_volume(dfplayer);
    if(volume < dfplayer_volume){ dfplayer_set_volume(dfplayer, volume); }
}

/**
 * @brief Sets the equalization preset.
 *
 * @param dfplayer The DFPlayer instance to set the equalization preset on.
 * @param eq The equalization preset to set.
 */
void dfplayer_set_eq(dfplayer_t *dfplayer, dfplayer_eq_t eq){
    dfplayer_write(dfplayer, CMD_EQ, eq);
}

/**
 * @brief Sets the playback mode.
 *
 * @param dfplayer The DFPlayer instance to set the playback mode on.
 * @param mode The playback mode to set.
 */
void dfplayer_set_playback_mode(dfplayer_t *dfplayer, dfplayer_mode_t mode){
    dfplayer_write(dfplayer, CMD_PLAYBACK_MODE, mode);
}

/**
 * @brief Resumes playback.
 *
 * @param dfplayer The DFPlayer instance to resume playback on.
 */
void dfplayer_resume(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_RESUME, 0);
}

/**
 * @brief Pauses playback.
 *
 * @param dfplayer The DFPlayer instance to pause playback on.
 */
void dfplayer_pause(dfplayer_t *dfplayer){
    dfplayer_write(dfplayer, CMD_PAUSE, 0);
}

