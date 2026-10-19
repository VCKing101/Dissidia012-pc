#include "main_presentation_view.h"
MainPresentationVector *func_089A3868(MainPresentationVector *self,const MainPresentationVector *source) {
    float fourth = source->w;
    float first = source->x;
    float second = source->y;
    float third = source->z;
    self->x = first;
    self->y = second;
    self->z = third;
    self->w = fourth;
    return self;
}
