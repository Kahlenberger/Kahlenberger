// Datei Konsole.h
// ------------------------------------------------------------------------
// Dateiname    :
//  $Source: /CVS/EndlicherAutomat/Konsole.h,v $
// CVS-User     :$Author: matze $
// Datum(commit):$Date: 2004/06/12 15:56:57 $
// Revision     :$Revision: 1.2 $
// Versionsmarke:$Name:  $
/* ------------------------------------------------------------------------
Der Modul Konsole besteht aus den Dateien Konsole.h und Konsole.cpp.
Mit diesem Modul (und dem Gnu-Cygnus-C++-Compiler) kann man unter
Windows95/98/NT
- das Standardausgabefenster loeschen (ClearScreen)
- fuer das Standardausgabefenster Farben festlegen (SetColors)
- einzelne Zeichen und ganze Zeichenketten zu bestimmten Stellen des
  Standardausgabefensters ausgeben (Put)
- im Standardausgabefenster den Cursor positionieren (GotoXY)
- von der Standardeingabe einzelne Zeichen "sofort" und ohne Echo lesen,
  (d.h. der Benutzer muss nicht auf die Return-Taste druecken und das
  eingegebene Zeichen erscheint nicht auf dem Bildschirm (GetImmediate
  und GetImmediateAny).
- die Groesse des Standardausgabefensters veraendern
  (FenstergroesseMaximal und Fenstergroesse_80_25)
- zu einer Farbe zyklisch die nachfolgende Farbe bzw. die vorhergehende
  Farbe berechnen lassen (ColorSucc und ColorPred).
- sich den Namen einer Farbe (als C-String) liefern lassen (ColorName).

Die bool-Funktionen liefern true, wenn "alles geklappt hat". Sie liefern
false, wenn ein Fehler auftrat (z.B. weil die Parameter falsch waren).
------------------------------------------------------------------------ */
#ifndef Konsole_h
#define Konsole_h
//-------------------------------------------------------------------------
// Groesse des Standardausgabefensters:
short AnzZeilen();  // Liefert die Anzahl der Zeilen
short AnzSpalten(); // Liefert die Anzahl der Spalten
//-------------------------------------------------------------------------
// Eine Farbe ist eine 4-Bit-Binaerzahl, mit je einem Bit fuer RED, GREEN
// BLUE und BRIGHT. BRIGHT plus RED ist BRIGHT_RED, BLUE plus GREEN ist
// CYAN, BRIGHT plus BLUE plus GREEN ist BRIGHT_CYAN etc. Die Farbe
// bright black wird hier als GRAY bezeichnet:
enum Color {
   BLACK     = 0, BLUE          = 1, GREEN        = 2, CYAN        = 3,
   RED       = 4, MAGENTA       = 5, YELLOW       = 6, WHITE       = 7,
   GRAY      = 8, BRIGHT_BLUE   = 9, BRIGHT_GREEN =10, BRIGHT_CYAN =11,
   BRIGHT_RED=12, BRIGHT_MAGENTA=13, BRIGHT_YELLOW=14, BRIGHT_WHITE=15
};
//-------------------------------------------------------------------------
bool ClearScreen (const Color  vordergrund = BRIGHT_WHITE,
                  const Color  hintergrund = BLACK);
// Loescht den Bildschirm und schickt den Bildschirmzeiger (cursor) nach
// Hause (Spalte 0, Zeile 0). Wahlweise kann eine Farbe fuer den Vorder-
// grund und eine Farbe fuer den Hintergrund angegeben werden. Diese
// Farben gelten dann fuer alle Zellen des Standardausgabefensters und
// alle zukuenftigen Ausgaben zum Standardausgabefenster (bis zum naechsten
// Aufruf dieser Funktion ClearScreen oder einer SetColors-Funktion).
//-------------------------------------------------------------------------
bool SetColors      (const Color  vordergrund = BRIGHT_WHITE,
                     const Color  hintergrund = BLACK);
// Legt die Farben fuer *zukuenftige* Ausgaben zum Standardausgabefenster
// fest. Die Farben gelten nur fuer Ausgaben wie z.B. cout << "Hallo!";
// oder cout.put('X'); (Kopfdatei <iostream>), nicht aber fuer Ausgaben
// mit den Put-Funktionen, die in diesem Modul vereinbart werden (siehe
// unten). Die Farben bleiben wirksam bis zum naechsten Aufruf einer
// SetColors-Funktion oder der ClearScreen-Funktion.
//-------------------------------------------------------------------------
bool SetColors      (const short  spalte,
                     const short  zeile,
                     const Color  vordergrund = BRIGHT_WHITE,
                     const Color  hintergrund = BLACK);
// Legt die Hintergrundfarbe und die Vordergrundfarbe fuer *eine* Zeichen-
// Zelle des Standardausgabefensters fest (das Standardausgabefenster
// besteht aus AnzZeilen * AnzSpalten vielen Zeichen-Zellen).
//-------------------------------------------------------------------------
bool SetColors      (const short  von_spalte,
                     const short  bis_spalte,
                     const short  von_zeile,
                     const short  bis_zeile,
                     const Color  vordergrund = BRIGHT_WHITE,
                     const Color  hintergrund = BLACK);
