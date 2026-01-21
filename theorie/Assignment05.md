# Betriebssysteme Theorieaufgabe 05

von Bennet und Feliks

### Aufgabenstellung

*Im Allgemeinen sind an Kommunikationsobjekten Nachrichten unterschiedlicher
Länge zugelassen.*
1. *Welche Auswirkungen hat dies auf die Daten des Kommunikationsobjekts? Bei
   welchen Varianten der Nachrichtenübergabe hat dies weitere Auswirkungen, und
bei welchen hat dies sonst keine weiteren Auswirkungen?*
2. *Welche Probleme können aus der Sicht der Kommunikationspartner auftreten,
   wenn Nachrichten unterschiedlicher Länge zugelassen sind? Schlagen Sie
geeignete Lösungen vor!*


*Erläutern Sie die Begriffe:*
* *Memory maped I/O*
* *DMA(direct memory access)*
* *Polling*
*Gegeben sei nun ein Prozessor mit einer Takfrequenc von 200 MHz. Eine Poling
operation benötigt 400 Taktzyklen. Berechnen Sie die prozentuale CPU-Auslastung
durch das Polling für die folgenden Geräte. Bei 2 und 3 ist dabei die
Abfragerate so zu wählen, dass keine Daten verloren gehen.*
1. *Maus mit einer Abfragerate von 30/sec*
2. *Diskettenlaufwerk: Datentransfer in 16-bit-Wörtern mit einer Datenrate von
   50KB/sec*
3. *Plattengerät, das die Daten in 32-bit Wörtern mit einer Rate von 2 MB/sec
   transportiert*

*(Hinweis: Wie viele Taktzyklen gibt es pro Sekunde? Wie viele Taktzyklen
werden für eine Abtastrate von 30/sec benötigt? Was für eine Abtastrate
entsprechen 50 KB/sec bei 16-bit-Wörtern?)*

*Für das Plattengerät soll jetzt DMA eingesetzt werden. Wir nehmen an, dass für
das Initialisieren des DMA 4000 Takte, für eine Interrupt Behandlung 2000 Takte
benötigt werden und dass der DMA 4 KB transportiert werden. Das Plattengerät
sei ununterbrochen aktiv. Zugriffskonflikte am Bus zwischen Prozessor und
DMA-Steuereinheit werden ignoriert.*

*Wie hoch ist nun die prozentuale Belastung des Prozessors? (Hinweise: Wie
viele DMA-Transfers pro Sekunde sind bei 2 MB/sec und 4KB Transfergröße
notwendig?)


### Lösung

1. Memory mapped I/O - Geräte haben einen Addressraum im Hauptspeicher wodurch
   die CPU mit dem Gerät interagiert, in Kontrast zu Interaktion durch
   spezielle I/O Operationen

2. Direct memory access - The Controller des Geräts kann direkt in den Speicher
   schreiben und benötigt nicht die CPU dazu
 
3. Polling - Dauerhafte abfrage nach Input an I/O Geräten


* 200 MHz = 200 000 000 Hz -> 2 * 10^8 Taktzyklen in einer Sekunde
* 400 Taktzyklen für Polling
1. Maus 30 Abfragen in der Sekunde

-> Taktzyklen insgesamt = Taktzyklen pro Abfrage * Abfragen = 400 * 30 = 12 000
-> (12 * 10^3)/(2 * 10^8) = 6 * 10^-5 = 0,000006 ^= 0.0006% prozentuale CPU Auslastung

2. Distkettenlaufwerk: 16-bit-Wörter mit Datenrate von 50 KB/sec

-> 50 KB/sec ^= 400 000 bit/sec
-> 400 000/16 = 25 000 Wörter
-> Man schafft 25 000 Wörter pro Sekunde
-> pro Wort eine Polling Operation
-> 25 000 * 400 = 10 000 000 ^= 10^7 Taktzyklen
-> 10^7/(2 * 10^8) = 0.05 ^= 5% prozentuale CPU Auslastung

3. Plattengeräte: 32-bit-Wörter, 2MB/sec Datenrate

-> 2MB/sec ^= 16 000 000 bit = 1,6 * 10^7
-> 16 000 000/32 = 500 000 Wörter pro Sekunde
-> 500 000 * 400 = 200 000 000 = 2 * 10^8  Taktzyklen
-> (2 * 10^8)/(2 * 10^8) = 1 ^= 100% prozentuale CPU Auslastung

4. Plattengerät mit DMA: 4000 Takte init, 2000 Takte interrupt pro DMA 4KB

-> 4000 Takte Initialisierung
-> 4 KB ^= 32 000 bit
-> 32 000/32 = 1000 Wörter pro Sekunde
-> 4000 + 1000 * 2000 = 2 004 000
-> 2 004 000/(2 * 10^8) = 0,01002 ^= 1,002% prozentuale CPU Auslastung 
-> hier wird die Rate von dem Plattengerät ignoriert da die Datenrate pro DMA
niedriger ist und somit der Bottleneck ist

