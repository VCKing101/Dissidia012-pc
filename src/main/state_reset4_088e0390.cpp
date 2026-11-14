/* Moves an active state (non-zero) to state 4. */
struct StateWord {
    int state;

    static bool active(StateWord *s)
    {
        return s->state != 0;
    }
};

extern "C" {
void func_088E0390(StateWord *s)
{
    if (StateWord::active(s)) {
        s->state = 4;
    }
}
}
