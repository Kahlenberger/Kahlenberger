// Datei Konsole.cpp
//  Datei Konsole.cpp
// ------------------------------------------------------------------------
// Implementierungsteil des Moduls Konsole
// ------------------------------------------------------------------------
// Dateiname    :
//  $Source: /CVS/EndlicherAutomat/Konsole.cpp,v $
// CVS-User     :$Author: dd6zc $
// Datum(commit):$Date: 2016/10/24 00:50:00 $
// Revision     :$Revision: 1.5 $
// Versionsmarke:$Name:  $
// ------------------------------------------------------------------------
#include "Konsole.h"
//#include <windows.h>
#include <iostream>
#include <cstring>  // fuer strlen

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hStdin  = GetStdHandle(STD_INPUT_HANDLE);
//-------------------------------------------------------------------------
short AnzZeilen() {
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   GetConsoleScreenBufferInfo(hStdout, &csbi);
   return csbi.dwSize.Y;
} // AnzZeilen;
//-------------------------------------------------------------------------
short AnzSpalten() {
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   GetConsoleScreenBufferInfo(hStdout, &csbi);
   return csbi.dwSize.X;
} // AnzSpalten;
//-------------------------------------------------------------------------
bool ClearScreen(const Color vordergrund,
                 const Color hintergrund) {
   // 1. Fuer alle Zellen des Bildschirms werden Vordergrund- und Hinter-
   //    grundfarbe neu festgelegt.
   // 2. Die auf dem Bildschirm angezeigten Zeichen werden geloescht.
   // 3. Der Bildschirmzeiger (cursor) wird nach Hause geschickt (Zeile 0,
   //    Spalte 0).
   // 4. Die Farben fuer nachfolgende Ausgaben (zur Standardausgabe)
   //    werden festgelegt (entsprechend den Parametern vordergrund
   //    und hintergrund). Dies ist notwendig, um bestimmte kleine Unter-
   //    schiede zwischen Windows95 und WindowsNT auszugleichen.

         bool  ok = true;
   const DWORD SOLL_ANZAHL  = AnzZeilen() * AnzSpalten();
         DWORD ist_Anzahl; // Anz. der tatsaechlich gefuellten Zellen
         COORD ursprung = {0, 0};
         WORD  farben   = vordergrund + 16 * hintergrund;
   const char  BLANK    = ' ';

   // Farbe aller Zellen neu festlegen:
   ok &= FillConsoleOutputAttribute(hStdout, farben, SOLL_ANZAHL,
      ursprung, &ist_Anzahl);

   // Die auf dem Bildschirm angezeigten Zeichen loeschen:
   ok &= FillConsoleOutputCharacter(hStdout, BLANK, SOLL_ANZAHL,
      ursprung, &ist_Anzahl);
   if (!ok) return false;

   // Farben fuer nachfolgende Ausgaben festlegen:
   ok &= SetConsoleTextAttribute(hStdout, farben);

   // Bildschirmzeiger nach Hause schicken und Ergebnis liefern:
   ok &= GotoXY(0, 0);

   return ok;
} // ClearScreen
//-------------------------------------------------------------------------
bool SetColors   (const Color  vordergrund,
                  const Color  hintergrund) {
   WORD farben = vordergrund + 16 * hintergrund;
   return SetConsoleTextAttribute(hStdout, farben);
} // SetColors
//-------------------------------------------------------------------------
bool SetColors(const short spalte,
               const short zeile,
               const Color vordergrund,
               const Color hintergrund) {
   // Die Farben einer einzelnen Zelle des Bildschirms werden festgelegt:
   bool ok;

   // Die Parameter spalte und zeile pruefen:
   if ((spalte < 0) || (AnzSpalten() <= spalte)) return false;
   if ((zeile  < 0) || (AnzZeilen()  <= zeile))  return false;

   COORD   c = {spalte, zeile};
   DWORD   anz;
   WORD    farben = vordergrund + 16 * hintergrund;
   ok = SetConsoleTextAttribute(hStdout, farben);
   if (!ok) return false;

   return WriteConsoleOutputAttribute(hStdout, &farben, 1, c, &anz);
} // SetColors
//-------------------------------------------------------------------------
bool SetColors   (const short  von_spalte,
                  const short  bis_spalte,
                  const short  von_zeile,
                  const short  bis_zeile,
                  const Color  vordergrund,
                  const Color  hintergrund) {
   // Die Farben eines ganzen Rechtecks auf dem Bildschirm werden
   // festgelegt:
   bool ok = true;

   // Die Parameter von_spalte, ..., bis_zeile pruefen:
   if ((von_spalte < 0) || (AnzSpalten() <= von_spalte)) return false;
   if ((bis_spalte < 0) || (AnzSpalten() <= bis_spalte)) return false;
   if ((von_zeile  < 0) || (AnzZeilen()  <= von_zeile )) return false;
   if ((bis_zeile  < 0) || (AnzZeilen()  <= bis_zeile )) return false;

   COORD c;
   DWORD anz;
   WORD  farben = vordergrund + 16 * hintergrund;

   ok &= SetConsoleTextAttribute(hStdout, farben);

   for (c.Y = von_zeile; c.Y <= bis_zeile; c.Y++) {
      for (c.X = von_spalte; c.X <= bis_spalte; c.X++) {
         ok &= WriteConsoleOutputAttribute(hStdout, &farben, 1, c, &anz);
      }
   }
   return ok;
} // SetColors
//-------------------------------------------------------------------------
bool Put(const short spalte, const short zeile, const char *text) {
   const DWORD laenge = strlen(text);
   const COORD c      = {spalte, zeile};
         DWORD anz;

   return WriteConsoleOutputCharacter(hStdout, text, laenge, c, &anz);
} // Put
//-------------------------------------------------------------------------
bool Put(const short spalte, const short zeile, const char zeichen) {
   char text[2] = {zeichen, '\0'};
   const DWORD laenge = 1;
   const COORD c      = {spalte, zeile};
         DWORD anz;

   return WriteConsoleOutputCharacter(hStdout, text, laenge, c, &anz);
} // Put
//-------------------------------------------------------------------------
bool GetImmediate(char * zeichen) {
   DWORD   modus_alt;
   DWORD   modus_neu = 0;
   DWORD   anzahl_gelesen;
   // WINBOOL ok;
   bool ok;
   const char CR  = 13;
   const char LF  = 10;
   const char NUL =  0;

   // Alten Modus der Konsole holen:
   if (!GetConsoleMode(hStdin, &modus_alt)) {
      return false;
   }
   // Neuen Modus (ohne Echo und ohne Warten auf Return) setzen:
   if (!SetConsoleMode(hStdin, modus_neu)) {
      return false;
   }
   // Ein Zeichen lesen (ohne Echo und ohne Warten auf Return).
   // Die Zeichen CR, LF und NUL werden "ueberlesen":
   do {
      ok = ReadConsoleA(hStdin, zeichen, 1, &anzahl_gelesen, NULL);
   } while (ok && (*zeichen == CR || *zeichen == LF || *zeichen == NUL));

   // Egal ob ReadConsoleA geklappt hat oder nicht: Den alten
   // Modus der Konsole wieder herstellen (hoffentlich klappt das):
   SetConsoleMode(hStdin, modus_alt); // Alten Modus wiederherstellen

   return ok;
} // GetImmediate
//-------------------------------------------------------------------------
bool GetImmediateAny(char  *z, short *vkc, int *cks) {
   INPUT_RECORD ip;
   //WINBOOL      ok;
   DWORD        anzGelesen;
   bool         ok;

   while (true) {                   // Schleife wird mit return verlassen
      ok = ReadConsoleInput(hStdin, &ip, 1, &anzGelesen);
      // Alle Ereignisse (events) ueberlesen ausser KeyDown-Ereignisse:
      if (ip.EventType != KEY_EVENT || !ip.Event.KeyEvent.bKeyDown) {
         continue;
      }
      *vkc = static_cast<short>(ip.Event.KeyEvent.wVirtualKeyCode);
      *cks = static_cast<short>(ip.Event.KeyEvent.dwControlKeyState);
      *z   =                    ip.Event.KeyEvent.uChar.AsciiChar;
      return ok;
   } // while
}// GetImmediateAny
//-------------------------------------------------------------------------
// CAPSLOCK_ON, ENHANCED_KEY, ... SHIFT_PRESSED sind Makros (oder: #define-
// Konstanten, wie altmodisch), die in der Datei Defines.h definiert sind.
// Die Werte dieser Konstanten sind im folgenden als Kommentar angegeben.
// "cks" soll an "Control Key State" erinnern:
bool Capslock_On       (int cks) {return cks & CAPSLOCK_ON;}        // 128
bool Enhanced_Key      (int cks) {return cks & ENHANCED_KEY;}       // 256
bool Left_Alt_Pressed  (int cks) {return cks & LEFT_ALT_PRESSED;}   //   2
bool Left_Ctrl_Pressed (int cks) {return cks & LEFT_CTRL_PRESSED;}  //   8
bool Numlock_On        (int cks) {return cks & NUMLOCK_ON;}         //  32
bool Right_Alt_Pressed (int cks) {return cks & RIGHT_ALT_PRESSED;}  //   1
bool Right_Ctrl_Pressed(int cks) {return cks & RIGHT_CTRL_PRESSED;} //   4
bool Scrolllock_On     (int cks) {return cks & SCROLLLOCK_ON;}      //  64
bool Shift_Pressed     (int cks) {return cks & SHIFT_PRESSED;}      //  16
//-------------------------------------------------------------------------
bool GotoXY(const short spalte, const short zeile) {
   const COORD c = {spalte, zeile};
   return SetConsoleCursorPosition(hStdout, c);
} // GotoXY
//-------------------------------------------------------------------------
bool FenstergroesseMaximal() {
   COORD   max = GetLargestConsoleWindowSize(hStdout);
   return SetConsoleScreenBufferSize(hStdout, max);
} // FenstergroesseMaximal
//-------------------------------------------------------------------------
bool Fenstergroesse_80_25() {
   COORD   std = {80, 25};
   return SetConsoleScreenBufferSize(hStdout, std);
} // Fenstergroesse_80_25
//-------------------------------------------------------------------------
Color ColorSucc(const Color c) {
   return (c==BRIGHT_WHITE ? BLACK : static_cast<Color>(c + 1));
} // ColorSucc
//-------------------------------------------------------------------------
Color ColorPred(const Color c) {
   return (c==BLACK ? BRIGHT_WHITE : static_cast<Color>(c - 1));
} // ColorPred
//-------------------------------------------------------------------------
const char *ColorName(const Color c) {
   static const char *Namen[16] =
      {"BLACK",       "BLUE",           "GREEN",         "CYAN",
       "RED",         "MAGENTA",        "YELLOW",        "WHITE",
       "GRAY",        "BRIGHT_BLUE",    "BRIGHT_GREEN",  "BRIGHT_CYAN",
       "BRIGHT_RED",  "BRIGHT_MAGENTA", "BRIGHT_YELLOW", "BRIGHT_WHITE"
      };
  return Namen[c];
} // ColorName
//-------------------------------------------------------------------------
