/* Clears the shared open flag, closes the item and forgets the current entry. */
typedef struct Menu088AFA94 {
    int current;
} Menu088AFA94;

extern unsigned char *D_089D7280;

void func_088822B4(int item);

void func_088AFA94(Menu088AFA94 *menu, int item)
{
    *D_089D7280 = 0;
    func_088822B4(item);
    menu->current = 0;
}
