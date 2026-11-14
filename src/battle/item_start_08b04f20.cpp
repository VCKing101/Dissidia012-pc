typedef struct StartItem {
    char data[0x24];
} StartItem;

typedef struct StartSet {
    char pad_00[0x1C];
    StartItem items[2];
} StartSet;

extern "C" {

extern int func_08B04F80(StartSet *self);
extern void func_08B054F0(StartItem *item);

int func_08B04F20(StartSet *self)
{
    if (func_08B04F80(self) == 0) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        func_08B054F0(&self->items[i]);
    }
    return 0;
}

}
