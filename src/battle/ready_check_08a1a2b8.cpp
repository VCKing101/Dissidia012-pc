struct ReadyEntry {
    char data[0x28];
};

struct ReadyTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    ReadyEntry entries[2];
};

extern "C" {

extern int func_08A01A9C(ReadyEntry *entry);

int func_08A1A2B8(ReadyTask *self)
{
    unsigned char ready = 1;
    for (int i = 0; i < 2; i++) {
        if (!func_08A01A9C(&self->entries[i])) {
            ready = 0;
            break;
        }
    }
    if (ready == 1) {
        self->state = 7;
    }
    return 0;
}

}
