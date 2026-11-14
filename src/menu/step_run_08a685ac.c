/* Runs the common update and the handler of steps 1-4. */
typedef struct StepOwner {
    char pad_00[0x30];
    int field_30;
} StepOwner;

extern void func_08A68654(StepOwner *self);
extern void func_08A686A8(StepOwner *self);
extern void func_08A68F34(StepOwner *self);
extern void func_08A687B4(StepOwner *self);
extern void func_08A688B4(StepOwner *self);

void func_08A685AC(StepOwner *self)
{
    switch (self->field_30) {
    case 1:
        func_08A68654(self);
        func_08A686A8(self);
        break;
    case 2:
        func_08A68654(self);
        func_08A68F34(self);
        break;
    case 3:
        func_08A68654(self);
        func_08A687B4(self);
        break;
    case 4:
        func_08A688B4(self);
        break;
    }
}
