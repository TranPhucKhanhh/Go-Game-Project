cmake_minimum_required(VERSION 3.28)
project(GoGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(GoGame 
                        src/main.cpp
                        src/component/Button.cpp
                        src/component/TextBox.cpp
                        src/component/Slider.cpp
                        src/component/BoardUI.cpp
                        src/component/Scroll.cpp
                        src/component/Notification.cpp

                        src/core/AssetManager.cpp

                        src/UI/InGame.cpp
                        src/UI/Menu.cpp
                        src/UI/UI.cpp
                        src/UI/Loading.cpp
                        src/UI/GameOption.cpp
                        src/UI/GameOptionPanel/GameCustomizeOption.cpp
                        src/UI/GameOptionPanel/GameSettingOption.cpp
                        src/UI/GameOptionPanel/LoadGameOption.cpp
                        src/UI/GameOptionPanel/NewGameOption.cpp

                        src/AI/AI.cpp

                        src/game/Board.cpp
                        src/game/Game.cpp
                        src/game/History.cpp
   )

target_compile_features(GoGame PRIVATE cxx_std_17)
target_link_libraries(GoGame PRIVATE SFML::Graphics SFML::Audio)
target_include_directories(GoGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(GoGame PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")
target_compile_definitions(GoGame PRIVATE SAVEGAME_DIR="${CMAKE_SOURCE_DIR}/saved")