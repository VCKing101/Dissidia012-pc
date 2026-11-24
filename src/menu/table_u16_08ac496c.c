/* Returns the first halfword of the 12-byte record at index (a1 & 0xFFFF) of D_08AC7370. */
typedef struct { unsigned short v; unsigned short pad[5]; } Rec12;
extern Rec12 D_08AC7370[];
unsigned short func_08AC496C(int a0, unsigned int idx) {
    return D_08AC7370[idx & 0xFFFF].v;
}
