// Datei KonsoleTst.cpp
/* ------------------------------------------------------------------------
Liest mit der Funktion GetImmediateAll() Zeichen von der Standardeingabe
(d.h. die Zeichen werden ohne Echo gelesen und ohne auf Return zu
warten) und gibt folgende Daten zur aktuellen Ausgabe aus:
- Das gelesene Zeichen (bzw. ein Blank, falls es nicht druckbar ist)
- Das gelesene Zeichen als Zahl (zwischen 0 und 255)
- Den (geraeteunabhaengigen) virtual key code des Zeichens
- Den control key status
- Die einzelnen Bits des control key status (Capslock_On, Enhanced_Key
  bis Shift_Pressed)
------------------------------------------------------------------------ */
// Dateiname    :
//  $Source: /CVS/EndlicherAutomat/KonsoleTst.cpp,v $
// CVS-User     :$Author: matze $
// Datum(commit):$Date: 2004/06/12 15:56:57 $
// Revision     :$Revision: 1.2 $
// Versionsmarke:$Name:  $
// ------------------------------------------------------------------------
#include "Konsole.h" // fuer ClearScreen, GetImmediateAny, GotoXY, ...
#include <iostream>
using namespace std;

int main() {
   ClearScreen();
   cout << "Programm KonsoleTst: Jetzt geht es los!" << endl;
   cout << "Bitte Tasten druecken (q zum Beenden):"  << endl;

   bool          alles_ok;
   char          zeichen_c  = 'x'; // Anfangswert ungleich 'q'
   unsigned char zeichen_uc;
   int           zeichen_i;
   short         vkc;              // virtual key code
   int           cks;              // control key status

   while (zeichen_c != 'q') {
      alles_ok   = GetImmediateAny(&zeichen_c, &vkc, &cks);
      zeichen_uc = static_cast<unsigned char>(zeichen_c);
      zeichen_i  = static_cast<int>(zeichen_uc);

      // Nicht-druckbare Zeichen durch Punkt '.' ersetzen:
      if (zeichen_uc < 32) zeichen_c = '.';

      // Informationen ueber die gedrueckte(n) Taste(n) ausgeben:
      GotoXY(0, 2);
      cout << "Zeichen als char  : '" << zeichen_c << "'" << endl;
      cout << "Zeichen als int   : ";
      cout.width(3);
      cout << zeichen_i << endl;
      cout << "VirtualKeyCode    : ";
      cout.width(3);
      cout << vkc << endl;;
      cout << "ControlKeyState   : ";
      cout.width(3);
      cout << cks << endl;
      cout << "Capslock_On       : " << Capslock_On       (cks) << endl;
      cout << "Enhanced_Key      : " << Enhanced_Key      (cks) << endl;
      cout << "Left_Alt_Pressed  : " << Left_Alt_Pressed  (cks) << endl;
      cout << "Left_Ctrl_Pressed : " << Left_Ctrl_Pressed (cks) << endl;
      cout << "Numlock_On        : " << Numlock_On        (cks) << endl;
      cout << "Right_Alt_Pressed : " << Right_Alt_Pressed (cks) << endl;
      cout << "Right_Ctrl_Pressed: " << Right_Ctrl_Pressed(cks) << endl;
      cout << "Scrolllock_On     : " << Scrolllock_On     (cks) << endl;
      cout << "Shift_Pressed     : " << Shift_Pressed     (cks) << endl;
   } // while

   ClearScreen(WHITE, BLACK);
   cout << "Programm KonsoleTst: Das war's erstmal!" << endl;
   return 0;
} // main
// ------------------------------------------------------------------------
