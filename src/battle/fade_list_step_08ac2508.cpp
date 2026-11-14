typedef struct FadeEntryE0 {
    char data[0xE0];
} FadeEntryE0;

typedef struct FadeListE0 {
    unsigned char active;
    char pad_01[0x2B];
    int count;
    FadeEntryE0 *entries;
} FadeListE0;

extern "C" {

extern void func_08AC11DC(FadeEntryE0 *entry, float dt);

void func_08AC2508(FadeListE0 *self, float dt)
{
    for (int i = 0; i < self->count; i++) {
        func_08AC11DC(&self->entries[i], dt);
    }
}

}
