typedef struct RankOwner {
    char pad_000[0xF4];
    int level;
} RankOwner;

extern int func_08A9C3D4(RankOwner *self, int key);
extern int func_08A9C40C(RankOwner *self, int key);

/* Raises the level up to 5 for raising keys, lowers it down to -5 for lowering keys. */
int func_08A9C344(RankOwner *self, int key)
{
    if (func_08A9C3D4(self, key) == 1) {
        if (self->level < 5) {
            self->level++;
        }
    } else if (func_08A9C40C(self, key) == 1) {
        if (self->level >= -4) {
            self->level--;
        }
    }
    return 0;
}
