#include "menu/agent36_types.h"

typedef struct Work468E0 {
    char pad_000[0x410];
    void *table;              /* +0x410 */
} Work468E0;

typedef struct Screen468E0 {
    char pad_000[0x418];
    Work468E0 *work;          /* +0x418 */
} Screen468E0;

int func_08A48450(void *table, int key, int column);
int func_08A484D8(void *table, int column, int row);
void func_08A462F4(Screen468E0 *self, int count);
void func_08A462B8(Screen468E0 *self, int lit);
void func_08A46180(Screen468E0 *self, int page);
void func_08A46BC0(Screen468E0 *self, int key);
void func_08A463D4(Screen468E0 *self, int keep, int id);
void func_08A46FB4(Screen468E0 *self, int row);
void func_08A45ACC(Screen468E0 *self, int row);
void func_08A45C14(Screen468E0 *self, int a);

void func_08A468E0(Screen468E0 *self, int base, int offset, int column)
{
    int value[1];
    int arg[3];
    int key = base + offset;
    int row = func_08A48450(self->work->table, key, column);


    if (row == -1) {
        return;
    }
    value[0] = func_08A484D8(self->work->table, 3, row);
    if (value[0] > 0) {
        func_08A462F4(self, value[0]);
    }
    arg[0] = 0;
    value[0] = func_08A484D8(self->work->table, 0, row);
    if (value[0] & 2) {
        arg[0] = 1;
    }
    func_08A462B8(self, arg[0]);
    value[0] = func_08A48450(self->work->table, key, 3);
    arg[0] = value[0] / 3;
    func_08A46180(self, arg[0]);
    func_08A46BC0(self, key);
    value[0] = func_08A484D8(self->work->table, 1, row);
    if (value[0] != -1) {
        func_08A463D4(self, 0, value[0]);
    }
    func_08A46FB4(self, row);
    func_08A45ACC(self, row);
    func_08A45C14(self, 0);
}
