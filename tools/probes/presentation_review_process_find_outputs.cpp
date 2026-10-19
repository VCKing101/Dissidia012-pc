extern "C" {
#include "main_presentation_process_view.h"
typedef struct MainPresentationClockView { unsigned char unknown[0x18]; float field_0018; } MainPresentationClockView;
extern MainPresentationClockView *D_089D69E0;
extern float D_089D5EAC;
extern void *func_089A39EC(float *,const float *,const float *,float);
}
struct PresentationReviewIntervals {
    static void find(MainPresentationChannel *self, MainPresentationEvent *&previous, MainPresentationEvent *&following) {
        int limit = self->field_0220 - 1;
        int i = 0;
        previous = 0;
        following = 0;
        for (i=0;i<limit;i++) {
            if (self->events_0020[i].field_0010 <= self->field_0224 && self->field_0224 < self->events_0020[i+1].field_0010) {
                previous = &self->events_0020[i];
                following = &self->events_0020[i+1];
                break;
            }
        }
    }
};
extern "C" void func_08873F80(MainPresentationChannel *self) {
    MainPresentationEvent *following;
    MainPresentationEvent *previous;
    PresentationReviewIntervals::find(self, previous, following);
    if (!previous) return;
    {
        float fraction = (self->field_0224 - previous->field_0010)/(following->field_0010 - previous->field_0010);
        func_089A39EC(self->interpolated_0010.values,previous->value.values,following->value.values,fraction);
        if (!(self->field_0228 < previous->field_0010)) {
            goto advance;
        }
        if (previous->field_0010 <= self->field_0224) {
            if (previous->field_0014) previous->field_0014(previous->field_0018);
        }
advance:
        self->field_0228 = self->field_0224;
        if (self->field_0238 == 1) self->field_0224 += D_089D5EAC;
        else self->field_0224 += D_089D69E0->field_0018;
        if (!(self->field_0224 < self->field_022C)) self->field_0234 = 2;
    }
}
