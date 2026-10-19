#include "main/presentation_channel.h"
void func_088740DC(MainPresentationChannel *self) {
    MainPresentationEvent *last = &self->events_0020[self->field_0220-1];
    self->field_0234 = 0;
    if (last->field_0014) last->field_0014(last->field_0018);
}
