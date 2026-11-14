/* Runs the handler of steps 1-5 around the common update. */
typedef struct StepOwner {
    char pad_00[0x28];
    int field_28;
} StepOwner;

extern void func_08A623FC(StepOwner *self);
extern void func_08A62498(StepOwner *self);
extern void func_08A632A4(StepOwner *self);
extern void func_08A624F0(StepOwner *self);
extern void func_08A62798(StepOwner *self);

void func_08A62344(StepOwner *self)
{
    switch (self->field_28) {
    case 1:
        func_08A623FC(self);
        func_08A62498(self);
        break;
    case 2:
        func_08A632A4(self);
        func_08A623FC(self);
        break;
    case 3:
        func_08A623FC(self);
        func_08A624F0(self);
        break;
    case 4:
        func_08A62798(self);
        break;
    case 5:
        func_08A623FC(self);
        break;
    }
}
