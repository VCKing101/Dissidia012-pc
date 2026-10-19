#include "main_presentation_initialized_view.h"
extern "C" int func_08873EB4(void) {
    int i = 0;
    MainPresentationChannel *channel = D_09B47B50;
    int result = 0;
    do {
        if (channel->field_0234 != 0 || channel->value.values[3] != 0.0f) { result = 1; break; }
        i++;
        channel++;
    } while (i < 3);
    return result;
}
extern "C" int func_08873F10(void) {
    int i = 0;
    MainPresentationChannel *channel = D_09B47B50;
    int result = 0;
    do {
        if (channel->field_0234 != 0) { result = 1; break; }
        i++;
        channel++;
    } while (i < 3);
    return result;
}
