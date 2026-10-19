#include "main_presentation_dispatch_view.h"
extern "C" void func_088739CC(MainPresentationChannel *self) {
    typedef void (MainPresentationChannel::*Method)();
    const Method handlers[3] = {&MainPresentationChannel::func_08873F78,
                          &MainPresentationChannel::func_08873F80,
                          &MainPresentationChannel::func_088740DC};
    (self->*handlers[self->field_0234])();
}
extern "C" void func_08873A64(MainPresentationChannel *self) {
    typedef void (MainPresentationChannel::*Method)();
    const Method handlers[3] = {&MainPresentationChannel::func_08874118,
                          &MainPresentationChannel::func_0887414C,
                          &MainPresentationChannel::func_08874180};
    if (!self->field_0239) (self->*handlers[self->field_0234])();
}
