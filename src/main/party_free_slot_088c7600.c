typedef struct Member { short id; char data_02[0x138 - 2]; } Member;
typedef struct Party { Member members[20]; int count; } Party;

int func_088C7600(Party *p)
{
    int count = p->count;
    int slot = -1;
    if (count < 20) {
        int i;
        int j;
        unsigned int used = 0;
        for (i = 0; i < count; i++) {
            used |= 1 << p->members[i].id;
        }
        for (j = 0; j < 20; j++) {
            if ((used & (1 << j)) == 0) {
                slot = j;
                break;
            }
        }
    }
    return slot;
}
