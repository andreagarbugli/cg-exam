# Introduzione

Questo progetto è stato realizzato in C++ facendo uso sia delle librerie standard del linguaggio che delle seguenti librerie:

- OpenGL (3.3+). Nel caso specifico si è fatto uso delle così dette __Modern OpenGL__, ovvero è stato del tutto evitato l'utilizzo della _fixed pipeline_ di OpenGL.
Per questo motivo è stato necessario realizzare tutti gli shader obbligatori per renderizzare un modello a schermo (_vertex_ e _fragment shaders_).
- [SDL2](https://www.libsdl.org/index.php). Utilizzata per la creazione della finestra di gioco e per la gestione degli input da mouse e tastiera.
- SDL_image 2.0. Per il caricamento di file immagini utilizzate come texture.
- SDL2_ttf 2.0. Per la gestione dei font utilizzati nell'interfaccia utente.
- [gl3w](https://github.com/skaslev/gl3w). Per caricare e gestire le funzionalità offerte dal core profile di OpenGL.
- [glm](https://glm.g-truc.net/0.9.9/index.html). Libreria matematica header only scritta in C++ e specifica per essere utilizzate in programmi di computer grafica.
 GLM è composta da classi e funzioni progettate ed implementate con le stesse convenzioni e funzionalità di GLSL, e questo la rende perfetta per essere utilizzata in progetti di questo tipo senza dover costruire una propria libreria matematica.
 
Per lo sviluppo del software sono stati utilizzati i seguenti tool:

- Per la scrittura del codice è stato utilizzato l'IDE [CLion](https://www.jetbrains.com/clion/) in versione gratuita per gli studenti 
- Per la compilazione del progetto è stato scelto di utilizzare [CMake](https://cmake.org/), questo per facilitare la compilazione stessa dato che sono state utilizzate delle librerie esterno come sopra indicato e per facilitare anche un eventuale sviluppo multipiattaforma.
- Come sistema di versioning del software è stato scelto [git](https://git-scm.com/) ed il progetto completo è hostato su [github](https://github.com/) nel repository [andreagarbugli/cg-exam](https://github.com/andreagarbugli/cg-exam).

Il progetto è stato realizzato e testato su Ubuntu 19.04.

## Compilazione ed esecuzione del progetto

Nella seguente sezione verranno specificati i passaggi necessare per compilare ed eseguire il progetto su sitemi linux.

Gli step da compiere sono:

- Assicurarsi di avere installate nel computer le seguenti librerie `libsdl2-dev, libsdl2-ttf-dev, libsdl2-image-dev`, nel caso non fossero presenti sarà possibile installarele tramite comando da terminale:

```bash
~$ sudo apt-get install {packet-name}
```

-