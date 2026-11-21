/* Shows the guard cue for a key (0-11): a held key starts the hold (-2
 * when it cannot), a pressed key starts the press (-3 when it cannot),
 * else the cue is reset; then the cue shows lit (0xA) or dim (0xB) (-1 bad
 * key). */
typedef struct CueEvent {
    unsigned char pad_00[0x64];
    void *owner;
    void *cues;
} CueEvent;

extern void *func_08AB3284(void *owner);
extern int func_08A65000(void *keys, unsigned int key);
extern int func_08A65054(void *keys, unsigned int key);
extern int func_08A650AC(void *keys, unsigned int key);
extern int func_08A6B23C(CueEvent *self);
extern int func_08A6B29C(CueEvent *self);
extern void func_08A96868(void *cues);
extern void func_08A97060(void *cues, int cue);

int func_08A6B65C(CueEvent *self, unsigned int key)
{
    void *keys;
    int one;
    int held;
    if (key >= 12) {
        return -1;
    }
    keys = func_08AB3284(self->owner);
    held = func_08A65000(keys, key);
    one = 1;
    if (held == one) {
        if (func_08A6B23C(self) != 0) {
            return -2;
        }
    } else if (func_08A65054(keys, key) == one) {
        if (func_08A6B29C(self) != 0) {
            return -3;
        }
    } else {
        func_08A96868(self->cues);
    }
    if (func_08A650AC(keys, key) == one) {
        func_08A97060(self->cues, 0xA);
    } else {
        func_08A97060(self->cues, 0xB);
    }
    return 0;
}
