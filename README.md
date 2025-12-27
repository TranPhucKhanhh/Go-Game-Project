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

## Full repository on Github

The Github link to the full project with instruction is: https://github.com/TranPhucKhanhh/Go-Game-Project.git

