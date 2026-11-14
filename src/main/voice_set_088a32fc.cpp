/* Sets a byte option on the voice's channel. */
struct Channel088A32FC {
    int field_00;
    char inner[4];
};

extern "C" void func_0889F898(void *inner, unsigned char value);

struct Voice088A32FC {
    int field_00;
    int field_04;
    Channel088A32FC *channel;

    static void set(Channel088A32FC *c, unsigned char value) { func_0889F898(c->inner, value); }
};

extern "C" {

void func_088A32FC(Voice088A32FC *v, unsigned char value)
{
    Voice088A32FC::set(v->channel, value);
}

}
