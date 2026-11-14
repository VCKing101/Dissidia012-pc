typedef struct KindTask {
    char pad_00[0x4];
    short kind;
} KindTask;

extern void func_08ABD4CC(KindTask *self);
extern void func_08ABD40C(KindTask *self);
extern void func_08ABD2E8(KindTask *self);
extern void func_08ABD298(KindTask *self);

void func_08ABC63C(KindTask *self)
{
    switch (self->kind) {
    case 1:
        func_08ABD298(self);
        break;
    case 2:
        func_08ABD2E8(self);
        break;
    case 4:
        func_08ABD40C(self);
        break;
    case 5:
        func_08ABD4CC(self);
        break;
    }
}
