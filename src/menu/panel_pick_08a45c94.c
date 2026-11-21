#include "menu/agent36_types.h"

/* Panel: picks the current cell: sets the list position to its value (the
 * list up to it, or the last entry when last is set) and starts the list. */

typedef struct Work45C94 {
    char pad_000[0x3E4];
    unsigned int flags;       /* +0x3E4 */
    char pad_3E8[0x4];
    unsigned char field_3EC;  /* +0x3EC */
    char pad_3ED[0x3];
    int column;               /* +0x3F0 */
    int kind;                 /* +0x3F4 */
    int first;                /* +0x3F8 */
    char pad_3FC[0x14];
    void *table;              /* +0x410 */
} Work45C94;

typedef struct Screen45C94 {
    char pad_000[0x418];
    Work45C94 *work;          /* +0x418 */
} Screen45C94;

int func_08A48450(void *table, int index, int kind);
int func_08A48608(void *table);
void func_088D54DC(void *sound);
void func_08A41A40(BattleGlobal36 *battle);

void func_08A45C94(Screen45C94 *self, unsigned char last)
{
    int value[2];
    int count;

    value[0] = func_08A48450(self->work->table, self->work->first + self->work->column, self->work->kind);
    value[1] = func_08A48608(self->work->table);
    self->work->field_3EC = 1;
    self->work->flags |= 1;
    func_088D54DC(D_08A457EC->sound);
    self->work->flags |= 0x100;
    count = value[0];
    if (last) {
        count = value[1] - 1;
    }
    D_09B6DA68.index = value[0];
    D_09B6DA68.count = count;
    func_08A41A40(D_08A457EC);
}
