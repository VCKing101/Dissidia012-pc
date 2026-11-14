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

extern unsigned char func_08AC12E8(FadeEntryE0 *entry);

/* Number of entries in phases 1 to 3. */
int func_08AC29E8(FadeListE0 *self)
{
    int active = 0;
    for (int i = 0; i < self->count; i++) {
        if (func_08AC12E8(&self->entries[i]) == 1) {
            active++;
        }
    }
    return active;
}

}
