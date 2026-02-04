/*
 * Linker-Skript für allein stehende Anwendung.
 *
 * Würde man es schaffen, das Speicherabbild an die entsprechende Adresse zu
 * laden, könnte man den Einsprungspunkt anspringen und die Anwendung
 * läuft.
 */

ENTRY(main)
SECTIONS
{
	. = 0x80000000;
	.text : {
		*(.text*)
	}
}

