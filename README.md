# Go Game project in university

This is a go game project made by 2 university students which is written entirely in C++ using SFML library for UI

This project only works on windows OS.

## AI instruction

Since AI is very heavy and cannot be compacted, this is an instruction on how to download and include AI in the game, step by step.

Step 1: Make an empty folder, name it "Katago".

Step 2: Click https://github.com/lightvector/KataGo/releases/download/v1.16.4/katago-v1.16.4-opencl-windows-x64.zip to download.

Step 3: Extract the downloaded file to "Katago" folder.

Step 4: Click https://media.katagotraining.org/uploaded/networks/models/kata1/kata1-b28c512nbt-s12043015936-d5616446734.bin.gz to download the model.

Step 5: Rename the downloaded file in step 4 into "model.bin.gz" and put it in "Katago" folder, DO NOT extract the file. You can visit https://katagotraining.org/networks/ to manually download any model you want, the rename step is the same, just to make sure that it MUST HAVE ".bin.gz" and not ".txt.gz".

Step 6: Put the whole "Katago" folder into the Game folder (the same folder level as "include" and "src").

Voila, this is the end of the AI instruction.

## Simple instruction

Ensure that CMake is available in your machine. You can download CMake from the official website https://cmake.org/download/

After that open the terminal in the current location pointing to the root of the project, then run these command lines

Remember to connect the device to the internet since CMake will download SFML after you run the programs

```
cmake -B build
cmake --build build
```

After all the preparations are finished, from the root of the project go to the `build/bin/Debug/` gogame and run the `gogame.exe`

More detail instructions are inside the `INTRUCTIONS.md` file

## Beginner assistance

Assistance is on by default. Use **Assistance: ON/OFF** in the game side panel to show or hide the learning overlays.

- Hover over a stone to outline its connected group and mark its empty neighboring intersections in green. The side panel shows the number of stones and distinct liberties; a liberty shared by several stones counts once.
- Red-orange outlines mark groups in **atari** (exactly one liberty). Hovering such a group also shows `ATARI` in the readout.
- Assistance follows the displayed position, including captures, undo/redo and history previews. It does not play a move or change the rules.
- Rejected suicide and superko moves open an English explanation. Close it to continue playing; these explanations remain available when assistance is off.

## Timed games and komi

The New Game screen offers **Untimed**, **1 min**, **5 min** and **10 min** of main time per player, without increments or overtime. Click **Komi: ... (edit)** to enter a value from -50 to 50 in half-point steps; komi is added to White's area score. The selected value is shared by live scoring, history review and KataGo.

Both clocks appear in the game side panel. A green outline identifies the active player, `*` marks a running timed clock and `||` marks a paused clock. A clock turns red at 30 seconds or less. Playing a move or passing changes the active clock; reaching zero ends the match with a timeout result.

This is a practice-game clock policy: settings/menus, dialogs, history review and an unfocused window pause the clocks. Undo/redo keeps time already spent; reset starts both clocks from the selected limit. Saving preserves both remaining times, komi and the outcome. Loading resumes without charging time spent outside the application. Legacy saves without timing fields load untimed with 7.5 komi. After closing the result dialog, completed games can also be saved.

AI search time counts even if the existing engine bridge temporarily blocks window updates during the search.

## Full repository on Github

The Github link to the full project with instruction is: https://github.com/TranPhucKhanhh/Go-Game-Project.git

