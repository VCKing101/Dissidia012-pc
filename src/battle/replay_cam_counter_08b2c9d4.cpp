#include "battle/agent28_types.h"
/* Replay pause screen: shows the free camera slot count (0 to 999) as a text effect, rebuilding it when the count changes. */
struct Pause {
    int p0;
    EffectModel *model;
    int p8;
    ModelEffect *eff;
    int p10;
    int last;
    int arg;
    int p1c;
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
extern int func_088C83FC(int a);
extern void func_0893476C(char *buf, char *fmt, int v);
extern void func_0887DFF4(int a, char *b, int c, char *d);
extern ModelEffect *func_0887AD78(int a, int b, int c, char **d, int e, int f);
extern void func_0887B9C4(ModelEffect *e);
extern void func_0887B958(ModelEffect *e);
extern void func_0887BA5C(ModelEffect *e);
extern char D_08B2CCC8[];
extern int D_089D6998;
int func_08B2C9D4(Pause *self) {
    char buf[16];
    char *argv[1];
    int n = func_088C83FC(self->arg);
    EffectModel *model = self->model;
    if (n < 0) {
        n = 0;
    } else if (n > 999) {
        n = 999;
    }
    if (model != 0) {
        if (self->eff != 0 && n != self->last) {
            EffectList::detach(model, self->eff);
            if (self->eff != 0) {
                func_0887BA5C(self->eff);
                self->eff = 0;
            }
        }
        if (self->eff == 0) {
            func_0893476C(buf, D_08B2CCC8, n % 1000);
            func_0887DFF4(D_089D6998, buf, 0x10, buf);
            argv[0] = buf;
            int one = 1;
            ModelEffect *made = self->eff = func_0887AD78(self->arg2, 0, 0x2C, argv, one, 0);
            if (made == 0) {
                return -1;
            }
            func_0887B9C4(made);
            func_0887B958(self->eff);
            EffectList::attach(self->model, self->eff);
        }
    }
    self->last = n;
    return 0;
}
}
