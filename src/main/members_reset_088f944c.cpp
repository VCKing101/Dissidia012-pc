/* Resets the four 0x80-byte members and the count at 0x200. */
struct Member80 {
    char data[0x80];
};

struct MemberSet {
    Member80 members[4];
    int count;
};

extern "C" {
extern void func_088C6FD4(Member80 *member);

void func_088F944C(MemberSet *set)
{
    for (int i = 0; i < 4; i++) {
        func_088C6FD4(&set->members[i]);
    }
    set->count = 0;
}
}
