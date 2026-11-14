/* Shows the card unless func_08AF96B0 reports set. */
extern char D_08B00C48[];
extern int func_08AF96B0(void);
extern void func_088D3F00(void *layout, void *card, void *table);

void func_08AF7654(void *layout, void *card)
{
    if (func_08AF96B0() != 0) {
        return;
    }
    func_088D3F00(layout, card, D_08B00C48);
}
