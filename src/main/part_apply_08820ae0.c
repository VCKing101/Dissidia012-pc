typedef struct PartInfo { int kind; } PartInfo;
typedef struct Part { int field_00; PartInfo *info; char pad_08[0x2C]; float rate; } Part;
extern void func_089A4180(void *target, void *source, float rate);
extern void func_089A4124(void *target, void *source, float rate);
extern void func_089A40C8(void *target, void *source, float rate);
void func_08820AE0(Part *p, void *target)
{
    int kind = p->info->kind;
    float rate = p->rate;
    if (kind % 3 == 0) {
        func_089A4180(target, target, rate);
    } else if (kind % 3 == 1) {
        func_089A4124(target, target, rate);
    } else {
        func_089A40C8(target, target, rate);
    }
}
