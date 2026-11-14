/* Clears the shared open flag, closes the item and forgets the current entry. */
typedef struct Menu088ABDF0 {
    char pad_00[0xF00];
    int current;
} Menu088ABDF0;

extern unsigned char *D_089D7280;

void func_088822B4(int item);

void func_088ABDF0(Menu088ABDF0 *menu, int item)
{
    *D_089D7280 = 0;
    func_088822B4(item);
    menu->current = 0;
}
