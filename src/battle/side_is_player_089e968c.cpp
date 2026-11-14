/* True when the given side's controller is the local player. */
struct SideTable {
    unsigned char pad_00[0x38];
    int controllers[2];
};

extern "C" {

extern SideTable *D_089D6A10;
extern int func_08842164(void);

bool func_089E968C(unsigned int side)
{
    if (side >= 2) {
        return false;
    }
    int controller = D_089D6A10->controllers[side];
    return controller == func_08842164();
}

}
