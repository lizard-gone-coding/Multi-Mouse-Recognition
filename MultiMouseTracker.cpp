#include "MultiMouseTracker.h"

void MultiMouseTracker::processRawInput(LPARAM lParam) {
    UINT dataSize = 0;

    // Ask Windows how much data we need
    GetRawInputData(
        (HRAWINPUT)lParam,
        RID_INPUT,
        nullptr,
        &dataSize,
        sizeof(RAWINPUTHEADER)
    );

    BYTE buffer[sizeof(RAWINPUT)];

    // Retrieve raw input data
    GetRawInputData(
        (HRAWINPUT)lParam,
        RID_INPUT,
        buffer,
        &dataSize,
        sizeof(RAWINPUTHEADER)
    );

    RAWINPUT* raw = (RAWINPUT*)buffer;

    // Ignore non-mouse input
    if (raw->header.dwType != RIM_TYPEMOUSE)
        return;

    HANDLE device = raw->header.hDevice;

    // Assign a new index if this mouse is new
    if (deviceToIndex.find(device) == deviceToIndex.end()) {
        int newIndex = (int)positions.size();
        deviceToIndex[device] = newIndex;
        positions.push_back({ 0, 0 });
    }

    int index = deviceToIndex[device];

    // Accumulate relative motion
    positions[index].x += raw->data.mouse.lLastX;
    positions[index].y += raw->data.mouse.lLastY;
}

int MultiMouseTracker::getMouseCount() const {
    return (int)positions.size();
}

POINT MultiMouseTracker::getMousePosition(int index) const {
    return positions[index];
}
