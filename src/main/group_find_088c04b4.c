/* Address of the group with the id; 0 when unknown. */
typedef struct Group088C04B4 {
    char data[0x57C];
} Group088C04B4;

int func_088C04F8(int id);
Group088C04B4 *func_088C04B4(Group088C04B4 *groups, int id)
{
    int index = func_088C04F8(id);
    Group088C04B4 *result = 0;
    if (index >= 0) {
        result = &groups[index];
    }
    return result;
}
