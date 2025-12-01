/*
 * Linker-Skript für unser Betriebssystem, dass das initiale Speicherlayout
 * festlegt.
 *
 * Es gibt zwei Dinge zu beachten:
 *
 * 1. Auf die echte Hardware gehen wir den Umweg über ein Speicherabbild. Dass
 *    heißt, wir dürfen hier nur einen zusammenhängenden Block von Daten/Code
 *    beschreiben.
 * 2. Beim Erstellen des U-Boot-Images ist der Eintrittspunkt hart auf die
 *    Adresse 0x2000 0000 kodiert. ENTRY() wird nur von QEMU ausgewertet.
 *    Wir müssen also dafür sorgen, dass sich _start tatsächlich an 0x2000 0000
 *    befindet.
 */

ENTRY(_start)
SECTIONS
{
	/* Wir beginnen an 0x2000 0000 mit unserem Eintrittspunkt. */
	. = 0x20000000;
	init : {
		system/entry.o(.text)
	}

	/* Dahinter kommt der ganze Rest. */
	.text : {
		*(.text)
	}
}

