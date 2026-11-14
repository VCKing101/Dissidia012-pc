typedef struct SyncInner {
    char pad_000[0x4E0];
    int field_4E0[8];
    char pad_500[0xC90 - 0x500];
    int field_C90;
    int field_C94;
    int field_C98;
} SyncInner;

typedef struct SyncOwner {
    char pad_0000[0xD80];
    SyncInner *field_D80;
} SyncOwner;

extern void func_088D9CD8(int handle);
extern void func_088D9CFC(int handle);

void func_08A168D4(SyncOwner *self)
{
    int i = 0;
    SyncInner *inner = self->field_D80;
    int offset = 0;
    do {
        func_088D9CFC(*(int *)((char *)inner + offset + 0x4E0));
        i++;
        offset += 4;
        inner = self->field_D80;
    } while (i < 7);
    if (inner->field_C90 == 0) {
        if (inner->field_C98 == 0) {
            func_088D9CD8(inner->field_4E0[0]);
        }
    } else if (inner->field_C90 == 1) {
        int sel = inner->field_C98;
        if (sel == 0) {
            func_088D9CD8(inner->field_4E0[1]);
        } else if (sel != inner->field_C94 - 1) {
            int prev = sel - 1;
            func_088D9CD8(*(int *)((char *)inner + prev * 4 + 0x4E8));
        }
    }
}
