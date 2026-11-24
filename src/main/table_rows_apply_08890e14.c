/* Reads a table's header and applies every row that matches the current
   group and slot; returns -1 when closed, -2 when the header fails. */
typedef struct TableHeader {
    char pad_00[0xC];
    unsigned short first;
    unsigned short count;
    char pad_10[0x58 - 0x10];
} TableHeader;

typedef struct TableRow {
    char pad_00[0x2C];
    short kind;
    char pad_2E[0x38 - 0x2E];
    unsigned short id;
    char pad_3A[3];
    signed char slot;
    char pad_3E[2];
} TableRow;

typedef struct Table {
    unsigned char open;
    char pad_01[3];
    int a;
    char pad_08[0x18 - 8];
    int b;
    int group;
} Table;

extern int func_088916A4(Table *self, TableHeader *header);
extern int func_08891B8C(Table *self, int a, int b);
extern int func_088912D0(Table *self);
extern int func_08891894(Table *self, TableRow *row, int index);
extern int func_08891900(Table *self);
extern void func_08890FC4(Table *self, int id);

int func_08890E14(Table *self)
{
    TableHeader header;

    if (self->open == 0) {
        return -1;
    }
    if (func_088916A4(self, &header) != 0) {
        return -2;
    }
    {
        TableRow row;
        int grouped = func_08891B8C(self, self->a, self->b);
        int group = 0;
        int by_slot = func_088912D0(self);
        int slot = -1;
        int i;

        for (i = 0; i < header.count; i++) {
            if (func_08891894(self, &row, header.first + i) != 0) {
                continue;
            }
            if (grouped == 1) {
                if (row.kind == 6 && i != 0) {
                    group++;
                }
                if (group != self->group) {
                    continue;
                }
            }
            if (by_slot == 1) {
                if (row.slot >= 0) {
                    slot = row.slot;
                }
                if (slot != func_08891900(self)) {
                    continue;
                }
            }
            func_08890FC4(self, row.id);
        }
    }
    return 0;
}
