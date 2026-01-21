/* Basisadresse des Memory-Controllers (siehe Datenblatt AT91RM9200 Kap. 8) */ 
#define MC_BASE 0xFFFFFF00
 
/*
 * Schnittstelle des Memory Controllers
 * (siehe Datenblatt AT91RM9200 Kap. 16.)
 */
struct mc { 
 	unsigned int rcr; 
 	unsigned int asr; 
 	void *aasr; 
 	unsigned int mpr;
};
 
#define RCB (1 << 0)
 
volatile struct mc * const mc = (struct mc *)MC_BASE; 
 
/*
 * mc_remap() - Memory-Remap durchführen
 *
 * Diese Funktion führt den Memory-Remap durch, wie er im Datenblatt
 * des AT91RM9200 in Abschnitt 16.3.3 beschrieben wird.
 *
 * Zunächst ist einer der ROM-Bereiche an Adresse 0x0000 0000 eingeblendet.
 * Jeder Aufruf schaltet zwischen ROM und internem RAM um.
 */
void mc_remap(void)
{
 	mc->rcr = RCB; 
}
 
/*
 * mc_get_abort_address() - letzte Abort-Adresse ermitteln
 *
 * Der Memory-Controller merkt sich welche Adresse warum zu einem Data-Abort
 * geführt hat (siehe AT91RM9200 Handbuch, Kap. 16.3.4). Wir werten hier nur
 * die Adresse selbst aus.
 */
void *mc_get_abort_address(void)
{
 	return mc->aasr; 
}
