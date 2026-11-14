typedef struct SyncState {
    char pad_0000[0x7C14];
    int field_7C14;
    char pad_7C18[0x802C - 0x7C18];
    int field_802C;
    char pad_8030[0x82F0 - 0x8030];
    int field_82F0;
} SyncState;

typedef struct SyncOwner {
    char pad_000[0x154];
    SyncState *field_154;
} SyncOwner;

extern int func_08AB2F24(int handle);

void func_08A9B950(SyncOwner *self)
{
    if (self->field_154->field_802C != 0) {
        self->field_154->field_7C14 = func_08AB2F24(self->field_154->field_802C);
        self->field_154->field_82F0 = self->field_154->field_7C14;
    }
}
