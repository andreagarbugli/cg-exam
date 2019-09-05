# Modulo Core dell'Engine

In questo capitolo discuteremo delle classi che compongono il nucleo dell'engine di gioco.

## `InputManager`

La prima classe che analizzeremo sarà quella che permette la gestione degli input, ovvero:

```cpp
class InputManager
{
  public:
    static InputManager *GetInstance()
    {
        static auto *_instance = new InputManager();
        return _instance;
    }

    void Init();

    void Update();

    bool IsKeyDown(KeyboardKey key);

    bool IsKeyUp(KeyboardKey key);

    bool IsKeyPressed(KeyboardKey key);

    bool IsMouseDown(MouseButton mouseButton);

    bool IsMouseUp(MouseButton mouseButton);

    bool IsMousePressed(MouseButton mouseButton);
}
```

Come è possibile vedere dal pezzo di codice, dove per semplicità sono stati omessi i campi privati e parte dei metodi, la classe `InputManager` è stata sviluppata come _Singleton_, in questo modo è possibile referenziare una singola instanza di questa classe in tutto il sistema. Per ottenere l'istanza occorre effettuare una chiamata al metodo statico `GetInstance()`.

Il metodo più importante della classe è sicuramente il metodo `Update` (anche in questo caso ne mostreremo solamente una piccola parte).

```cpp
void InputManager::Update()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                _quit = true;
                break;

            case SDL_TEXTINPUT:
                break;

            case SDL_KEYDOWN:
            {
                _keyboard = SDL_GetKeyboardState(nullptr);
                auto index = event.key.keysym.scancode;
                _keyDown[index] = true;
                break;
            }
        }
    }
}
```

Questo metodo aggiorna lo stato relativo a tutti gli input, sia del mouse che della tastiera, in modo da poter essere utilizzato successivamente dalle classi di gioco che posseggono il riferimento all'istanza della classe `InputManager`.

Insieme alla classe per la gestione degli input è stato sviluppato anche un file header chiamato `InputDefinitions.h`. All'interno di questo file possiamo trovare gli enumerativi che fanno da wrapper per tutti gli input gestiti da SDL2. Questa classe rende più semplice l'utilizzo degli input all'interno delle altre classi di progetto, oltre a facilitare un eventuale passaggio ad un'altra libreria per la gestione degli input.

## `Window`

La classe `Window` è un piccolo wrapper che incorpora una finestra creata dalla libreria SDL2. Come nel caso precedente, oltre a permette un più facile utilizzo delle finestra di gioco, fornisce anche una maggiore astrazione nel caso si voglia cambiare libreria.

```cpp
class Window
{
private:
    SDL_Window* _sdlWindow;
    SDL_GLContext _glContext;

    int _width;
    int _height;

public:
    Window() = default;

    static Window* CreateWindow(std::string name, int width, int height);

    void SwapBuffers();

    void Close();

    void Clear();
}
```

## `GameObject`

Di seguito abbiamo una delle classi più importanti dell'engine, in quanto si tratta della classe che rappresenta ogni singolo oggetto della scena di gioco. Questa classe fa da base per tutte le classi presenti nella cartella `game`. Tali classsi verranno discusse nel capitolo sul [gioco](/game)

```cpp
class GameObject
{
protected:
    GameObjectState _state;

    glm::vec3 _scale;
    glm::vec3 _rotation;
    glm::vec3 _position;

    class Game* _game;

    glm::mat4 _modelMatrix;

    InputManager* _inputManager = InputManager::GetInstance();

public:
    GameObject(class Game* game, std::string name,
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
            _game{game},
            _name{std::move(name)},
            _position{position},
            _scale{scale},
            _rotation{rotation},
            _state{GameObjectState::Created},
            _rotationAngle{0}
    {
    }
}
```

Come possiamo vedere questa classe presenta tutti i parametri per definire la posizione, scala e rotazione di un oggetto in scena. In questo modo tutte le classi che deriveranno da questa non dovranno specificare nessuno di questi campi, ma semplicemente implemetare la logica di aggiornamento degli stessi attraverso la ridefinizione della funzione `Update()`.

