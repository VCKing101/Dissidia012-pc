/* The save manager (*(0x089D7260)) starts the request it has been set up for: flags 0 and 2 (2: the last load's
   header did not match) cleared, then 0x088AF134 runs the savedata utility. Called from the replay save's
   callback 0x088B0C08 after the texts are built. */
typedef struct SaveManager {
    int unk_00;
    union {
        unsigned int word;
        struct {
            unsigned int b0 : 1;
            unsigned int b1 : 1;
            unsigned int header_bad : 1;
        } bits;
    } flags;
    int *buffer;
    void *save;
} SaveManager;

void func_088AF134(SaveManager *manager, int mode);

void func_088AFB30(SaveManager *manager)
{
    manager->flags.bits.header_bad = 0;
    manager->flags.bits.b0 = 0;
    func_088AF134(manager, 0);
}