// Legt die Hintergrundfarbe und die Vordergrundfarbe fuer ein Rechteck
// auf dem Bildschirm fest. Das Rechteck kann aus beliebig vielen
// Zeichen-Zellen bestehen (maximal aus allen Zellen des Bildschirms).
//-------------------------------------------------------------------------
bool Put            (const short  spalte,
                     const short  zeile,
                     const char  *text);
// Gibt den text zum Standardausgabefenster aus. Das erste Zeichen des
// Textes wird in die Zelle (spalte, zeile) geschrieben.
// ACHTUNG: Die Position des Bildschirmzeigers (cursor) wird durch dieses
// Unterprogramm *nicht* veraendert!
//-------------------------------------------------------------------------
bool Put            (const short  spalte,
                     const short  zeile,
                     const char   zeichen);
// Gibt das zeichen zum Standardausgabefenster aus, an die Position (spalte,
// zeile).
// ACHTUNG: Die Position des Bildschirmzeigers (cursor) wird durch dieses
// Unterprogramm *nicht* veraendert!
//-------------------------------------------------------------------------
bool GotoXY         (const short  spalte,
                     const short  zeile);
// Bringt den Bildschirmzeiger im Standardausgabefenster an die Position
// (spalte, zeile). Nachfolgende Ausgaben (z.B. cout << "Hallo!";) erfolgen
// ab dieser neuen Position.
//-------------------------------------------------------------------------
bool GetImmediateAny(char  *zeichen,
                     short *psVirtualKeyCode,
                     int   *piControlKeyState);
// Liest ein Zeichen von der Standardeingabe (Tastatur). Der eingebende
// Benutzer muss seine Eingabe *nicht* mit der Return-Taste abschliessen.
// Die Eingabe erfolgt *ohne Echo* (d.h. das eingegebene Zeichen erscheint
// nicht automatisch auf dem Bildschirm). Mit diesem Unterprogramm kann
// *jeder* Tastendruck ("any key") gelesen werden (auch ein Druck auf eine
// Taste wie Pfeil-nach-oben, Bild-nach-unten, Umschalt-Taste, linke
// Alt-Taste, Rechte Alt-Taste, linke Strg-Taste, rechte Strg-Taste oder
// auf eine Funktionstaste F1, F2 etc. etc.). Die folgenden Funktionen
// sollen das Analysieren des Parameters *piControlKeyState erleichtern.
// ------------------------------------------------------------------------
// Funktionen zur Ananlyse des Parameters *piControlKeyState der
// Funktion GetImmediateAny:
bool Capslock_On       (int iControlKeyState);
bool Enhanced_Key      (int iControlKeyState);
bool Left_Alt_Pressed  (int iControlKeyState);
bool Left_Ctrl_Pressed (int iControlKeyState);
bool Numlock_On        (int iControlKeyState);
bool Right_Alt_Pressed (int iControlKeyState);
bool Right_Ctrl_Pressed(int iControlKeyState);
bool Scrolllock_On     (int iControlKeyState);
bool Shift_Pressed     (int iControlKeyState);
//-------------------------------------------------------------------------
bool GetImmediate(char  *zeichen);
// Liest ein Zeichen von der Standardeingabe (Tastatur). Der eingebende
// Benutzer muss seine Eingabe *nicht* mit der Return-Taste abschliessen.
// Die Eingabe erfolgt *ohne Echo* (d.h. das eingegebene Zeichen erscheint
// nicht automatisch auf dem Bildschirm). Mit diesem Unterprogramm koennen
// nur "normale Zeichen" (Buchstaben, Ziffern, Sonderzeichen etc.) einge-
// lesen werden. Steuerzeichen (z.B. Pfeil-nach-oben, Bild-nach-unten etc.)
// werden vom Betriebssystem "verschluckt".
//-------------------------------------------------------------------------
bool FenstergroesseMaximal();
// Vergroessert das Standardausgabefenster auf die maximale Groesse (die
// von der Groesse des Bildschirms und dem verwendeten Font abhaengt).
//-------------------------------------------------------------------------
bool Fenstergroesse_80_25();
// Veraendert die Groesse des Standardausgabefensters so, dass es 25 Zeilen
// mit je 80 Spalten enthaelt.
//-------------------------------------------------------------------------
Color ColorSucc(const Color c);
// Zyklische Nachfolgerfunktion fuer Color (nach BRIGHT_WHITE kommt BLACK)
Color ColorPred(const Color c);
// Zyklische Vorgaengerfunktion fuer Color (vor BLACK liegt BRIGHT_WHITE)
const char *ColorName(const Color c);
// Liefert den Namen der Farbe c als String.
//-------------------------------------------------------------------------
#endif // Konsole_h
