#include "main_presentation_initialized_view.h"
extern "C" bool func_08873EB4(void) {
    MainPresentationChannel *channel = D_09B47B50;
    int i = 0;
    do {
        if (channel->field_0234 != 0 || channel->value.values[3] != 0.0f) return true;
        i++;
        channel++;
    } while (i < 3);
    return false;
}
extern "C" bool func_08873F10(void) {
    MainPresentationChannel *channel = D_09B47B50;
    int i = 0;
    do {
        if (channel->field_0234 != 0) return true;
        i++;
        channel++;
    } while (i < 3);
    return false;
}
