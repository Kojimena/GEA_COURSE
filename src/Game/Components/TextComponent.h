#pragma once
#include <string>
#include <raylib.h>

struct TextComponent {
    std::string text;
    int fontSize;
    Color color;

    TextComponent()
            : text(""), fontSize(20), color(WHITE) {}

    TextComponent(const std::string& txt, int size, Color col)
            : text(txt), fontSize(size), color(col) {}
};