#include "battle/agent28_types.h"

/* Reloads the three models of a triple-model widget and refreshes it. */
extern void func_088DA060(void *model);
extern void *func_088DAA40(void *source, int name, int flags);
extern int func_089F39E4(TripleModel *widget);

int func_089F3B48(TripleModel *widget)
{
    if (!widget->open) {
        return -1;
    }
    if (widget->cap != 0) {
        func_088DA060(widget->cap);
        widget->cap = 0;
    }
    if (widget->fill != 0) {
        func_088DA060(widget->fill);
        widget->fill = 0;
    }
    if (widget->base != 0) {
        func_088DA060(widget->base);
        widget->base = 0;
    }
    widget->base = func_088DAA40(widget->source, 8, 0);
    if (widget->base == 0) {
        return -2;
    }
    widget->fill = func_088DAA40(widget->source, 4, 0);
    if (widget->fill == 0) {
        return -3;
    }
    widget->cap = func_088DAA40(widget->source, 7, 0);
    if (widget->cap == 0) {
        return -4;
    }
    func_089F39E4(widget);
    return 0;
}
