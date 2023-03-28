#include <flipper/flipper.h>

// Set these values to match your Bluetooth module and PC settings
#define BT_MODULE_ADDRESS "00:00:00:00:00:00"
#define BT_MODULE_PIN "1234"

// Replace these functions with the ones to send Alt + 1, Alt + 2, etc.
void send_left() {
    // ...
}

void send_up() {
    // ...
}

void send_right() {
    // ...
}

void send_down() {
    // ...
}

int main() {
    // Initialize Flipper Zero
    lf_init();

    // Connect to the Bluetooth module
    lf_bt_connect(BT_MODULE_ADDRESS, BT_MODULE_PIN);

    while (1) {
        // Read button presses from Flipper Zero and send corresponding keystrokes
        if (lf_button_pressed(LEFT_BUTTON)) {
            send_left();
        } else if (lf_button_pressed(UP_BUTTON)) {
            send_up();
        } else if (lf_button_pressed(RIGHT_BUTTON)) {
            send_right();
        } else if (lf_button_pressed(DOWN_BUTTON)) {
            send_down();
        }

        // Add a small delay to avoid flooding the PC with keystrokes
        lf_delay_ms(100);
    }

    return 0;
}