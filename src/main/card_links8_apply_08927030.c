typedef struct CardLink8 { short id; short sub; int value; } CardLink8;
typedef struct CardBody { char pad_000[0xB7]; signed char link8_count; char pad_0B8[4]; CardLink8 links8[120]; } CardBody;
typedef struct CardUnit { char pad_00[0x20]; CardBody *body; } CardUnit;
extern void func_08926F08(CardUnit *unit, void *a, void *b, short id, int value, short sub);
void func_08927030(CardUnit *unit, void *a, void *b)
{
    int i;
    for (i = 0; i < unit->body->link8_count; i++) {
        func_08926F08(unit, a, b, unit->body->links8[i].id, unit->body->links8[i].value, unit->body->links8[i].sub);
    }
}
