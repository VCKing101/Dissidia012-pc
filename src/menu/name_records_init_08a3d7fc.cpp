#include "menu/agent36_types.h"

/* Name list screen: sets up the 26 character records (from D_08A40880)
 * and the "all" record, and the totals of six kinds. */

struct Chara3D7FC {
    char pad_00[0x3];
    unsigned char field_03;
};

extern "C" {
extern signed char D_08A40880[];
void *func_088D5AC8(void *block, int a, int b, int c);
Chara3D7FC *func_088B70C8(int chara);
void func_08A3F518(NameScreen36 *self, NameRecord36 *record);
void func_08A3D790(NameScreen36 *self, int kind);
}

extern "C" void func_08A3D7FC(NameScreen36 *self)
{
    int i;
    NameRecord36 *record;
    NameRecord36 *all;

    self->work->field_2A00 = func_088D5AC8(self->work->block_2838, 0x23, 1, 0);
    self->work->record_count = 0x1A;
    for (i = 0; i < self->work->record_count; i++) {
        record = &self->work->records[i];
        record->chara = D_08A40880[i];
        record->field_08 = func_088B70C8(record->chara)->field_03 != 0;
        func_08A3F518(self, record);
    }
    func_08A3D790(self, 0x15);
    func_08A3D790(self, 0x16);
    func_08A3D790(self, 0x1D);
    func_08A3D790(self, 0x19);
    func_08A3D790(self, 0x20);
    func_08A3D790(self, 0x17);
    all = &self->work->all;
    all->chara = -1;
    func_08A3F518(self, all);
}
