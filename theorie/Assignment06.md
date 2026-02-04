# Betriebssysteme Theorieaufgabe 06

von Bennet und Feliks

### Aufgabenstellung

Um die in der Vorlesung beschriebenen Auswahlstrategien der Speicherzuteilung
zu implementieren, muss eine Liste der freien speicherstücke verwaltet werden.
Wie lange dauert die durschnittliche Suche bei den drei beschriebenen
Strategien First-Fit, Next-Fit und Best-Fit?

### Lösung

First-Fit

* Worst case n weil man im schlechsten Fall alle durchgeht
* Best case 1 wenn man direkt einen Block findet
* Average O(n) vermutlich n/2

Next-Fit

* Fast genauso wie First-Fit
* Wenn man hintereinander Nutzung von Next-Fit betrachtet vermeidet man
  allerdings die gerade gemachten einträge zu durchsuchen
* Der Fall ist komplizierter wenn der Speicher bereits vorher belegt ist oder
  zwischen durch frei wird
* Somit ist es natrülich average case O(n) genauer ist es weniger als \<n/2

Best-Fit

* Mit der Annahme dass man kein perfekter Platz zu finden ist müsste man den gesamten Speicher durchgehen
* Somit Best case = Worst case = Average case = n
