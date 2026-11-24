/* Updates the mode's view selection; when its group changes, or a unit
   model's visibility flag changes, re-applies that model's display state. */
struct ViewModel {
    char data_00[0x34];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual unsigned char IsShown();
};

struct ModeUnit {
    char pad_000[0x5A0];
    ViewModel *model;
};

struct ModeCtl {
    char pad_000[0x38];
    ModeUnit *units[2];
    char pad_040[0x7B8 - 0x40];
    int prev_view;
    int view;
    unsigned char prev_shown[2];
    unsigned char shown[2];
};

struct ModeState {
    char pad_00[0x4C];
    int kind;
};

extern "C" {

extern ModeState *D_089D6A10;

int func_08889B08(ModeCtl *self);
int func_08889468(ModeCtl *self, int view);
unsigned long long func_08871BC4(ViewModel *model);
void func_08871BD0(ViewModel *model, unsigned long long state);

void func_088892E8(ModeCtl *self)
{
    if (D_089D6A10->kind == 9) {
        return;
    }
    self->prev_view = self->view;
    self->view = func_08889B08(self);
    if (func_08889468(self, self->prev_view) != func_08889468(self, self->view)) {
        for (int i = 0; i < 2; i++) {
            if (self->units[i] == 0) {
                continue;
            }
            ViewModel *model = self->units[i]->model;
            if (model == 0) {
                continue;
            }
            func_08871BD0(model, func_08871BC4(model));
        }
    }
    for (int i = 0; i < 2; i++) {
        if (self->units[i] == 0) {
            continue;
        }
        ViewModel *model = self->units[i]->model;
        if (model == 0) {
            continue;
        }
        *((unsigned char *)self + 0x7C0 + i) = *((unsigned char *)self + 0x7C2 + i);
        *((unsigned char *)self + 0x7C2 + i) = model->IsShown();
        if (*((unsigned char *)self + 0x7C2 + i) != *((unsigned char *)self + 0x7C0 + i)) {
            func_08871BD0(model, func_08871BC4(model));
        }
    }
}

}
