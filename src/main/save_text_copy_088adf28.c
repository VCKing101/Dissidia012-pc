/* Copies UI text number `index` (from the text table at +0xEE0) into a 0x80-byte save text field: the replay save
   puts text 0x13 into the savedata parameters' second title (+0x8E4). */
extern char *func_0887AFBC(int *table, int index, char flag);
extern char *func_08935BB4(char *dst, char *src, int size);

void func_088ADF28(int texts, char *dst, int index)
{
    char *text;
    text = func_0887AFBC(*(int **)(texts + 0xEE0), index, 0);
    func_08935BB4(dst, text, 0x80);
}
