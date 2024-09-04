noflags =
flags = -g -fsanitize=leak
errflags = -Wall -Wextra
linkedLibraries = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lmingw32
dependencies = Makefile main.cpp func.h 
shareLinkedLibraries = SDL2_image.dll SDL2_ttf.dll SDL2.dll

main: $(dependencies)
	g++ $(errflags) main.cpp -o ClickerClone -IC:\Users\david\Downloads\Code\SDL2\include -LC:\Users\david\Downloads\Code\SDL2\lib $(linkedLibraries)
	./ClickerClone.exe
	python key.py

noKey: $(dependencies)
	g++ $(errflags) main.cpp -o ClickerClone -IC:\Users\david\Downloads\Code\SDL2\include -LC:\Users\david\Downloads\Code\SDL2\lib $(linkedLibraries)
	./ClickerClone.exe

noRun: $(dependencies)
	g++ $(errflags) main.cpp -o ClickerClone -IC:\Users\david\Downloads\Code\SDL2\include -LC:\Users\david\Downloads\Code\SDL2\lib $(linkedLibraries)

ClickerClone: $(dependencies)
	g++ $(errflags) -static main.cpp -o ClickerClone $(shareLinkedLibraries)
	powershell Remove-Item C:\Users\david\Downloads\Code\Exports\ClickerClone\ClickerClone.exe
	powershell move ClickerClone.exe C:\Users\david\Downloads\Code\Exports\ClickerClone
	powershell Remove-Item C:\Users\david\Downloads\Code\Exports\ClickerCloneZip.zip
	powershell Compress-Archive C:\Users\david\Downloads\Code\Exports\ClickerClone C:\Users\david\Downloads\Code\Exports\ClickerCloneZip.zip
	powershell make noRun
	python key.py