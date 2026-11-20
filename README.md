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

## Interactive endgame scoring

Two consecutive passes now open a scoring view and pause the clocks. Click a stone to mark its entire connected group as dead; click it again to restore it. Red crosses mark dead stones, black/white squares show territory ownership, and gray dots show neutral points. The side panel displays remaining stones, territory, White's komi and the resulting totals.

Scoring uses Chinese area: remaining stones plus owned empty intersections, with komi added to White. Removed dead stones make empty intersections; prisoners are not added as separate points. During play and history review, the `Area:` values are raw area counts with komi, not an AI prediction.

Choose **Confirm Result** to finish the match, or **Resume Play** to clear the dead-group marks and continue from the unchanged played position. Passes remain in history; two new consecutive passes are needed to open scoring again. AI turns stop during scoring and resume when appropriate after Resume Play.

**Save game** is available while scoring. Saves preserve pending/confirmed scoring, whole-group markings and clocks. Confirmed territory remains visible at the latest position; older history positions show their own boards. Earlier completed saves remain completed when loaded.

## Game review

Use **First**, **Previous**, **Next** and **Latest** above the move list. First shows the empty starting board. Previous/Next step through individual moves, including passes; clicking a move selects it and highlights its row. The yellow header shows the selected move and the total number of moves. The board, last-move marker, assistance overlays and area counts follow the selected position.

**Left/Right** step backward/forward, **Home** shows the start, and **End** or **Escape** returns to the latest live position. Reaching the final move with Next stays in review; choose Latest to leave review. Clocks pause during review, and board placement, undo/redo, passing, resignation and reset cannot change the match while reviewing. Review controls wait until an AI turn finishes and do not process shortcuts inside dialogs.

Toggle **Numbers: ON/OFF** to label surviving stones with their original move numbers. Captures remove labels, passes consume a number without placing a label, and a later stone at the same point gets its own number. The selected move is colored when it is a stone; a pass is identified in the header and move list. Labels resize with the board, and the four navigation buttons share one fitted font size.

**Save game** saves the full match even while an earlier position is displayed. Completed games retain their result, komi, clock settings and scoring choices when reopened from Load Game. Close the result dialog to review them; reopening a completed AI game does not require starting KataGo.

Repository-specific contributor instructions are stored in `.idea/AGENTS.md`. Only that file is tracked from `.idea/`; private reports and scratch files stay ignored.

## Full repository on Github

The Github link to the full project with instruction is: https://github.com/TranPhucKhanhh/Go-Game-Project.git

