typedef struct ViewMark {
    char pad00[0x10];
    unsigned char marked;   /* 0x10 */
} ViewMark;

extern unsigned char func_08B27FC0(ViewMark *mark);

int func_08B27F80(ViewMark *mark)
{
    if (func_08B27FC0(mark) == 0) {
        return 0;
    }
    mark->marked = 1;
    return 0;
}
