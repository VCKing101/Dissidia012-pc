/* Text id for kinds 0-2 (0x8A, 0x89, 0x88); 0x8B for anything else. */
extern "C" {
int *func_08A776F0(int *first, int *last, int *value);

int func_08A77658(void *unused, int kind)
{
    int kinds[3] = { 0, 1, 2 };
    int *found = func_08A776F0(kinds, kinds + 3, &kind);
    int text = 0x8B;
    if (found != kinds + 3) {
        int texts[3] = { 0x8A, 0x89, 0x88 };
        text = texts[found - kinds];
    }
    return text;
}
}
