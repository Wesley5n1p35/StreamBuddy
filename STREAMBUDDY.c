#include <flipper/flipper.h>
#include <furi-hal.h>
#include <furi-hal-gpio.h>
#include <applications.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/canvas.h>
#include <gui/view.h>
#include <notification/notification-messages.h>
#include <ble.h>
#include <services/hids/hids.h>

static uint8_t current_set = 1;

static void update_led_color() {
    if (current_set == 1) {
        furi_hal_led_set_color(FURI_HAL_LED_GREEN, 255);
    } else {
        furi_hal_led_set_color(FURI_HAL_LED_YELLOW, 255);
    }
}

static void send_keystroke(uint8_t keycode) {
    uint8_t report[3] = {0xE2, keycode, 0x00};
    hids_input_report_send(report, sizeof(report));
}

static void button_callback(ButtonEvent event, void* context) {
    if(event == BUTTON_PRESS) {
        switch(furi_hal_gpio_button_get_id()) {
            case FURI_HAL_GPIO_BUTTON_LEFT:
                send_keystroke(current_set == 1 ? 0x1E : 0x3A);
                break;
            case FURI_HAL_GPIO_BUTTON_UP:
                send_keystroke(current_set == 1 ? 0x1F : 0x3B);
                break;
            case FURI_HAL_GPIO_BUTTON_RIGHT:
                send_keystroke(current_set == 1 ? 0x20 : 0x3C);
                break;
            case FURI_HAL_GPIO_BUTTON_DOWN:
                send_keystroke(current_set == 1 ? 0x21 : 0x3D);
                break;
            case FURI_HAL_GPIO_BUTTON_OK:
                current_set = (current_set == 1) ? 2 : 1;
                update_led_color();
                break;
            default:
                break;
        }
    }
}

static void render_callback(Canvas* canvas, void* context) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(canvas, 0, 0, "Bluetooth Remote");
    canvas_draw_str(canvas, 0, 10, "Set 1:");
    canvas_draw_str(canvas, 0, 20, "Alt+1, Alt+2, Alt+3, Alt+4");
    canvas_draw_str(canvas, 0, 30, "Set 2:");
    canvas_draw_str(canvas, 0, 40, "Alt+5, Alt+6, Alt+7, Alt+8");
}

int32_t bluetooth_remote_app(void* p) {
    ViewDispatcher* view_dispatcher = view_dispatcher_alloc();
    View* view = view_alloc();

    view_set_context(view, NULL);
    view_set_render_callback(view, render_callback);
    view_dispatcher_add_view(view_dispatcher, 0, view);
    view_dispatcher_switch_to_view(view_dispatcher, 0);

    furi_hal_gpio_button_set_callback(button_callback, NULL);

    ble_init();
    ble_set_name("Flipper BT Remote");
    hids_init();

    update_led_color();

    while(1) {
        view_dispatcher_yield(view_dispatcher);
    }

    ble_deinit();
    hids_deinit();
    view_dispatcher_free(view_dispatcher);

    return 0;
}
