typedef struct CardOwner { char pad_00[0xC]; void *table; } CardOwner;
extern void func_089272CC(CardOwner *owner, int kind, int id);
void func_08927A7C(CardOwner *owner, short id)
{
    func_089272CC(owner, 0, id);
}
