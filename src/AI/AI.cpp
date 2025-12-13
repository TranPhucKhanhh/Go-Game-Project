#include <cstdlib>
#include <string>
#include <iostream>
#include <AI/AI.h>

#ifndef KATAGO_DIR
#define KATAGO_DIR "."
#endif

void runAI() {
    std::string cmd =
        "cd /d \"" KATAGO_DIR "\" && "
        "start \"\" katago.exe gtp -model model.bin.gz -config default_gtp.cfg";

    std::cout << "CMD: " << cmd << '\n';

    system(cmd.c_str());
}
