#pragma once
#include <vector>
#include <cstdint>

struct IntGridComponent {
    std::vector<std::vector<int>> grid;
    int width;
    int height;
};