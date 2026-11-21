/* A loaded motion/effect file of a model bank (0x1C bytes), keyed by its file name. */
struct BankSlot {
    char name[0x1A];                   /* +0x00 */
    char used;                         /* +0x1A */
    unsigned char unk_1B;

    static bool in_use(BankSlot *slot) { return slot->used != 0; }
};

/* The model bank (D_089D5F94): 0xC0 model holders from +0x1C, then two banks of 0xC0
 * file slots from +0xED1C. */
struct ModelBanks {
    unsigned char unk_0000[0xED1C];
    BankSlot slots[2][0xC0];           /* +0xED1C */
};

extern "C" {

extern ModelBanks *D_089D5F94;
void func_088654D4(BankSlot *slot, const char *name, int kind, void *data, int size);

/* bank_load: put a file into the first free slot of bank 0 or 1 (kinds 4 and 0xB). */
void func_0886D354(int bank, const char *name, void *data, int size)
{
    int kinds[2] = {4, 0xB};
    ModelBanks *banks = D_089D5F94;

    for (int i = 0; i < 0xC0; i++) {
        if (!BankSlot::in_use(&banks->slots[bank][i])) {
            func_088654D4(&banks->slots[bank][i], name, kinds[bank], data, size);
            return;
        }
    }
}

}
