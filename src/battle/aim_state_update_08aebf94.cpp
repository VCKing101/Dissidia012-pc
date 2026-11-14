typedef struct UnitFlags {
    char pad_00[0x2];
    unsigned char bits;

    static bool guarding(UnitFlags *flags) { return (flags->bits & 4) != 0; }
} UnitFlags;

typedef struct FlagUnit {
    char pad_000[0x5B0];
    UnitFlags flags;
} FlagUnit;

typedef struct AimState {
    char pad_00[0x8];
    FlagUnit *unit;
    int guarded;
} AimState;

extern "C" {

extern float func_08AEC338(AimState *self, int arg);
extern void func_08AEC1EC(AimState *self, int target, float angle);

void func_08AEBF94(AimState *self, int target)
{
    func_08AEC1EC(self, target, func_08AEC338(self, 0));
    if (UnitFlags::guarding(&self->unit->flags) == 1) {
        self->guarded = 1;
    }
}

}
