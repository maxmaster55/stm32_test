#include "glob.h"
#include <MCAL/flash/flash.h>




int main(void)
{

    flash_unlock();
    flash_lock();

    while(1);
}

