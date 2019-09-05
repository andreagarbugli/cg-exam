# Progetto esame fondamenti di computer graphics

L'intero progetto è stato realizzato in C++ facendo uso sia delle librerie standard del linguaggio che delle seguenti librerie:

-   OpenGL (3.3+). Nel caso specifico abbiamo fatto uso delle così dette _Modern OpenGL_, ovvero abbiamo del tutto evitato l'utilizzo della _fixed pipeline_ di OpenGL. Per questo motivo è stato necessario realizzare tutti gli shader obbligatori per renderizzare un modello a schermo (_vertex_ e _fragment shaders_).
-   [SDL2](https://www.libsdl.org/index.php). Utilizzata per la creazione della finestra di gioco e per la gestione degli input da mouse e tastiera.
-   SDL2_image. Per il caricamento di file immagine utilizzati come texture.
-   SDL2_ttf. Per la gestione dei font utilizzati per scriver del testo a schermo.
-   [gl3w](https://github.com/skaslev/gl3w). Per caricare e gestire le funzionalità offerte dal core profile di OpenGL.
-   [glm](https://glm.g-truc.net/0.9.9/index.html). Libreria matematica header only scritta in C++ e pensata per essere utilizzata in programmi di computer grafica. GLM è composta da classi e funzioni progettate ed implementate con le stesse convenzioni e funzionalità di GLSL, e questo la rende perfetta per essere utilizzata in progetti di questo tipo senza dover costruire una propria libreria matematica.

Per lo sviluppo del software abbiamo utilizzato i seguenti tool:

-   Per la scrittura del codice abbiamo fatto uso dell'IDE [CLion](https://www.jetbrains.com/clion/) nella sua versione gratuita per studenti.
-   Per la compilazione del progetto abbiamo scelto di utilizzare [CMake](https://cmake.org/), questo per facilitare la fase di compilazione del progetto, sia perchè abbiamo utilizzato delle librerie esterne come indicato nei punti precedenti, sia per rendere più semplice un eventuale sviluppo multipiattaforma.
-   Come sistema di versioning del software abbiamo utilizzato [git](https://git-scm.com/) e l'intero progetto è stato poi caricato direttamente su [github](https://github.com/) nel repository [andreagarbugli/cg-exam](https://github.com/andreagarbugli/cg-exam).

Questo progetto di esame è stato realizzato e testato su Ubuntu 19.04.

## Struttura del Progetto

Il progetto è strutturato nelle seguenti cartelle e sottocartelle:

-   `bin` Contiene tutti i file che servono in fase di esecuzione del programma compreso il file eseguibile del progetto stesso.

    -   `assets` File per il font utilizzato nell'interfaccia grafica.
    -   `models` Modelli in formato [obj](https://en.wikipedia.org/wiki/Wavefront_.obj_file) e i rispettivi file dei materiali [mtl](https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library)
    -   `shaders` Vertex e fragment shader scritti nel linguaggio GLSL.
    -   `textures` Immagini utilizzate come texture per le mesh, il terreno di gioco e lo skybox.

-   `docs` Cartella che contiene il sito statico della documentazione generato automaticamente da [Vuepress](https://vuepress.vuejs.org/).
-   `docs-dev`: Insieme di file markdown per lo sviluppo della documentazione.

-   `extern` Librerie di terze parti utilizzate nel programma come indicato all'inizio del capitolo [introduttivo](/)

-   `src` Questa cartella contiene tutto il codice C++ del programma ed è a sua volta suddivisa in sottocartelle per i vari moduli del sistema:

    -   `engine` Contiene il codice core del programma come le classi per la creazione e gestione della finestra di gioco e degli input, oltre alle classi basi per l'applicazione e gli oggetti di gioco
    -   `game` Cartella dove sono presenti tutte le classi specifiche del gioco, ovvero le classi che rappresentano i vari oggetti della scena.
    -   `graphics` Contiene tutte le classi che astraggono e fanno da wrapper alle funzioni di OpenGL, in questo modo risulta più semplice la gestione di _mesh_, _luci_, _texture_ e tutti gli altri elementi grafici.
    -   `math` Contiene un piccolo file utilty con alcune funzioni matematiche.
    -   `shaders` Contiene tutte le classi wrapper per gli shader utilizzati nel gioco.
    -   `utils` Contiene una piccola libreria per il caricamento sia di file `.obj` che dei file `.mtl` ad essi associati (ovvero i file dei materiali).

    In ogni cartella è presente anche un file `CMakeLists.txt` che consente a `cmake` di mantenere i riferimenti ai file C++ e di conseguenza di gestire il progetto sia in fase di sviluppo che di compilazione. In questo modo è possibile avere strumenti come l'autocompletamento del codice o spostarsi dalle dichiarazioni di funzioni, classi e oggetti alla loro definizione. Logicamente per utilizzare questa funzioni occorre un IDE che supporti cmake come nel caso di CLion (oppure VS Code tramite apposita estensione).

Nella prossima sezione spiegheremo i passi necessari per compilare ed eseguere il progetto d'esame, mentre nei prossimi capitoli approfondiremo alcuni dei file presenti all'interno della cartella `src`.

## Compilazione ed esecuzione del progetto

Nella seguente sezione verranno specificati i passaggi essenziali per compilare ed eseguire il progetto su sitemi linux.

Gli step da compiere sono:

-   Assicurarsi di avere installate nel computer le seguenti librerie `libsdl2-dev, libsdl2-ttf-dev, libsdl2-image-dev`, e nel caso non fossero presenti installarle tramite terminale con il comando:

    ```bash
    sudo apt-get install {packet-name}
    ```

-   Assicurarsi di avere installato anche `cmake`, oppure installarlo tramite il seguente comando:

    ```bash
    sudo apt-get install cmake
    ```

-   Tutte le altre librerie utilizzate nel progetto sono incluse sotto forma di codice all'interno della cartella `extern` del progetto e pertanto verranno compilate insieme al progetto stesso tramite cmake.

-   A questo punto occorre effettuare i seguenti passi per compilare con cmake:

    -   Creazione di una cartella per la build e configurazione del progetto con cmake:

        ```bash
        # portarsi all'interno della root del progetto,
        cd cg-exam
        # crea la cartella per la compilazione
        # e sposta al suo interno.
        mkdir build && cd build
        # configura il progetto tramite cmake.
        # -DCMAKE_BUILD_TYPE={build-type} serve per compilare
        # il progetto con o senza ottimizzazioni, i valori
        # possibili sono:
        #   - Release (consigliato)
        #   - Debug
        #   - RelWithDebInfo
        #   - MinSizeRel
        cmake -DCMAKE_BUILD_TYPE=Release ..
        ```

    -   Diamo per scontato che i seguenti passi siano stati eseguiti in un sistema Linux e che quindi il tool di building selezionato automaticamente da cmake sia `make`, occorre ora eseguire il comando:

        ```bash
        # esegue la compilazione vera e propria del progetto.
        # -j {nproc} serve per compilare il progetto usando
        # più processori.
        make -j 8
        ```

-   Se i passaggi precedenti saranno andati a buon fine, all'interno della cartella `bin` verrà generato il file eseguibile del progetto, il quale potrà essere eseguito in questo modo:

    ```bash
    ./cg-exam
    ```