#define _CRT_SECURE_NO_WARNINGS

#include "testtools.h"

#include <time.h>
#include <stdio.h>

static sf::RenderWindow* CurrentWindow = NULL;
static float FPS = 0;
static sf::Text FPStext;
static sf::Font FPSfont;

void startFPScnt (sf::RenderWindow* window) {
    CurrentWindow = window;

    FPSfont.loadFromFile("couriernew.ttf");

    FPStext.setCharacterSize(22);
    FPStext.setFillColor(sf::Color::Green);
    FPStext.setOutlineColor(sf::Color::Green);
    FPStext.setOutlineThickness(0.f);
    FPStext.setPosition(0, 0);
    FPStext.setFont(FPSfont);
}

void updateFPS () {
    static time_t prevframes[4] = {0};
    static time_t prevframe = 0;
    static time_t sumtime = 0;
    static size_t curpos = 0;

    time_t nowtime = 0;

    sumtime -= prevframes[curpos];
    nowtime = clock();
    prevframes[curpos] = nowtime - prevframe;
    sumtime += prevframes[curpos];
    curpos = (curpos + 1) % 4;
    prevframe = nowtime;

    FPS = (CLOCKS_PER_SEC * 4.f) / (sumtime * 1.f);
}

void drawFPS () {
    if (!CurrentWindow) {
        return;
    }

    char num[20];
    sprintf(num, "%.1f", FPS);

    //sf::Vector2u res = CurrentWindow->getSize();
    FPStext.setString(num);

    CurrentWindow->draw(FPStext);
}





