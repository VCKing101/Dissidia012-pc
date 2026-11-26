#include "battle/agent28_types.h"
/* Replay Camera Edit screen: shows the "No.%03d" camera label text effect, rebuilding it when the slot or page changes. */
struct Label {
    int p0;
    EffectModel *model;
    ModelEffect *eff;
    int last;
    int lastB;
    int arg;
    int p18;
    int arg2;
};
struct EffectList {
    static void set(ModelEffect **slot, ModelEffect *value) { *slot = value; }
    static void attach(EffectModel *model, ModelEffect *e)
    {
        if (e == 0) {
            return;
        }
        int c;
        e->flags |= 0x8000;
        if (model->tail == 0) {
            model->tail = e;
            model->head = e;
            e->next = 0;
            e->prev = 0;
            c = model->count;
        } else {
            set(&model->tail->next, e);
            ModelEffect *t = model->tail;
            e->next = 0;
            e->prev = t;
            c = model->count;
            model->tail = e;
        }
        model->count = c + 1;
    }
    static void detach(EffectModel *model, ModelEffect *effect)
    {
        if (effect != 0) {
            effect->flags &= ~0x8000;
            if (effect->prev == 0) {
                model->head = effect->next;
            } else {
                set(&effect->prev->next, effect->next);
            }
            if (effect->next == 0) {
                model->tail = effect->prev;
            } else {
                set(&effect->next->prev, effect->prev);
            }
            effect->prev = 0;
            effect->next = 0;
            model->count--;
        }
    }
};
extern "C" {
extern int func_088CA42C(int a);
extern int func_08B2C1CC(Label *self);
extern int func_0887AFBC(int a, int b, int c);
extern void func_0893476C(char *buf, char *fmt, int v);
extern void func_0887DFF4(int a, char *b, int c, char *d);
extern ModelEffect *func_0887AD78(int a, int b, int c, char **d, int e, int f);
extern void func_0887B9C4(ModelEffect *e);
extern void func_0887B970(ModelEffect *e);
extern void func_0887BA5C(ModelEffect *e);
extern char D_08B2C260[];
extern int D_089D6998;
int func_08B2BEDC(Label *self) {
    char buf[16];
    char *argv[2];
    int n = func_088CA42C(self->arg);
    if (n < -1) {
        n = -1;
    } else if (n > 998) {
        n = 998;
    }
    int page = func_08B2C1CC(self);
    if (page < 0) {
        return -1;
    }
    EffectModel *model = self->model;
    if (model != 0) {
        if (self->eff != 0 && (n != self->last || page != self->lastB)) {
            EffectList::detach(model, self->eff);
            if (self->eff != 0) {
                func_0887BA5C(self->eff);
                self->eff = 0;
            }
        }
        if (self->eff == 0) {
            func_0893476C(buf, D_08B2C260, n + 1);
            func_0887DFF4(D_089D6998, buf, 0x10, buf);
            int w = func_0887AFBC(self->arg2, page, 0) + 2;
            argv[0] = buf;
            argv[1] = (char *)w;
            int two = 2;
            ModelEffect *made = self->eff = func_0887AD78(self->arg2, 0, 0x2E, argv, two, 0);
            if (made == 0) {
                return -1;
            }
            func_0887B9C4(made);
            func_0887B970(self->eff);
            EffectList::attach(self->model, self->eff);
        }
    }
    self->last = n;
    self->lastB = page;
    return 0;
}
}
