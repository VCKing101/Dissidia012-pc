typedef struct StageMark {
    char pad00[0xC];
    unsigned char marked;   /* 0x0C */
} StageMark;

extern int func_08B20118(StageMark *mark);

int func_08B20010(StageMark *mark)
{
    if (func_08B20118(mark) == 0) {
        return -1;
    }
    mark->marked = 1;
    return 0;
}