Opzionalmente la classe può avere anche un riferimento ad un modello (classe [Model](/graphics#model)), ovvero l'oggetto che deve essere disegnato a schermo. Questo campo è opzionale perchè come vedremo nella prossima sezione, nonstante la classe `BasicCamera` derivi da `GameObject`, non ha bisogno di nessun modello ad essa associtao, in quanto solitamente oggetti di scena come la camera non vengono disegnati.

### `Camera`

La classe `BasicCamera` è la prima che vediamo che estende `GameObject`. Questa classe farà a sua volta da base per due diversi tipi di camera implementati all'interno del gioco. I suoi aspeti più importanti sono:

-   la funzione `GetViewMatrix()` che permette di ottenere la matrice per applicare la trasformazione di vista agli oggetti della scena.
-   la funzione `Update()`, ovvero una funzione la quale ricalcola ogni volta la matrice per la trasformazione di vista, e che prima di fare questo richiama una funzione virtuale, chiamata `_UpdatePosition()`, che può essere implementata dalle classi che estendono `BasicCamera`, in modo da gestire la posizione delle camere nel modo più apportuno al tipo di movimento che si vuole creare.

```cpp
void BasicCamera::Update(double deltaTime)
{
    _UpdatePosition(deltaTime);

    _viewMatrix = _CalculateViewMatrix();
}
```

-   `Camera` Questa prima versione permette il movimento della vista attraverso la tastiera ed il muose, possiamo quindi parlare di _Free Camera_. Ogni volta che viene aggiornato l'intero gioco nel loop principale che vedremo fra poco, viene aggiornata anche la posizione della camera in base agli input del giocatore e di conseguenza deve essere ricalcolata la matrice per la trasformazione di vista.

-   `FollowCamera` Il secondo tipo di camera implementato è un tipo di camera che segue un determinato `GameObject`. In questo caso la classe si tiene salvato il riferimento ad un oggetto della scena e si muove in modo relativo al movimento quest'ultimo.

## `Game`

Parleremo ora della classe principale di tutto il progetto, ma prima di fare ciò mostreremo il `main` del progetto, ovvero il file dove questa viene utilizzata:

```cpp
#include "engine/Game.hpp"

int main(int argc, char const *argv[])
{
    auto *p = new Engine::Game();

    p->Run();

    return 0;
}
```

Esatto il `main` del progetto si compone solamente di queste righe di codice. Ora spiegheremo passo per passo perchè il `main` è così semplice e quali sono le operazioni che compie la funzione `Run()` della classe `Game`.

Una versione semplificata di tale funzione può essere espressa come:

```cpp
void Game::Run()
{
    if (Init())
    {
        _globalStateManager->Start();

        StartUp();

        // game loop
        while (_globalStateManager->IsRunning())
        {
            ProcessInput();

            Update();

            Render();
        }

        ShutDown();
    }
}
```

In questi pochi passi possiamo vedere tutto quello che viene fatto durante l'esecuzione del gioco.

### Metodo `Init()`

Metodo attraverso il quale vengono inizializzati tutti i sottosistemi che servono all'applicazione per funzionare, come ad esempio:

-   Inizializzazione di SDL_Video, SDL_Image, SDL_TTF.
-   Vengono settati tutti gli attributi di OpenGL.
-   Inizializzato gl3w per utilizzare il profilo core di OpenGL.

Dopodichè viene settato lo stato globale come in esecuzione.

### Metodo `StartUp()`

A questo punto vengono caricati tutti i modelli, le texture, luci ed elementi che compongono la scena di gioco.

Da questo momento, fino a quando non viene settato lo stato globale per la richiesta di chiusura del programma, entriamo nel _game loop_. Questo ciclo continuo viene interrotto solamente in caso di chiusura del gioco, e ad ogni iterazione compie i seguenti passi:

-   Controllo dei nuovi input da parte del giocatore.
-   Aggiornamento di tutti gli oggetti che fanno parte del gioco.
-   Render della scena e visualizzazione a schermo.

### Metodo `ProcessInput()`

Viene aggiornato lo stato dell'`InputManager` e poi vengono gestiti alcuni degli di interesse per il gioco, come ad esempio la richiesta di chiusura.

```cpp
void Game::ProcessInput()
{
    _inputManager->Update();

    if (_inputManager->QuitRequested() ||
        _inputManager->IsKeyDown(KEY_ESCAPE))
    {
        _globalStateManager->Stop();
        return;
    }
}
```

### Metodo `Update()`

Vengono aggiornati tutti i `GameObject` del gioco secondo la specifica implementazione del metodo `Update()` per ogni classe che estende la classe base `GameObject` appunto.

```cpp
void Game::Update(double elapsedTime)
{
    _updatingGameObjects = true;
    for (auto gameObject : _gameObjects)
    {
        gameObject->Update(elapsedTime);
    }
    _updatingGameObjects = false;
}
```

### Metodo `Render()`

Il metodo che renderizza la scena finale è diviso a sua volta in diversi metodi:

-   Prima di tutto ridimensiona la viewport, dato che potrebbe essere stata modificata per generare altre scene (ad esempio per la generazione della minimappa nel nostro caso).
-   Se la minimappa è attiva, allora prima di renderizzare la scena vera e propria viene renderizzata una sua copia da un punto di vista specifico, per poi utilizzare tale scena come texture da applicare alla mesh della minimappa.
-   A questo punto viene renderizzata la scena finale.
-   Infine viene renderizzata anche l'eventuale interfaccia grafica, direttamente sopra la scena 3D.

```cpp
void Game::Render()
{
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_isMinimapActive)
    {
        _RenderMinimap();
    }

    _RenderScene(0);

    _RenderGUI();

    glUseProgram(0);
}
```

### Metodo `ShutDown()`

Come facile intuire chiude la finestra ed il programma.