typedef struct History7FBDC {
    char pad_000[0x8];
    unsigned int values[3];
} History7FBDC;

int func_08A7FBDC(void *self, History7FBDC *history)
{
    if (!history) {
        return -1;
    }
    return history->values[0];
}
