typedef struct IdPair {
    short from;
    short to;
} IdPair;

typedef struct IdMap {
    unsigned char active;
    char pad_001[0x2BB];
    int count;
    IdPair *pairs;
} IdMap;

/* Maps an id through the pair table; unknown ids map to themselves. */
int func_08AABDC8(IdMap *self, int id)
{
    if (self->active == 0) {
        return id;
    }
    {
        int i;
        for (i = 0; i < self->count; i++) {
            if (self->pairs[i].from == id) {
                return self->pairs[i].to;
            }
        }
    }
    return id;
}
