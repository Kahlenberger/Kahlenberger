# Datei Makefile
################################################################
# Dateiname    :
#  $Source: /CVS/EndlicherAutomat/Makefile,v $
# CVS-User     :$Author: dd6zc $
# Datum(commit):$Date: 2016/10/24 00:50:00 $
# Revision     :$Revision: 1.18 $
# Versionsmarke:$Name:  $
################################################################

#CC=c:\mingw\bin\g++		# Compiler
# ...Great! CFLAGS=-c -g -Wall -ansi -O			# Compiler-Optionen
# ...and w/o debug symbols ... CFLAGS=-c -g -ansi -O			# Compiler-Optionen
CFLAGS=-c -ansi -O			# Compiler-Optionen
#LD=c:\mingw\bin\g++		# Linker
CC=g++
LD=g++

#all: EndlicherAutomat.exe KonsoleTst.exe
all: EndlicherAutomat.exe

#EndlicherAutomat.exe: EndlicherAutomat.o Konsole.o	
#	${LD} -o EndlicherAutomat.exe EndlicherAutomat.o Konsole.o
EndlicherAutomat.exe: EndlicherAutomat.o	
	${LD} -o EndlicherAutomat.exe EndlicherAutomat.o

#KonsoleTst.exe: KonsoleTst.o Konsole.o
#	${LD} -o KonsoleTst.exe KonsoleTst.o Konsole.o

EndlicherAutomat.o: EndlicherAutomat.cpp
	${CC} ${CFLAGS} EndlicherAutomat.cpp 	

KonsoleTst.o: KonsoleTst.cpp Konsole.h
	${CC} ${CFLAGS} KonsoleTst.cpp

Konsole.o: Konsole.cpp Konsole.h
	${CC} ${CFLAGS} Konsole.cpp

clean:
	@echo -n "Loesche Objektdateien ... "
	@rm -f *.o
	@echo "Fertig!"
	@echo -n "Loesche Binaries      ... "
	@rm -f *.exe
	@echo "Fertig!"	
