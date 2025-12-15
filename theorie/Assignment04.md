# Betriebssysteme Theorieaufgabe 04

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

### Lösung

1.

* Die Nachricht muss unterteilt werden da sie ab einer Größe nicht mehr in ein
  Register passt
* Dies hat vor allem eine Auswirkung wenn man die Nachricht durch übetragen per
  Wert weitergibt, da die Übetragungsgröße des Channels limiert ist und man
somit mehrere Nachrichtenpackete übetragen müsste
* Im Fall von Übertragung durch Referenz müsste man entweder mehrere Referenzen
  weitergeben oder eine Startaddress mit der Länge der Nachricht
* Beim Mapping müsste man die Fähigkeite haben eine variable Menge an
  Speicheraddressen übetragen zu können

2.

* Buffer Overflow - Während der Verarbeitung kann der Zwischenspeicher überlaufen
    * Man könnte den senden thread blockieren bis Nachrichtenaufnahme bereit
      ist, dies könnte aber schwierig sein wenn man asynchrone Kommunikation
      haben will
    * Wenn man durch Referenzen kommuniziert könnte man Speicher markieren der
      noch nicht verarbeitet wurde bis der Empfänger diesen bearbeitet hat
* Interaktions Probleme mit anderen Threads - Was ist wenn der Empfänger
  beschäftigt ist eine Nachricht zu Empfangen während jemand anders auch eine
  andere senden will
    * Man könnte ähnliches zu Scheduling implementieren. Timer der der
      Empfänger zwingt eine andere Nachricht stückweise zu bearbeiten und dann
      round robin 
* Abbruch bei der Übetragen wenn man mehrere Übetragungen brauch
* Wann endet die Nachricht? Was ist wenn das Ende nicht ankommt?
    * Protokoll Einführen was ein Signal zum Ende der Nachricht erlaubt
    * Timer, sodass selbst wenn das Ende nicht ankommt man nicht blockiert bleibt

