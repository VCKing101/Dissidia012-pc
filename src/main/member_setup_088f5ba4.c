/* Resets the member, fills it from the arguments and stores the last one at 0x2D4. */
typedef struct PartyMember {
    char pad_000[0x2D4];
    int owner;
} PartyMember;

extern void func_088F4F14(PartyMember *member);
extern void func_088F5FEC(PartyMember *member, int a, int b, int c, int d, int e, int f);

void func_088F5BA4(PartyMember *member, int a, int b, int c, int d, int e, int f, int owner)
{
    func_088F4F14(member);
    func_088F5FEC(member, a, b, c, d, e, f);
    member->owner = owner;
}
