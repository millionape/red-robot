#include <button.h>

void init_buttons(void)
{
    // set port G4 as input
    DDRG &= ~(1 << 4);
}