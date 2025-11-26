# Betriebssysteme Theorieaufgabe 03

von Bennet und Feliks

*Grenzen Sie die folgenden Begriffe auf
Grund der Vorlesung gegeneinander ab:*
- gegenseitiger Ausschluss
- Signalisierung
- Synchronisation
- Koordination
- Kommunikation
- Kooperation 

Synchronisation ist eine Art der Koordination, wo Koordination der zeitliche
Aspekt der Thread Interaktion ist und somit ein Voraussetzung von Kommunikation
und Kooperation darstellt. 

Gegenseitiger Ausschluss und Signalisierung sind unter anderem ein Hilfsmittel
zur Interaktion zwischen Threads, während der Rest der Begriffe verschiedene
Formen der Interaktion beschreibt. Beispielsweise ist Signaling nützlich für
zeitliche Koorindation zwischen Threads aber auch nutzvoll bei nutzen von
shared Data, und somit kann sich das auch auf Kommunkation erweitern.

Beim gegegnseitigen Auschluss stellt man sicher, dass nur ein Thread zu einem
Zeitpunkt eine Handlung ausführt. Vor allem ist dies nützlich um kritische
Vorgänge zu sichern die nicht unterbrochen werden sollten.

Im Gegensatz dazu ist Signalisierung eine einfache Methode einen zeitlichen
Zustand zu kommunizeren.

Bei der Kooperation hat man mehrere Threads die an einer Aufgabe über den
selben Speicherraum arbeiten. Die wichtige Aufgabe bei diesem Aspekt ist den
gemeinesem Speicher zu verwalten.

Dagegen ist Kommunikation der Austausch von Information zwischen Threads in
verschiedenen Variationen wobei es nicht dringend den selben Speicherraum
vorraussetzt.

