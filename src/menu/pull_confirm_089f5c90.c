#include "menu/agent29_types.h"

/* Screen0B6A0: once the confirm prompt is answered yes, sets up the pull:
   takes the chosen entry (+0x38A8 into the pair table at +0x2A60C), resets
   the page, saves and moves to state 5. */
typedef struct EntryPair29 {
    unsigned short field_00;
    unsigned short entry;
    char pad_04[0x4];
} EntryPair29;

extern int func_089FB568(Screen0B6A0 *self, void *layout, void *handles, void *texts);
extern int func_089FBF94(Screen0B6A0 *self);
extern void func_089FABF8(Screen0B6A0 *self);
extern void func_089F5460(Screen0B6A0 *self);
extern void func_08A05C78(Screen0B6A0 *self);
extern void func_089F2578(Screen0B6A0 *self, int entry);
extern void func_089F5728(Screen0B6A0 *self);
extern void func_089F5588(Screen0B6A0 *self);
extern void func_088F2C18(void *table);
extern void func_088FE9CC(void);

void func_089F5C90(Screen0B6A0 *self)
{
    if (func_089FB568(self, &self->work->layout, self->work->handles, self->work->texts) == 0) {
        return;
    }
    if (func_089FBF94(self) == 0) {
        return;
    }
    self->work->field_38A0 = 0;
    func_089FABF8(self);
    WORK_U16(self->work, 0x272E) =
        ((EntryPair29 *)((char *)self->work + 0x2A60C))[self->work->field_38A8].entry;
    func_089F5460(self);
    func_08A05C78(self);
    func_089F2578(self, (short)WORK_U16(self->work, 0x272E));
    WORK_BYTE(self->work, 0x389C) = 0;
    func_089F5728(self);
    func_089F5588(self);
    func_088F2C18((char *)D_089D712C + 0x1E208);
    func_088FE9CC();
    func_088AC8C8(D_089D712C, 1);
    self->work->state = 5;
}
