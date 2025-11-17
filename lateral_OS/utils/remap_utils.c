#include <stdint.h>
// Ziel: von ROM (READ_ONLY) -> SRAM (WRITE_ONLY)
// -> redefine ARM Vectors

// Memory Controller (MC) - Basis Adresse
#define MC_BASE_ADRESS 0xFFFFFF00
// MC Remap Control Register (RCR) (WRITE_ONLY)
#define MC_RCR (*(volatile uint32_t *)(MC_BASE_ADRESS + 0x00))
// RCB = Bit 0 im RCR-Register
#define MC_RCR_RCB (1 << 0)

void memory_remap(void)
{
    //Bit 0 -> Remap
    MC_RCR = MC_RCR_RCB;
}