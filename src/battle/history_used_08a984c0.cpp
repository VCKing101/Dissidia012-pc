struct HistoryEntry {
    char pad_00[0xD];
    signed char used;
    char pad_0E[0x2];
};

struct History {
    unsigned char active;
    char pad_01[0x73];
    HistoryEntry *entries;
    unsigned int capacity;
    unsigned int count;
};

extern "C" {

extern int func_08899944(void);
extern int func_08A98828(History *self);

/* True when the newest history entry is marked used. */
bool func_08A984C0(History *self)
{
    if (self->active == 0) {
        return false;
    }
    if (func_08899944() == 1) {
        return false;
    }
    if (func_08A98828(self) == 0) {
        return false;
    }
    unsigned int last = self->count - 1;
    if (last >= self->capacity) {
        return false;
    }
    return self->entries[last].used != 0;
}

}
