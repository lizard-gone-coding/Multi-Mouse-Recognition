#pragma once

#include <windows.h>
#include <unordered_map>
#include <vector>

// This class owns all multi-mouse state and logic
class MultiMouseTracker {
public:
    // Called whenever WM_INPUT occurs
    void processRawInput(LPARAM lParam);

    // Query functions (these replace std::cout later)
    int getMouseCount() const;
    POINT getMousePosition(int index) const;

private:
    // Maps Windows device handle ? stable index
    std::unordered_map<HANDLE, int> deviceToIndex;

    // Stores accumulated positions by index
    std::vector<POINT> positions;
};
