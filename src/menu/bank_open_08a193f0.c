/* Screen1AB08: opens the bank page for item `which`. With mode 0 it also
   lists the (up to five) positive amounts of record kinds[kind] of the
   save's bank table, resets the page and sets flag 0x20. Standalone:
   the work fields it uses are not in the region header. */
typedef struct BankAmount29 {
    int value;
    short field_04;
    short field_06;
} BankAmount29;

typedef struct BankWork29 {
    char pad_0000[0x10EC];
    unsigned int flags;   /* +0x10EC */
    char pad_10F0[0xC];
    int kinds[5];         /* +0x10FC */
    int items[0x10];      /* +0x1110 */
    char pad_1150[0x3F0];
    BankAmount29 amounts[5]; /* +0x1540 */
    char pad_1568[0x138];
    int page;             /* +0x16A0 */
    int count;            /* +0x16A4 */
    int item;             /* +0x16A8 */
    int field_16AC;
} BankWork29;

typedef struct BankScreen29 {
    void *vtable;
    char pad_04[0x17AC];
    BankWork29 *work; /* +0x17B0 */
} BankScreen29;

typedef struct BankRecord29 {
    struct {
        int value;
        int field_04;
    } amounts[9];
} BankRecord29;

typedef struct BankTable29 {
    char pad_00[0x24];
    BankRecord29 records[1];
} BankTable29;

extern char *D_089D712C;

void func_08A193F0(BankScreen29 *self, int mode, int kind, int which)
{
    BankTable29 *table = (BankTable29 *)(D_089D712C + 0x3E758);
    int count;
    int i;
    int value;

    if (mode == 0) {
        count = 0;
        for (i = 0; i < 5; i++) {
            value = table->records[self->work->kinds[kind]].amounts[i].value;
            if (value > 0) {
                self->work->amounts[i].value = value;
                count++;
                self->work->amounts[i].field_04 = 0;
                self->work->amounts[i].field_06 = 0;
            }
        }
        self->work->count = count;
        self->work->page = 0;
        self->work->item = self->work->items[which];
        self->work->field_16AC = 5;
        self->work->flags |= 0x20;
    } else {
        self->work->item = self->work->items[which];
        self->work->field_16AC = 5;
    }
}
