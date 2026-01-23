#include <stdio.h>
#include <stdlib.h>

#include "../headers/allocator.h"

int main() {
    byte mask = 0;

    SET_SECTOR_OCCUPIED(mask, 2);
    CLEAR_SECTOR_OCCUPIED(mask, 2);

    printf("%i", IS_SECTOR_OCCUPIED(mask, 2));
}
