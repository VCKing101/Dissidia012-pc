/* A loaded motion/effect file of a model bank (0x1C bytes), keyed by its file name. */
struct BankSlot {
    char name[0x1A];                   /* +0x00 */
    char used;                         /* +0x1A */
    unsigned char unk_1B;
};

/* The model bank (D_089D5F94): 0xC0 model holders from +0x1C, then two banks of 0xC0
 * file slots from +0xED1C. */
struct ModelBanks {
    unsigned char unk_0000[0xED1C];
    BankSlot slots[2][0xC0];           /* +0xED1C */
};

extern "C" {

extern ModelBanks *D_089D5F94;
int func_08935970(const char *a, const char *b);   /* strcmp */

/* bank_find: the slot of bank 0 or 1 holding the file `name`, or 0. */
BankSlot *func_0886D4B8(int bank, const char *name)
{
    ModelBanks *banks = D_089D5F94;

    for (int i = 0; i < 0xC0; i++) {
        if (func_08935970(banks->slots[bank][i].name, name) == 0) {
            return &banks->slots[bank][i];
        }
    }
    return 0;
}

}
