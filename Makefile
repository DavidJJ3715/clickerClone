errflags = -Wall -Wextra
linkedLibraries = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lmingw32
dependencies = Makefile main.cpp func.h shop.h
shareLinkedLibraries = SDL2_image.dll SDL2_ttf.dll SDL2.dll
ICSDL = -IC:\Users\david\Downloads\Code\SDL2\include
ICBoost = -IC:/Users/david/Downloads/Code/boost_1_86_0
LCSDL = -LC:\Users\david\Downloads\Code\SDL2\lib

main: $(dependencies)
	g++ $(errflags) main.cpp -o ClickerClone $(ICSDL) $(ICBoost) $(LCSDL) $(linkedLibraries)
	./ClickerClone.exe
	python key.py

noKey: $(dependencies)
	g++ $(errflags) main.cpp -o ClickerClone $(ICSDL) $(ICBoost) $(LCSDL) $(linkedLibraries)
	./ClickerClone.exe

noRun: $(dependencies)
	g++ $(errflags) main.cpp -o ClickerClone $(ICSDL) $(ICBoost) $(LCSDL) $(linkedLibraries)

ClickerClone: $(dependencies)
	g++ $(errflags) -static main.cpp -o ClickerClone $(ICBoost) $(shareLinkedLibraries)
	powershell Remove-Item C:\Users\david\Downloads\Code\Exports\ClickerClone\ClickerClone.exe
	powershell move ClickerClone.exe C:\Users\david\Downloads\Code\Exports\ClickerClone
	powershell Remove-Item C:\Users\david\Downloads\Code\Exports\ClickerCloneZip.zip
	powershell Compress-Archive C:\Users\david\Downloads\Code\Exports\ClickerClone C:\Users\david\Downloads\Code\Exports\ClickerCloneZip.zip
	powershell make noRun
	python key.py