# Elden Ring Launcher
A simple utility to launch Elden Ring with or without anti-cheat.

> Note: This program has been "boxed" with Enigma Virtual Box, which can sometimes be flagged as malicious by virus scanners.
If you are not comfortable with this, you are welcome to build the project yourself without it being boxed.

## Installation
1. Download the latest [release](https://github.com/v-maxson/EldenRingLauncher/releases/latest).
2. Navigate to your Elden Ring installation directory (right click the game on Steam > `Manage` > `Browse Local Files`)
   and navigate into the `Game` folder.
3. Rename the existing `start_protected_game.exe` to `start_protected_game_original.exe`.
4. Drag the downloaded `start_protected_game.exe` into your Elden Ring directory.
5. Start Elden Ring through Steam.

## Building
Use Visual Studio 2022 to build this project. This project currently only supports Windows, 
but should be fairly simple to port to other platforms should the need arise.
