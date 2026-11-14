typedef struct StartItem {
    char data[0x24];
} StartItem;

typedef struct StartSet {
    char pad_00[0x1C];
    StartItem items[2];
} StartSet;

extern "C" {

extern int func_08B04F80(StartSet *self);
extern int func_08B04FCC(StartSet *self);
extern void func_08B05AD8(StartItem *item);

int func_08B05164(StartSet *self)
{
    if (func_08B04F80(self) == 0) {
        return -1;
    }
    if (func_08B04FCC(self) == 0) {
        return 0;
    }
    for (int i = 0; i < 2; i++) {
        func_08B05AD8(&self->items[i]);
    }
    return 0;
}

}
