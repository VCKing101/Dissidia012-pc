/* Starts the reference ref: an unknown id shows notice 2, a known one goes
 * to the list (func_08A581F0). True when started. */
struct Id4E1F4 {
    int low;
    int high;
    static bool IsNone(const Id4E1F4 &id) {
        if ((id.low & id.high) == -1) {
            return true;
        } else {
            return false;
        }
    }
};

struct Work4E1F4 {
    char pad_0000[0xC];
    void *messages;           /* +0x000C */
    char pad_0010[0x6FC0];
    void *list;               /* +0x6FD0 */
};

struct Screen4E1F4 {
    char pad_00[0x5C];
    Work4E1F4 *work;          /* +0x5C */
};

extern "C" {
void func_08A4D6D8(void);
Id4E1F4 func_08928B90(int key);
void func_08A4E2E4(Screen4E1F4 *self, int kind, void (*a)(void), void (*b)(void), void (*c)(void), int id, int d, int e, int f);
void func_08A581F0(void *list, int *ref, void *messages, void (*done)(void), int arg);
}

extern "C" bool func_08A4E1F4(Screen4E1F4 *self, int *ref, int arg)
{
    Id4E1F4 id = func_08928B90(*ref);

    if (!Id4E1F4::IsNone(id)) {
        func_08A4E2E4(self, 2, func_08A4D6D8, func_08A4D6D8, func_08A4D6D8, id.low, -1, -1, -1);
        return false;
    }
    func_08A581F0(self->work->list, ref, self->work->messages, func_08A4D6D8, arg);
    return true;
}
