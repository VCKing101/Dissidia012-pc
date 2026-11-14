/* Percentage of the table's count over its total, compared with the limit. */
typedef struct Result088C3D7C { int value; int limit; } Result088C3D7C;
typedef struct Big088C3D7C { char pad[0xE788]; int table; } Big088C3D7C;
void func_088B95E8(int *count, int *total, int *table);
int func_088C3D7C(Result088C3D7C *r, Big088C3D7C *big, int *limit)
{
    int count;
    int total;
    int percent;
    func_088B95E8(&count, &total, &big->table);
    percent = count * 100 / total;
    r->value = percent;
    r->limit = *limit;
    return percent >= r->limit;
}
