struct Slot24B84 {
    short id;
    short on;
};

struct Work24B84 {
    char pad_00[0x23BC];
    Slot24B84 slots[20];
};

struct Screen24B84 {
    char pad_00[0x28F0];
    Work24B84 *work;
};

struct Record24B84 {
    char pad_00[4];
    unsigned short flags;
    char pad_06[0x138 - 6];
};

struct Records24B84 {
    Record24B84 records[20];
    int count;
};

extern "C" {
extern int D_089D712C;
}

extern "C" void func_08A24B84(Screen24B84 *self)
{
    int i;
    int k;
    int count;
    Records24B84 *list;
    for (i = 0; i < 20; i++) {
        self->work->slots[i].id = -1;
    }
    list = (Records24B84 *)(D_089D712C + 0x39F38);
    count = list->count;
    k = 0;
    for (i = 0; i < count; i++) {
        Record24B84 *rec = &list->records[i];
        if (rec->flags & 1) {
            self->work->slots[k].id = i;
            self->work->slots[k].on = 1;
            k++;
        }
    }
}
