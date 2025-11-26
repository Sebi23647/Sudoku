Sudoku – Proiect Inginerie Software

Proiecte:
SudokuLib – logica jocului (static library)
SudokuUI – interfața grafică (folosește SFML 3.0)
SudokuTests – testele unitare (GTest + GMock)

! Cerință importantă: instalarea SFML 3.0

Descarcă SFML 3.0 pentru Visual Studio (MSVC) de aici:
https://www.sfml-dev.org/download/

Dezarhivează și pune folderul undeva stabil (ex: C:\Libraries\SFML-3.0\)

În Visual Studio, la proiectul SudokuUI:

Properties → C/C++ → General → Additional Include Directories
adaugă: C:\Libraries\SFML-3.0\include

Properties → Linker → General → Additional Library Directories
adaugă: C:\Libraries\SFML-3.0\lib

Properties → Linker → Input → Additional Dependencies
adaugă librăriile SFML necesare (sfml-graphics-3, sfml-window-3, sfml-system-3)

Copiază DLL-urile din SFML-3.0/bin în folderul de build (x64/Debug sau x64/Release).