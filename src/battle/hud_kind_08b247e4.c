typedef struct HudItem {
    char pad00[0x20];
    unsigned char kind;     /* 0x20 */
} HudItem;

int func_08B247E4(HudItem *item)
{
    if (item->kind >= 4 && item->kind < 8) {
        return 1;
    }
    return 0;
}
