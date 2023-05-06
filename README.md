# Elden Ring Launcher v3.0
A simple utility to launch Elden Ring with or without anti-cheat.

> Note: Version 3.0 (like 2.0), has no new functionality. I rewrote this program in Rust :).

## Installation
1. Download the latest [release](https://github.com/v-maxson/EldenRingLauncher/releases/latest).
2. Navigate to your Elden Ring installation directory (right click the game on Steam > `Manage` > `Browse Local Files`)
   and navigate into the `Game` folder.
3. Rename the existing `start_protected_game.exe` to `start_protected_game_original.exe`.
4. Drag the downloaded `start_protected_game.exe` into your Elden Ring directory.
5. Start Elden Ring through Steam.

## Building
Download [Rust](https://www.rust-lang.org/tools/install) and run the `cargo build --release` command. 