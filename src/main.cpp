#include "gui/gui.h"

int main() {
    if (!init_gui()) {
        return -1;  // Exit if GUI initialization fails
    }

    // Main loop
    while (render_gui()) {
        // Render GUI continuously
    }

    cleanup_gui();  // Clean up resources
    return 0;
}
