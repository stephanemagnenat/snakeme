add_executable(snakeme CAliveSnake.cpp CSnakeArray.cpp CSnakeEngine.cpp CSnakePowerUp.cpp SnakeUI.cpp CDeadSnake.cpp CSnake.cpp CSnakeLevelEditor.cpp SnakeMe.cpp StringTable.cpp)
target_link_libraries(snakeme SGU ${SDL_MIXER_LIBRARY} ${SDL_LIBRARY})
