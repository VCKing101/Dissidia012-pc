#include "menu/agent29_types.h"

/* Screen0B6A0: plays the page script, makes the name and description
   texts (0x16, 0x17) of the current record (+0x3166), places the texts
   and starts the page counter (2 pages when +0x2734 is below +0x316C). */
typedef struct Record6CA4 {
    char pad_00[0x4];
    unsigned short name;
    unsigned short text;
} Record6CA4;

extern const unsigned short D_08A09C62[];
extern const unsigned short D_08A09C9A[];
extern const unsigned short D_08A0A12A[];
extern Record6CA4 *func_088C6CA4(int id);
extern void *func_088C6D30(void);
extern void *func_0887AD78(void *source, int arg1, int id, int arg3, int arg4, int arg5);
extern void func_089F3FC8(Screen0B6A0 *self, int count, int id, int mode, int arg);

void func_089F45C8(Screen0B6A0 *self)
{
    Record6CA4 *record;
    int mode;

    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09C62);
    record = func_088C6CA4(WORK_SHORT(self->work, 0x3166));
    if (record != 0) {
        self->work->texts[0x16] = func_0887AD78(func_088C6D30(), 0, record->name, 0, 0, 0);
        self->work->texts[0x17] = func_0887B3EC(self->work->messages, 3, 0, record->text, 0, 0);
    }
    func_088D428C(self->work->handles, self->work->texts, D_08A09C9A, 1);
    mode = 1;
    if (WORK_U16(self->work, 0x2734) < WORK_SHORT(self->work, 0x316C)) {
        mode = 2;
    }
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A12A);
    func_089F3F68(self, 0xA5);
    func_089F3FC8(self, WORK_SHORT(self->work, 0x316C), 0xAA, mode, 0);
}
