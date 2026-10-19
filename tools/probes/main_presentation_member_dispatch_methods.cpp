#include "main_presentation_dispatch_view.h"
void MainPresentationChannel::func_088739CC() {
    typedef void (MainPresentationChannel::*Method)();
    Method handlers[3] = {&MainPresentationChannel::func_08873F78,
                          &MainPresentationChannel::func_08873F80,
                          &MainPresentationChannel::func_088740DC};
    (this->*handlers[field_0234])();
}
void MainPresentationChannel::func_08873A64() {
    typedef void (MainPresentationChannel::*Method)();
    Method handlers[3] = {&MainPresentationChannel::func_08874118,
                          &MainPresentationChannel::func_0887414C,
                          &MainPresentationChannel::func_08874180};
    if (!field_0239) (this->*handlers[field_0234])();
}
