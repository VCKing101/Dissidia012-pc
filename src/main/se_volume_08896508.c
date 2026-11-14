/* Sets the volume of both menu sound channels (0x18703, 0x18704). */
void func_088A27D0(int channel, float volume, float pan);

void func_08896508(float volume)
{
    float zero = 0.0f;
    func_088A27D0(0x18703, volume, zero);
    func_088A27D0(0x18704, volume, zero);
}
