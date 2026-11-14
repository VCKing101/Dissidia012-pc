/* Closes the current download (if any) and clears its state word. */
typedef struct Download {
    char pad_000[0x1C8];
    int state;
} Download;

extern Download *D_089D8120;
extern int func_088FEC98(Download *dl, unsigned char arg);

int func_088FF1AC(unsigned char arg)
{
    int result;

    if (D_089D8120 == 0) {
        return 0;
    }
    {
        Download *dl = D_089D8120;

        result = func_088FEC98(dl, arg);
        dl->state = 0;
    }
    return result;
}
