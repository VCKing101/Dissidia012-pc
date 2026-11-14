/* Returns the phase byte of a fade (first byte of a small array, read
   through the decayed pointer, which SNC does not fold). */
typedef struct FadeState {
    char pad_00[0xC];
    unsigned char phase[4];     /* 0x0C */
} FadeState;

unsigned char func_08920850(FadeState *fade)
{
    unsigned char *p = fade->phase;
    return *p;
}
