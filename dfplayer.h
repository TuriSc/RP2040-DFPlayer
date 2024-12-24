/**
 * @file dfplayer.h
 * @brief Library to control a DFPlayer mini (or clone) with Raspberry Pi Pico.
 * @author Turi Scandurra – https://turiscandurra.com/circuits
 */

#ifndef INC_DFPLAYER_H_
#define INC_DFPLAYER_H_

#include <pico/stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dfplayer DFPlayer Library
 * @{
 */

/**
 * @def BAUDRATE
 * @brief The baud rate for the DFPlayer serial communication.
 */
#define BAUDRATE    9600

/**
 * @defgroup commands DFPlayer Commands
 * @{
 */

/**
 * @def CMD_NEXT
 * @brief Command to play the next track.
 */
#define CMD_NEXT            0x01

/**
 * @def CMD_PREV
 * @brief Command to play the previous track.
 */
#define CMD_PREV            0x02

/**
 * @def CMD_PLAY_TRACK
 * @brief Command to play a specific track.
 */
#define CMD_PLAY_TRACK      0x03

/**
 * @def CMD_VOL_INC
 * @brief Command to increase the volume.
 */
#define CMD_VOL_INC         0x04

/**
 * @def CMD_VOL_DEC
 * @brief Command to decrease the volume.
 */
#define CMD_VOL_DEC         0x05

/**
 * @def CMD_VOL
 * @brief Command to set the volume.
 */
#define CMD_VOL             0x06

/**
 * @def CMD_EQ
 * @brief Command to set the equalization preset.
 */
#define CMD_EQ              0x07

/**
 * @def CMD_PLAYBACK_MODE
 * @brief Command to set the playback mode.
 */
#define CMD_PLAYBACK_MODE   0x08

/**
 * @def CMD_RESUME
 * @brief Command to resume playback.
 */
#define CMD_RESUME          0x0D

/**
 * @def CMD_PAUSE
 * @brief Command to pause playback.
 */
#define CMD_PAUSE           0x0E

/**
 * @def QUERY_STATUS
 * @brief Command to query the player status.
 */
#define QUERY_STATUS        0x42

/**
 * @def QUERY_VOLUME
 * @brief Command to query the current volume.
 */
#define QUERY_VOLUME        0x43

/**
 * @def QUERY_SD_TRACK
 * @brief Command to query the current track ID.
 */
#define QUERY_SD_TRACK      0x4C

/**
 * @def QUERY_NUM_SD_TRACKS
 * @brief Command to query the number of tracks on the SD card.
 */
#define QUERY_NUM_SD_TRACKS 0x47

/**
 * @}
 */

/**
 * @defgroup playback_modes Playback Modes
 * @{
 */

/**
 * @enum dfplayer_mode_t
 * @brief Enumerates the playback modes.
 */
typedef enum {
    /**
     * @brief Single play mode.
     */
    MODE_SINGLE_PLAY     = 0x0001,
    /**
     * @brief Folder repeat mode.
     */
    MODE_FOLDER_REPEAT   = 0x0001,
    /**
     * @brief Single repeat mode.
     */
    MODE_SINGLE_REPEAT   = 0x0002,
    /**
     * @brief Random mode.
     */
    MODE_RANDOM          = 0x0003
} dfplayer_mode_t;

/**
 * @}
 */

/**
 * @defgroup equalization_presets Equalization Presets
 * @{
 */

/**
 * @enum dfplayer_eq_t
 * @brief Enumerates the equalization presets.
 */
typedef enum {
    /**
     * @brief Normal equalization preset.
     */
    EQ_NORMAL       = 0,
    /**
     * @brief Pop equalization preset.
     */
    EQ_POP          = 1,
    /**
     * @brief Rock equalization preset.
     */
    EQ_ROCK         = 2,
    /**
     * @brief Jazz equalization preset.
     */
    EQ_JAZZ         = 3,
    /**
     * @brief Classic equalization preset.
     */
    EQ_CLASSIC      = 4,
    /**
     * @brief Bass equalization preset.
     */
    EQ_BASS         = 5
} dfplayer_eq_t;

/**
 * @}
 */

/**
 * @struct dfplayer_t
 * @brief Structure representing a DFPlayer instance.
 */
typedef struct {
    /**
     * @brief UART instance used for communication with the DFPlayer.
     */
    uart_inst_t *uart;
    /**
     * @brief Maximum allowed volume.
     */
    uint8_t max_volume;
} dfplayer_t;

/**
 * @defgroup functions Functions
 * @{
 */

/**
 * @brief Calculates the checksum for a given buffer.
 *
 * @param buffer The buffer to calculate the checksum for.
 * @return The calculated checksum.
 */
int16_t calculate_checksum(uint8_t *buffer);

/**
 * @brief Enables or disables the transmission of checksums.
 *
 * @param flag True to enable checksum transmission, false to disable.
 */
void dfplayer_set_checksum_tx(bool flag);

/**
 * @brief Writes a command to the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to write to.
 * @param cmd The command to write.
 * @param arg The argument for the command.
 */
void dfplayer_write(dfplayer_t *dfplayer, uint8_t cmd, uint16_t arg);

/**
 * @brief Queries the DFPlayer for information.
 *
 * @param dfplayer The DFPlayer instance to query.
 * @param cmd The command to query.
 * @param param The parameter for the query.
 * @return True if the query was successful, false otherwise.
 */
bool dfplayer_query(dfplayer_t *dfplayer, uint8_t cmd, uint16_t param);

/**
 * @brief Gets the current status of the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to get the status for.
 * @return The current status of the DFPlayer.
 */
uint8_t dfplayer_get_status(dfplayer_t *dfplayer);

/**
 * @brief Gets the current volume of the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to get the volume for.
 * @return The current volume of the DFPlayer.
 */
uint8_t dfplayer_get_volume(dfplayer_t *dfplayer);

/**
 * @brief Gets the current track ID of the DFPlayer.
 *
 * @param dfplayer The DFPlayer instance to get the track ID for.
 * @return The current track ID of the DFPlayer.
 */
uint16_t dfplayer_get_track_id(dfplayer_t *dfplayer);

/**
 * @brief Gets the number of tracks on the SD card.
 *
 * @param dfplayer The DFPlayer instance to get the number of tracks for.
 * @return The number of tracks on the SD card.
 */
uint16_t dfplayer_get_num_tracks(dfplayer_t *dfplayer);

/**
 * @brief Initializes a DFPlayer instance.
 *
 * @param dfplayer The DFPlayer instance to initialize.
 * @param uart The UART instance to use for communication.
 * @param gpio_tx The GPIO pin to use for transmission.
 * @param gpio_rx The GPIO pin to use for reception.
 */
void dfplayer_init(dfplayer_t *dfplayer, uart_inst_t *uart, uint8_t gpio_tx, uint8_t gpio_rx);

/**
 * @brief Plays the next track.
 *
 * @param dfplayer The DFPlayer instance to play the next track on.
 */
void dfplayer_next(dfplayer_t *dfplayer);

/**
 * @brief Plays the previous track.
 *
 * @param dfplayer The DFPlayer instance to play the previous track on.
 */
void dfplayer_previous(dfplayer_t *dfplayer);

/**
 * @brief Plays a specific track.
 *
 * @param dfplayer The DFPlayer instance to play the track on.
 * @param track The track ID to play.
 */
void dfplayer_play(dfplayer_t *dfplayer, uint16_t track);

/**
 * @brief Increases the volume.
 *
 * @param dfplayer The DFPlayer instance to increase the volume on.
 */
void dfplayer_increase_volume(dfplayer_t *dfplayer);

/**
 * @brief Decreases the volume.
 *
 * @param dfplayer The DFPlayer instance to decrease the volume on.
 */
void dfplayer_decrease_volume(dfplayer_t *dfplayer);

/**
 * @brief Sets the volume.
 *
 * @param dfplayer The DFPlayer instance to set the volume on.
 * @param volume The volume to set.
 */
void dfplayer_set_volume(dfplayer_t *dfplayer, uint16_t volume);

/**
 * @brief Sets the maximum allowed volume.
 *
 * @param dfplayer The DFPlayer instance to set the maximum volume on.
 * @param volume The maximum volume to set.
 */
void dfplayer_set_max_volume(dfplayer_t *dfplayer, uint16_t volume);

/**
 * @param eq The equalization preset to set.
 */
void dfplayer_set_eq(dfplayer_t *dfplayer, dfplayer_eq_t eq);

/**
 * @brief Sets the playback mode.
 *
 * @param dfplayer The DFPlayer instance to set the playback mode on.
 * @param mode The playback mode to set.
 */
void dfplayer_set_playback_mode(dfplayer_t *dfplayer, dfplayer_mode_t mode);

/**
 * @brief Resumes playback.
 *
 * @param dfplayer The DFPlayer instance to resume playback on.
 */
void dfplayer_resume(dfplayer_t *dfplayer);

/**
 * @brief Pauses playback.
 *
 * @param dfplayer The DFPlayer instance to pause playback on.
 */
void dfplayer_pause(dfplayer_t *dfplayer);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // INC_DFPLAYER_H_

