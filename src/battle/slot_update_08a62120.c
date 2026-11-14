typedef struct SlotUpdate62120 {
    unsigned char active;
} SlotUpdate62120;

extern int func_08A6217C(SlotUpdate62120 *self);
extern void func_08A620B0(SlotUpdate62120 *self, int index);

void func_08A62120(SlotUpdate62120 *self)
{
    if (!func_08A6217C(self)) {
        return;
    }
    {
        int i;
        for (i = 0; i < 10; i++) {
            func_08A620B0(self, i);
        }
    }
}
