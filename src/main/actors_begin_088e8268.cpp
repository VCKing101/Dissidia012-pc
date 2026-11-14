/* Starts an actor pass: flags the current actor done and clears the global busy flag. */
struct ActorFlag {
    bool done;
};

struct BattleFlags {
    char pad_000[0x6B2];
    bool busy;

    static void setBusy(BattleFlags *flags, bool busy)
    {
        flags->busy = busy;
    }
};

struct ActorListB {
    ActorFlag *current;
    char pad_04[0x48 - 4];
    unsigned char dirty;
};

extern "C" {
extern BattleFlags *D_089D6AC8;

void func_088E8268(ActorListB *list)
{
    if (list->current != 0) {
        list->current->done = true;
    }
    list->current = 0;
    if (list->dirty != 0) {
        BattleFlags::setBusy(D_089D6AC8, false);
    }
}
}
