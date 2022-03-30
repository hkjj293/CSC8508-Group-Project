cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_library(GUI STATIC
    GameMenu.h
    GameUI.h
    MainMenu.h
    PauseMenu.h
    SettingMenu.h

    GameUI.cpp
    MainMenu.cpp
    PauseMenu.cpp
)