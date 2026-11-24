/* Resolves a sound bank's data block and its entry table (past the 16-byte
   header) once the bank is loaded. */
typedef struct BankRef {
    int bank;
    char *data;
    char *entries;
} BankRef;

extern char *func_088A0CF8(int bank);

void func_088A39D4(BankRef *self)
{
    int bank = self->bank;

    if (bank != 0) {
        BankRef *keep[1];
        char *data;
        keep[0] = self;
        data = func_088A0CF8(bank);
        self = keep[0];
        self->data = data;
        self->entries = data + 0x10;
    }
}
