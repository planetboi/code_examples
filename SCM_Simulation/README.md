# Verteilte Systeme in Wintersemester 2021/22
**Projekt:** Anwendung aus dem Bereich des Logistik bzw. Supply Chain Monitoring

**Autoren:** Anastasia Galeyev, Cheryl Yamoah

# Anforderungsanalyse

## Ziel des zu entwickelnden Systems

Im Rahmen des Praktikums Verteilte Systeme soll eine Anwendung aus dem Bereich des Logistik bzw. Supply Chain Monitoring entwickelt werden. Dazu sollen die Technologien Sockets, Remote Procedure Calls (RPCs) sowie Message Oriented-Middleware (MOM) verwendet werden.

## Anforderungen

| Funktionale Anforderungen | Nichtfunktionale Anforderungen |
| ------ | ------ |
|**Termin 2** <br />1. Sensoren erfassen Information von Ein- und Auslieferungen<br /> 2. Sensoren erfassen Information von Inventarisierungen (RFID-Leser oder Barcode-Leser)<br /> 3. Produktanzahl ändert sich korrekt mit jeder Ein-, Auslieferung oder Inventarisierung<br /> 4. Sensoren leiten Information an Lager mittels UDP weiter<br /> 5. Nachrichten vom Sensor werden auf der Standardausgabe (IP, Port, Sensor-Typ) ausgegeben<br /> 6. Nachrichten von Sensoren werden in einer Log-Datei festhalten |**Termin 2** <br />1. Information, die von Sensoren erfasst wird = Information, welche vom Lager ausgegeben und gespeichert wird → Test durch Vergleich<br /> 2. 1 simulierte Stunde = 4 Echtzeit Sekunden, 1 simulierter Tag = 96 Echtzeit Sekunden -> Messen (z.B durch Standardausgaben)<br /> 3.In Log-Datei wird die Information mit einem Datum/einem Zeitpunkt gespeichert |
|**Termin 3**<br />1. HTTP-Server liest HTTP-GET Anfragen korrekt und vollständig ein und verarbeitet diese<br />2. HTTP-Server kann auf Sensordaten zugreifen<br />3. Einzelne Sensordaten können aufgerufen werden (mit eigener URL)<br />4. Historie der Sensordaten kann aufgerufen werden (mit eigener URL)<br />5. Zentrale kommuniziert gleichzeitig mit Sensoren und HTTP-Clients |**Termin 3**<br />1. URL-Format für einzelnen Sensor: /Sensor-ID/index<br />2. URL-Format für Historie: /Sensor-ID/all<br />3. Wenn die Sensor-ID oder der URL nicht existiert wird eine Hilfeseite aufgerufen<br />4. Chrome und Firefox Browser werden unterstützt |
|**Termin 4**<br />1. Lager übermitteln aktuelle Sensordaten über thrift an andere Lager<br />2. Ein Lager kann Produktbestand eines anderen Lagers abfragen<br />3. Es können Lieferungen gemacht werden |**Termin 4**<br />1. Die gelieferten Lagerdaten stimmen mit den eigentlichen Lagerdaten überein<br />2. Wenn eine Lieferung beantragt wird, erfasst der Sensor dies und die Produktanzahl wird angepasst<br />3. Wenn eine Lieferung nicht möglich ist, da z.B. ein Produkt nicht mehr vorhanden ist, wird dies mitgeteilt<br />4. Jedes Lager speichert die Daten persistent →  Datenbank/Log-Datei (?) |
|**Termin 5**<br />1. Lager können andere Lager abonnieren<br />2. Lager können die Produkte anderer Lager einsehen<br />3. Lager können ihre Daten mit MQTT veröffentlichen und übermitteln |**Termin 5**<br />1. Lagerdaten = Zusammenfassung der Sensordaten<br />2. Daten werden mit MQTT veröffentlicht und übertragen<br />3. Wichtige Daten: Welche Daten sind nicht mehr im Bestand ? |

## Randbedingungen
- Verwendete Programmiersprache: C++
- Verwendetes Kommunikationsprotokoll: Apache Thrift
- Verwendeter MQTT Broker: Mosquitto
- Alle Komponenten können in Docker gestartet werden (Make File) und laufen als eigenständige Container 

## Änderungsübersicht
| Version | Datum | Bearbeiter/in | Beschreibung |
| ---- | ---- | ---- | ---- |
| _Eindeutige Versionsnummer_ | _Datum, an dem die Änderung vollständig abgeschlossen wurde_ | _Mitarbeiter/in, welche/r die Änderung durchgeführt hat_ | _Allgemeine Beschreibung was geändert wurde_ |
|2.0 |08.11.2021 |Anastasia Galeyev |Sensor-Funktion infoErfassen() wurde verworfen, stattdessen wurde ein Vektor mit allen Produktinfos implementiert.<br />Sensor-Funktionen infoSenden(int), nachrichtAusgabe(string), nachrichtSpeichern(string) wurden verworfen, da diese überflüssig sind.
|2.1 |08.11.2021 |Cheryl Yamoah |Lager-Funktion datenSenden() sind überflüssig, da das Lager keine Nachrichten an den Sensor sendet.<br />Funktionen processSensorInfo(), printSensorInfo() wurden in Lager ergänzt.<br />Funktion udp() wurde im Lager implementiert, um Nachrichten vom Sensor zu erhalten.
|3.0 |22.11.2021 |Cheryl Yamoah |HTTP-Server wurde implementiert, welcher HTTP-GET Befehle verarbeiten kann.
|4.0 |12.01.2022 |Cheryl Yamoah & <br />Anastasia Galeyev |Puffer-Termin verwendet. <br />-> Verschiebung der Aufgaben
|4.1 |12.01.2022 |Anastasia Galeyev | Methode orderProduct(), um bestellen zu können wurde implementiert. <br />Methode addStock() um den Lagerinhalt zu aktualisieren wurde implementiert
|4.2 |12.01.2022 |Cheryl Yamoah |Aufsetzen einer Thrift-File und einer Shell-File. <br />Methode seeProducts(), um den Lagerinhalt anzugucken wurde implementiert. <br/>Methode saveStock(), um den Lagerinhalt zu speichern wurde implementiert.
|5.0 |25.01.2022 |Anastasia Galeyev | Implementierung von dem MQTT-Subscriber.
|5.1 |25.01.2022 |Cheryl Yamoah | Implementierung von dem MQTT-Publisher.
|5.2 |26.01.2022 |Cheryl Yamoah & <br />Anastasia Galeyev | Implementierung von Tests.|


