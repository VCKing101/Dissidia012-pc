typedef struct QueueEntry {
    int field_00;
    int field_04;
} QueueEntry;

typedef struct EntryQueue {
    unsigned char active;
    char pad_01[0x3];
    QueueEntry entries[100];
    char pad_324[0x4];
    int index;
} EntryQueue;

extern int func_08A65928(EntryQueue *self);
extern int func_08A65CF4(EntryQueue *self, QueueEntry *entry);

int func_08A657C4(EntryQueue *self)
{
    if (!self->active) {
        return -1;
    }
    if (func_08A65928(self) == 1) {
        return -2;
    }
    while (func_08A65CF4(self, &self->entries[self->index]) == 0) {
        self->index++;
        if (func_08A65928(self) == 1) {
            return -3;
        }
    }
    self->index++;
    return 0;
}
