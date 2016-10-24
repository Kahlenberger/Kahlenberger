// Gruppe: 2in1, Mitglieder: Dennis Zeuner, Matthias Kahlenberger.
// Aufgabe 10: Kalk01, PRG3-TI, SS04
// Datei EndlicherAutomat.cpp
// ------------------------------------------------------------------------
// Dateiname    :
//  $Source: /CVS/EndlicherAutomat/EndlicherAutomat.cpp,v $
// CVS-User     :$Author: dd6zc $
// Datum(commit):$Date: 2016/10/24 00:50:00 $
// Revision     :$Revision: 1.10 $
// Versionsmarke:$Name:  $
// ------------------------------------------------------------------------
// Ein endlicher Automat, der Berechnungen aehnlich einem Tischkalkulator 
// durchfuehrt. Der Automat zeigt jeder Zeit eine korreckte Berechnung an.
// Die Eingabe von 'm' wechselt das Vorzeichen des aktuellen Operanden.
// Die Taste 'c' setzt alle Eingaben auf 0 zurueck, die Eingabe eines
// Operators ('*', '+', '-' oder '/') schaltet zum zweiten Operanden 
// weiter, mit Return kann ebenfalls zwischen den Operanden gewechselt 
// werden (ohne einen Operator eingeben zu muessen). 
// ------------------------------------------------------------------------
// Das Symbol <TEST_EA> muss einkommentiert sein, falls Du detailiertere
// Information zur Laufzeit benötigst. (Tests - Debugging)
//#define TEST_EA
   #ifdef TEST_EA
         // Eigenes Symbol fuer die Compiler-Version
         #define  COMPILER_VERSION __VERSION__
         // ... Und dieses Symbol musst Du einkommentieren, wenn
         // Du Informationen zur Fehlersuche benötigst.
         #define DEBUG_EA
   #endif
 #define DEBUG_EA_PREFIX "--[debug] "
// ------------------------------------------------------------------------
// Ein paar nuetzliche Symbole...
#define OS_NORMAL_RC   0
#define OS_FAIL_RC   255
// ------------------------------------------------------------------------
#include <iostream>
#include <utility>   // fuer die Klassenschablone pair
#include <iomanip>
//#include "Konsole.h" // GetImmediateAny(), ...
using namespace std;
// ------------------------------------------------------------------------
// Globale Variablen fuer die Ausgabe auf der Konsole
// ...declaration here, is unused (gcc -Wunused-var...) ... short  const MAX_SPALTE  = AnzSpalten()-1;
short  const AUS_ZEILE1  = 6;
short  const AUS_ZEILE2  = 8;
short  const AUS_ZEILE3  = 9;
short  const AUS_ZEILE4  = 10;
short  const AUS_ZEILE5  = 11;
short  const ZEILE_DEBUG = 14;
short  const ZEILE_START_MELDUNG = 2;
//string const BLANKS(AnzSpalten(), ' ');
// ------------------------------------------------------------------------
// Die Zustaende und die "abstrakten Eingaben" des endlichen Automaten:
enum Zustand {Z0, Z1, Z2, Z3, Z4};
enum Eingabe {ZIF, VORZ, OPER, RET, CLR, QUIT};
// Vom Typ var wird nur eine einzige Variable definiert:
struct var {
   Zustand akt_z;    // Der aktuelle Zustand des endlichen Automaten
   Eingabe akt_e;    // Die aktuelle "abstrakte Eingabe"
   char    letztes;  // Die aktuelle "konkrete  Eingabe"
   int     operand1; // Der erste Operand der Berechnung
   int     operand2; // ... der Zweite
   int     ergebnis; // und das Ergebnis der Berechnung
   char    operation;// Das math. Operationszeichnen
};
// Eine Aktion ist ein Zeiger auf eine void-Funktion mit einem Parameter
// vom Typ var:
typedef void (* Aktion)(var &v);
// Ein AZ_Paar besteht aus einer Aktion und einem Zustand:
typedef pair<Aktion, Zustand> AZ_Paar;
// ------------------------------------------------------------------------
// Funktionsdeklarationen der "dienstbaren" Funktionen
void initTTY();         // bereitet die Konsole auf die Ausgabe vor
void display(var &v);        // gibt die Berechnung auf dem Bildschirm aus
void compute(var &v);      // fuehrt die eigentliche Berechnnung durch
// [TODO]
// ...remove this bad thingsss, please!
// gibt eine Meldung an den Programmierer aus
void debug(int zeilenNr, string message);
// gibt ein Zeichen an den Programmierer aus
void debug(int zeilenNr, char   zeichen);
// [TODO-END]
// ------------------------------------------------------------------------
// Die einzelnen Aktionen des endlichen Automaten:
//  Hinweis: Einige Aktionen fuehren keine Anweisungen aus,
//  diese Funktionen hätten alle a0 sein können. Für eine
//  spätere Verfeinerung können hier zusaetzl. Funktionen
//  implementiert werden.
void a0(var &v) {
}
void a1(var &v) {
   v.operand1 =((int) v.letztes) - '0';
}
void a2(var &v) {
   v.operand1 = v.operand1 * 10 + (v.letztes - '0');
}

