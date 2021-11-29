# ZombieAppocalypse
Zombie shooter game in C++

## Setup
- Clone and get the submodules, e.g., `git submodule update --init`
- Build SFML with CMake:
  - Set the output dir (for binaries) to `<project>/vendor/SFML/build`
  - Set `CMAKE_INSTALL_PREFIX` to `<project>/vendor/SFML/install`
  - Hit generate -> Open Project
  - Build  both `ALL_BUILD` and `INSTALL` for both Debug and Release
- Download premake and put it in the root folder
- Run premake, e.g., `premake5.exe vs2019`
- Build the project in either Debug or Release

## Run
Run around, kill zombies, get pickups, clear the waves.
Once a wave is cleared, you can choose an upgrade, e.g., run faster, more health, better pickups. Choose wisely.