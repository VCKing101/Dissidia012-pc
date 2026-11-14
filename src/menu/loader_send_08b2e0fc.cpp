/* Sends `value` to the loader (command 1) when there is one. */
struct State08B2E0FC {
    char pad_000[0x6C4];
    void *field_6C4;
};

struct Owner08B2E0FC {
    char pad_000[0x844];
    State08B2E0FC *field_844;
};

extern "C" void func_08A814D4(void *loader, int command, int value);

struct Send08B2E0FC {
    static void send(int value, void *loader) { func_08A814D4(loader, 1, value); }
};

extern "C" {
void func_08B2E0FC(Owner08B2E0FC *self, int value)
{
    if (self->field_844->field_6C4 != 0) {
        Send08B2E0FC::send(value, self->field_844->field_6C4);
    }
}
}