void a3(var &v) {
   v.operand1 *= -1;
}
void a4(var &v) {
   v.operand2 =((int) v.letztes) - '0';
}
void a5(var &v) {
   v.operand2 = v.operand2 * 10 + (v.letztes - '0');
}
void a6(var &v) {
   v.operand2 *= -1;
}
void a7(var &v) {
   v.operation = v.letztes;
}
void a8(var &v) {
}
void a9(var &v) {
   v.operand1  = 0;
   v.operation = '+';
   v.operand2  = 0;
   v.ergebnis  = 0;
}
void a10(var &v) {
}

// ------------------------------------------------------------------------
// Die Funktion next liest "konkrete Eingabezeichen" und uebersetzt sie
// in die "abstrakten Eingaben" des endlichen Automaten:
void next(var &v) {
   while (true) {        // Diese Schleife wird mit return verlassen
      short psVirtualKeyCode;
   //   v.letztes = '13'; 
      // Die Variable piControlKeyState_dummy wird von GetImmediateAny()
      // als Parameter benötigt - sie wird hier aber nicht ausgewertet!
      // ...int   piControlKeyState_dummy;
      string helpMeVariabli;
      helpMeVariabli = helpMeVariabli + 'q'; 
      // input
      cin >>  helpMeVariabli ; // >> flush; 
      v.letztes = helpMeVariabli.at(0); // >> flush; 

//	GetImmediateAny(&v.letztes,
//        &psVirtualKeyCode, &piControlKeyState_dummy);
      #ifdef DEBUG_EA
         debug(2, "Folgendes Zeichen wurde eingelesen-->");
         debug(3, v.letztes);
      #endif
      switch (v.letztes) {
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
                   {v.akt_e = ZIF;     return;}
         case 'm': {v.akt_e = VORZ;    return;}
         case 'c': {v.akt_e = CLR;     return;}
         case '+': case '-': case '*': case '/':
                   {v.akt_e = OPER;    return;}
         case 'q': case 'Q':
                   {v.akt_e = QUIT;    return;}
         default : ;;
       } // switch (v.letztes)
       switch(psVirtualKeyCode){
         case 13 : {v.akt_e = RET;     return;}
         default : ;;
      } // switch (psVirtualKeyCode)
   } // while (true)
} // next()
// ------------------------------------------------------------------------
int main() {
   initTTY(); // Die Ausgabe initialisieren
   // ---------------------------------------------------------------------
   // Die Aktions-Folgezustands-Tabelle des endlichen Automaten:
   AZ_Paar tab[5][6]; // 5 Zustaende und 6 (abstrakte) Eingaben

   // Die Reihung tab wird nur der besseren Lesbarkeit wegen durch einzelne
   // Zuweisungen initialisiert (und nicht durch eine Komponentenliste bei
   // bei der Vereinbarung):
   tab[Z0][ZIF]      = AZ_Paar(a1, Z1);
   tab[Z0][VORZ]     = AZ_Paar(a3, Z0);
   tab[Z0][OPER]     = AZ_Paar(a7, Z2);
   tab[Z0][RET]      = AZ_Paar(a10,Z2);
   tab[Z0][CLR]      = AZ_Paar(a9, Z0);
   tab[Z0][QUIT]     = AZ_Paar(a0, Z4);

   tab[Z1][ZIF]      = AZ_Paar(a2, Z1);
   tab[Z1][VORZ]     = AZ_Paar(a3, Z1);
   tab[Z1][OPER]     = AZ_Paar(a7, Z2);
   tab[Z1][RET]      = AZ_Paar(a10,Z2);
   tab[Z1][CLR]      = AZ_Paar(a9, Z0);
   tab[Z1][QUIT]     = AZ_Paar(a0, Z4);

   tab[Z2][ZIF]      = AZ_Paar(a4, Z3);
   tab[Z2][VORZ]     = AZ_Paar(a6, Z2);
   tab[Z2][OPER]     = AZ_Paar(a7, Z2);
   tab[Z2][RET]      = AZ_Paar(a8, Z0);
   tab[Z2][CLR]      = AZ_Paar(a9, Z0);
   tab[Z2][QUIT]     = AZ_Paar(a0, Z4);

   tab[Z3][ZIF]      = AZ_Paar(a5, Z3);
   tab[Z3][VORZ]     = AZ_Paar(a6, Z3);
   tab[Z3][OPER]     = AZ_Paar(a7, Z3);
   tab[Z3][RET]      = AZ_Paar(a8, Z0);
   tab[Z3][CLR]      = AZ_Paar(a9, Z0);
   tab[Z3][QUIT]     = AZ_Paar(a0, Z4);

   // Diese Zeile der Tabelle ist nur aus
   // Vollstaendigkeitsgruenden vorhanden.
   // Bevor hier herraus eine Aktion gewaehlt wird,
   // bricht die while()-Schleife (siehe unten) ab.
   tab[Z4][ZIF]      = AZ_Paar(a0, Z4);
   tab[Z4][VORZ]     = AZ_Paar(a0, Z4);
   tab[Z4][OPER]     = AZ_Paar(a0, Z4);
   tab[Z4][RET]      = AZ_Paar(a0, Z4);
   tab[Z4][CLR]      = AZ_Paar(a0, Z4);
   tab[Z4][QUIT]     = AZ_Paar(a0, Z4);
   // ---------------------------------------------------------------------
   // Alle Variablen (ausser tab) sind Komponenten des Verbundes v:
   var v;                     // Einzige Variable vom Typ var
   v.akt_z     = Z0;          // Z0 ist der Anfangszustand
   v.operand1  = 0;           // Diese Initialisierungen sind notwendig,
   v.operand2  = 0;           // da sich die Aktionen darauf verlassen
   v.operation = '+';         // das min. diese vier Komp. initial. sind
   v.ergebnis  = 0;

   // Das "Treiberprogramm" fuer den endlichen Automaten:
   while (v.akt_z != Z4) {                    // Z4 ist der Endzustand
      next(v);                                // Naechstes Zeichen lesen
      tab[v.akt_z][v.akt_e].first(v);         // Entsprechende Aktion ausfuehren
      v.akt_z = tab[v.akt_z][v.akt_e].second; // In den Folgezustand uebergehen
      #ifdef DEBUG_EA
         debug(0, "Neuer Zustand:");
         debug(1, (char) v.akt_z + '0');
      #endif
      compute(v);    // Eine Berechnung durchfuehren
      display(v);    // Die Ausgabe "auffrischen"
   } // while
// ClearScreen(); // Sauberkeit muss sein... :-)
   return OS_NORMAL_RC;
} // main()
// -----------------------------------------------------------------------
// Die folgenden Funktionen gehören konzeptionell nicht zum
// Endlichen Automaten.
void initTTY(){
   /* Diese Funktion initialisiert den Ausgabebildschirm -
    *  sie bereitet die Ausgabe vor.
    */
//   ClearScreen();
   #ifdef COMPILER_VERSION
//      GotoXY(10, ZEILE_DEBUG);
      // Ich sage nur: GCC Version 2.96!!! ;-)
//      debug(0, "Version des Compilers: ");
//      debug(1, COMPILER_VERSION);
   #endif
   // Begruessungmeldung ausgeben
//   GotoXY(8,ZEILE_START_MELDUNG);
//   ...ohne             cout << "Willkommen beim Tischkalkulator" << flush;
// ...mit \n sequenz   cout << "Willkommen beim Tischkalkulator" << endl; // flush;
     cout << "Willkommen beim Tischkalkulator" << endl; // flush;
//   cout.setf(ios::right);
   // Den initialen Zustand ausgeben
//   GotoXY(10,AUS_ZEILE2); cout << setw(10) << "0" << flush;
//   GotoXY(10,AUS_ZEILE3); cout << setw(10) << '+' << flush;
//   GotoXY(10,AUS_ZEILE4); cout << setw(10) << "0" << flush;
//   GotoXY(10,AUS_ZEILE5); cout << setw(10) << "0" << flush;
}
void display(var &v) {
   cout << v.ergebnis << flush;	
   /* Diese Funktion ist fuer die Ausgabe der Berechnung zuständig
    * Nutze sie, wenn sich die Operanden, die Operation
    * oder das Ergebnis des Endlichen Automaten geaendert haben.
    * (siehe die Struktur "struct var")
    */
/**
  *
   // Ausgabe-Zeilen loeschen
   GotoXY(0,AUS_ZEILE2); cout<< BLANKS << flush;
   GotoXY(0,AUS_ZEILE3); cout<< BLANKS << flush;
   GotoXY(0,AUS_ZEILE4); cout<< BLANKS << flush;
   GotoXY(0,AUS_ZEILE5); cout<< BLANKS << flush;

   cout.setf(ios::right);
   // jetzt, die eigentliche Ausgabe...
   GotoXY(10,AUS_ZEILE2); cout << setw(10) << v. operand1 << flush;
   GotoXY(10,AUS_ZEILE3); cout << v.operation << flush;
   GotoXY(10,AUS_ZEILE4); cout << setw(10) << v. operand2 << flush;
   GotoXY(10,AUS_ZEILE5); cout << setw(10) << v. ergebnis << flush;
   GotoXY(AnzSpalten() - 1, AnzZeilen() - 1);
  *
  */

} // display()
void compute(var &v) {
   /* Diese Funktion berechnet die in der Stuktur v
    * enthaltene Rechnung.
    */
   #ifdef DEBUG_EA
      if(v.operation == '/' && v.operand2 == 0)
         debug(-10, "WARNUNG: Berechnung ungültig! (Div. by zero)");
      else
         debug(-10, "                                            ");
   #endif
   switch(v.operation){
         case '+':
         {v.ergebnis = v.operand1 + v.operand2; break;}
         case '-':
         {v.ergebnis = v.operand1 - v.operand2; break;}
         case '*':
         {v.ergebnis = v.operand1 * v.operand2; break;}
         case '/':
         {v.ergebnis = v.operand1 / ((v.operand2 == 0) ? 1 : v.operand2);
            break;}
         default : ;;
   } // switch v.operation
} // compute()
// -----------------------------------------------------------------------
// Die folgenden Funktionen sind fuer die Wartung und Tests noetig,
// Sie sollten noch Möglichkeit nur in "#ifdef DEBUG_XX"
// bzw. "#ifdef TEST_XX"-Direktiven eingeschlossen sein.
// Ziel --> "bedingte Kompilierung"
// TO-DO: Einen eigenen Ausgabeoperator für Debug-
//        bzw. Test-Meldungen
#ifdef TEST_EA
void debug(int zeilenNr, string message) {
//   GotoXY(5, ZEILE_DEBUG + zeilenNr);
//   cout << BLANKS << flush;
//   GotoXY(5, ZEILE_DEBUG + zeilenNr);
//   cout << DEBUG_EA_PREFIX << message << flush;
//   GotoXY(AnzSpalten() - 1, AnzZeilen() - 1);
}
void debug(int zeilenNr, char zeichen) {
//   GotoXY(5, ZEILE_DEBUG + zeilenNr);
//   cout << BLANKS << flush;
//   GotoXY(5, ZEILE_DEBUG + zeilenNr);
//   cout << DEBUG_EA_PREFIX << zeichen << flush;
//   GotoXY(AnzSpalten() - 1, AnzZeilen() - 1);
}
#endif

