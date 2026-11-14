/* Looks the motion up by id and starts it on the actor. */
extern "C" {

int func_088CB8A4(int id);
void func_088065B4(void *self, int motion, int kind, float value);

void func_08806560(void *self, int id, int kind, float value)
{
    func_088065B4(self, func_088CB8A4(id), kind, value);
}

}
