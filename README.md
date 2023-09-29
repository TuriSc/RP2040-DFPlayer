# RP2040 DFPlayer

A small C library to control a DFPlayer mini (or a clone like MP3-TF-16P) with Raspberry Pi Pico or another RP2040-based microcontroller.

The DFPlayer is an inexpensive audio player capable of playing Mp3 and WAV files at different sampling rates.

This library supports the following operations:
- playback of a specific track
- skip to next or previous track
- set volume (range is 1-30)
- increase or decrease volume
- set the equalizer to one of six built-in presets
- set playback mode to single play, folder repeat, single repeat, or random
- pause and resume playback
- query player status, id of current track, and total number of tracks available
- send custom commands

The device supports many more commands than this library implements, most of which can be invoked calling dfplayer_write(). For example, to put the device in standby mode you can use:
```c
dfplayer_write(&dfplayer, 0x0A, 0);
```
Please refer to the [DFPlayer datasheet](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299) for more info on the available commands.

### Usage
A typical setup would have the player read Mp3 files off a microSD card (formatted as FAT-32), with the audio files at the root level and named sequentially starting with four digits. So, for example:
```
- root
    - 0001-intro.mp3
    - 0002.mp3
    - 0003.mp3
```

An example application is provided.

### Wiring
In the example, uart1 is used on GPIOs 8 and 9 on the Pico. Another pair of TX/RX pins can be chosen, just be sure to address the correct uart instance when initializing the DFPlayer.

| RPi Pico | RP2040   | DFPlayer   |
|----------|----------|------------|
| pin 11   | GP8 (TX) | pin 2 (RX) |
| pin 12   | GP9 (RX) | pin 3 (TX) |

Two distinct DFPlayers can be used simultaneously using both uart0 and uart1.

### Version history
- 2023.09.29 - v1.1.1 - Removed blocking delay while querying
- 2023.09.03 - v1.1.0 - Added query function
- 2023.08.29 - v1.0.0 - First release
