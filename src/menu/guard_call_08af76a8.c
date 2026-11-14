/* Hides the card icon and closes the card unless func_08AF96B0 reports set. */
typedef struct HideCard {
    char pad_00[0x28];
    void *field_28;
} HideCard;

extern char D_08B00C48[];
extern int func_08AF96B0(void);
extern void func_088D9CFC(void *icon);
extern void func_088D3EA0(void *layout, HideCard *card, void *table);

void func_08AF76A8(void *layout, HideCard *card)
{
    if (func_08AF96B0() != 0) {
        return;
    }
    func_088D9CFC(card->field_28);
    func_088D3EA0(layout, card, D_08B00C48);
}
