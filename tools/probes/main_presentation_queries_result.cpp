#include "main_presentation_initialized_view.h"
extern "C" int func_08873EB4(void) {
    int i = 0;
    MainPresentationChannel *channel = D_09B47B50;
    do {
        if (channel->field_0234 != 0 || channel->value.values[3] != 0.0f) goto busy;
        i++;
        channel++;
    } while (i < 3);
    return 0;
busy:
    return 1;
}
extern "C" int func_08873F10(void) {
    int i = 0;
    MainPresentationChannel *channel = D_09B47B50;
    do {
        if (channel->field_0234 != 0) goto busy;
        i++;
        channel++;
    } while (i < 3);
    return 0;
busy:
    return 1;
}
