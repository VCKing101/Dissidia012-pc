typedef struct MotionUnit {
    char pad_000[0x180];
    short motion;
} MotionUnit;

typedef struct GuardWatch {
    unsigned char active;
    char pad_01[0x3];
    MotionUnit *unit;
    char pad_08[0x4];
    char track[0x44];
    unsigned char field_50;
    unsigned char field_51;
    unsigned char field_52;
    char pad_53[0xD];
    char field_60[0x10];
} GuardWatch;

extern int func_08AB3958(void *track, int id);
extern void func_08A966AC(GuardWatch *self, void *data);
extern int func_08A83FDC(MotionUnit *unit);
extern void func_08A969BC(GuardWatch *self, int mode);

int func_08A965E0(GuardWatch *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->unit->motion == 0x1B8 && func_08AB3958(self->track, 10) == 0 && func_08AB3958(self->track, 11) == 0) {
        func_08A966AC(self, self->field_60);
    }
    if (self->field_50 == 0 && self->field_51 == 1 && self->field_52 == 1 && func_08A83FDC(self->unit) == 1) {
        func_08A969BC(self, 1);
    }
    return 0;
}
