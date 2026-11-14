/* Returns the fighter unit of a side, or none for a bad side. */
struct UnitTable {
    unsigned char pad_00[0x38];
    void *units[2];
};

extern "C" {

extern UnitTable *D_089D6A10;

void *func_08A22C78(unsigned int side)
{
    if (side >= 2) {
        return 0;
    }
    return D_089D6A10->units[side];
}

}
