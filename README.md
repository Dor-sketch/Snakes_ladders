# Snakes and Ladders Game

This project is an implementation of the classic Snakes and Ladders game with a twist - it incorporates musical notes that are played as the players move across the board. The game uses GTK for the GUI and optionally supports MIDI files to generate the notes.

<p align="center">
    <img src="images/image-4.png" width="200" /> <img src="images/image-5.png" width="200" />
</p>
<p align="center">
    <img src="images/image-6.png" width="200" /> <img src="images/image-7.png" width="200" />
</p>

## Table of Contents
- [Table of Contents](#table-of-contents)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Step 1: Install Dependencies](#step-1-install-dependencies)
  - [Step 2: Build the Project](#step-2-build-the-project)
    - [Using Makefile](#using-makefile)
  - [Playing the Game](#playing-the-game)
- [MIDI Support](#midi-support)
- [Example](#example)
- [License](#license)
- [Contributing](#contributing)

## Features

- Classic Snakes and Ladders gameplay
- Musical notes played based on player positions
- Graphical user interface using GTK
- Optional MIDI file support for generating notes

## Prerequisites

- C++ compiler (e.g., g++)
- GTK development libraries
- CMake (optional, for building)
- MIDI library (optional, if using MIDI files)
- OpenCV (optional, for image processing)

## Installation

### Step 1: Install Dependencies

For Ubuntu:

```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev cmake
```

For macOS:

```bash
brew install gtk+3 cmake
```

### Step 2: Build the Project

The project can be built using the provided `Makefile` or `CMakeLists.txt` file.

#### Using Makefile

```bash
make <flags>
```

where `<flags>` can be:

- `USE_MIDIFILE=1`: Enable MIDI file support
- `USE_OPENCV=1`: Enable OpenCV support - not implemented yet

### Playing the Game

1. **Rolling the Dice**: Click the "Roll" button to roll the dice and move your player.
2. **Winning the Game**: The first player to reach the last cell wins the game.
3. **Musical Notes**: Notes are played based on the player's position on the board.

## MIDI Support

This project optionally supports using MIDI files to generate the musical notes. To enable this feature:

1. **Install MIDI Library**:
    - Follow the instructions in the MIDI library's documentation to install it.

2. **Define the `USE_MIDIFILE` Macro**:
    - Add `#define USE_MIDIFILE` at the top of `game.cpp` or pass it as a compiler flag (`-DUSE_MIDIFILE`).

3. **Link MIDI Library**:
    - Ensure that the MIDI library is linked during the build process. Modify the build commands or `CMakeLists.txt` accordingly.

4. **Set MIDI File Path**:
    - Modify the `generateNotes` function in `game.cpp` to use the desired MIDI file path.

## Example

Here is an example of how to modify `game.cpp` to use a MIDI file:

```cpp
#define USE_MIDIFILE
#include "midifile/include/MidiFile.h"

// Modify generateNotes function
std::vector<std::vector<int>> Game::generateNotes(const std::string &midi_file_path) {
    std::cout << "MIDI file path: " << midi_file_path << std::endl;
    if (midi_file_path.empty()) {
        std::cerr << "MIDI file path is empty" << std::endl;
        return generateNotes(10);
    }

    // Open the MIDI file
    smf::MidiFile midi_file;
    if (!midi_file.read(midi_file_path)) {
        std::cerr << "Failed to open MIDI file" << std::endl;
        return generateNotes(10);
    }

    // Create a 2D vector to hold the notes
    std::vector<std::vector<int>> notes;

    // Read the MIDI messages
    for (int track = 0; track < midi_file.getTrackCount(); ++track) {
        for (int event = 0; event < midi_file[track].getEventCount(); ++event) {
            if (midi_file[track][event].isNoteOn()) {
                int note = midi_file[track][event][1];
                notes.push_back({note});
            }
        }
    }

    return notes;
}
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Feel free to create an issue or submit a pull request if you have any improvements or bug fixes.
