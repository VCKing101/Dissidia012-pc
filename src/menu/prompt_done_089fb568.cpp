#include "menu/agent29_types.h"

/* Screen0B6A0: whether the open prompt (+0x3460, -1 for none) has
   finished showing: kind 0 prompts wait for models 0x130 and 0x12F,
   kinds 1 and 2 for model 0x134; once it has, func_089FB0EC takes the
   texts. */
typedef struct Prompt29 {
    int kind;
    char pad_04[0x20];
} Prompt29;

extern "C" {
extern const Prompt29 D_08A0B574[];
void func_089FB0EC(Screen0B6A0 *self, void *texts);
}

struct PromptModel29 {
    static bool Done(MenuModel *model) { return (model->field_20 & 0x20) != 0; }
};

extern "C" int func_089FB568(Screen0B6A0 *self, void *layout, MenuModel **handles, void *texts)
{
    int kind;
    int done;

    if (WORK_INT(self->work, 0x3460) == -1) {
        return 1;
    }
    done = 0;
    kind = D_08A0B574[WORK_INT(self->work, 0x3460)].kind;
    if (kind == 0) {
        if (PromptModel29::Done(handles[0x130]) && PromptModel29::Done(handles[0x12F])) {
            done = 1;
        }
    } else if (kind == 1 || kind == 2) {
        if (PromptModel29::Done(handles[0x134])) {
            done = 1;
        }
    }
    if (done) {
        func_089FB0EC(self, texts);
    }
    return done;
}
