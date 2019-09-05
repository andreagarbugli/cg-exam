# Moduli per la grafica

In questo capitolo introdurremo uno dei moduli più importanti di tutto il progetto, ovvero `Graphics`. Questo modulo è stato diviso all'interno di due diverse cartelle (`graphics` e `shaders`) in quanto si tratta del modulo che contiene il numero maggiore di classi; per questo motivo cercheremo di descrivere ognuna di esse nel modo più veloce e chiaro possibile.

## Classi cartella `graphics`

### `Mesh`

La classe `Mesh` ha come obbiettivo quello di rendere più semplice il lavoro su mesh utilizzando delle chiamate OpenGL.

```cpp
class Mesh
{
private:
    unsigned int _verticesNumber;
    unsigned int _uvsNumber;
    unsigned int _normalsNumber;
    unsigned int _indicesNumber;

    unsigned int _vertexArrayObject;

    unsigned int _vertexBufferObject;
    unsigned int _uvBufferObject;
    unsigned int _normalBufferObject;

    unsigned int _indexBufferObject;

    bool _hasIndices;

public:

    explicit Mesh(Utils::RawMesh& model);

    ~Mesh();

    void Use();
};
```

Il costruttore della classe prende in ingresso una `RawMesh`, ovvero l'oggetto che viene costrutito per ognuno degli oggetti presenti all'interno di un file `.obj` in fase di caricamento ([ObjFileLoader](utils)). Dopodiché costruisce un _Buffer Object_ per ognuno dei seguenti elementi: vertici, coordinate texture e normali; ed in caso siano presenti, uno anche per gli indici. Gli indici servono per non ripetere più di una volta le coordinate di uno stesso vertice nel caso faccia parte di più di una faccia.

Questi passaggi permettono di inviare i dati alla scheda video una volta sola, in questo modo possiamo richiedere direttamente i vertici che vogliamo disegnare di volta in volta solamente attivando l'_Array Object_ ed i i _Buffer Object_ associati alla mesh (metodo `Use()`), ma senza inviare nuovamente tutte le informazioni alla scheda video, essendo queste già presenti nella sua memoria. Questo permette di risparmiare motlo del tempo impiegato in operazioni di I/O.

### `Model`

La classe `Model` rappresenta appunto un modello, il quale può essere associato ad un `GameObject`. Come possiamo vedere si tratta di una classe la quale può fare riferimento anche a dei sottomodelli (ad esempio la macchina è composta dal corpo più le quattro ruote). Ma non solo, ad un modello sono associati anche:

-   Una lista di texture,
-   Una lista di materiali,
-   Lo shader da utilizzare per il modello.

Tutti elementi essenziali per la fase di rendering.

```cpp
struct SubModel
{
    Model* parent;
    Mesh* mesh;
    Utils::Material* material;
    bool hasTextures;
};

class Model
{
private:

    std::map<std::string, SubModel*> _meshes;
    std::map<std::string, Texture*> _textures;
    std::map<std::string, Utils::Material*> _materials;

    BasicShader* _shader;

    std::string _name;

public:

    static bool sortByDrawingOrdin futuro poer(Model* m1, Model* m2);

    Model(std::string name);

    void Draw(BasicShader* shader);
};
```

Il metodo `Draw` è quello che viene chiamato, per ognuno degli oggetti che fanno parte della scena, all'interno del ciclo del metodo `Render()` della classe [Game](engine-core/#metodo-render).

### `Texture`

Come molte altre classi si occupa di incapsulare le funzioni OpenGL per lavorare con le Texture.

La classe mette a disposizione un metodo `Load` che prende in ingresso il nome del file immagine. Dopodiché fa uso della libreria SDL2_Image e delle funzioni OpenGL per leggere il file e creare una texture.

Fanno parte di questa classe anche due funzioni chiamate:

```cpp
void CreateForRendering(int width, int height, unsigned int format);

void CreateFromSurace(SDL_Surface* surface);
```

La prima permette di creare una texture associata al _FrameBuffer_. In questo modo è possibile creare il contenuto della texture renderizzando la scena 3D (utilizzata nel caso della minimappa). La seconda crea una texture a partire da una `SDL_Surface`. Questa seconda funzione viene utilizza per creare texture da usare nell'interfaccia grafica.

### `Light`

Più che di una classe si tratta di file che contiene tre strutture per rappresentare i tre diversi tipi di luci che sono presenti nel gioco, ovvero:

-   Direzionale,
-   Point Light,
-   Spot Light.

Queste tre strutture permettono di salvare i dati da dover poi passare agli shader attraverso degli `uniform`, come vedremo alla fine di questo capitolo.

### `Terrain`

La classe `Terrain` è una delle classi che specializza `GameObject`. Nel nostro caso abbiamo deciso di utilizzare il metodo delle blend map per applicare diverse texture allo stesso terreno, in modo da differenziarlo un pochino. Per questo motivo, nel costruttore, vengono caricate cinque diverse texture. Le prime quattro hanno i seguenti compiti:

-   le prime tre sono le texture vere e proprie da applicare al terreno,
-   la quarta è la blend map vera e propria, ovvero una texture che viene utilizzata per mischiare le altre tre in base ai valori `red`, `green` e `blue` per ognuno dei pixel.

La quinta texture è semplicemente una texture nera che verrà utilizzata come specular map.

Ad un certo punto dello sviluppo abbiamo implementato anche un metodo per generare diverse altezze per il terreno, metodo delle height map. La funzione che permette di generare questo effetto è ancora presente nel codice, ma è stato scelto di non usarlo nel gioco finale, in quanto si sarebbe dovuto implementare anche un meccanismo per disegnare gli altri oggetti della scena in una posizione consona all'altezza del terreno.

### `Skybox`

La classe `Skybox` viene usata per disegnare un ambiente fittizio intorno alla scena.

Nel costruttore di questa classe vengono creati uno `ShaderProgram` apposito ed un `CubeMap`, ovvero un cubo sul quale vengono applicate sei diverse texture.

Questo cubo viene poi disegnato intorno alla scena 3D di gioco in modo da dare l'impressione di trovarsi all'interno di un ambiente reale.

### `ShaderProgram`

Si tratta della classe che si occupa della creazione di un shader program di OpenGL. Attualmente permette di caricare e compilare solamente due tipi di shader, ovvero _vertex_ e _fragment shader_.

```cpp
 class ShaderProgram
{
private:
    unsigned int _vertexShader;
    unsigned int _fragShader;
    unsigned int _shaderProgram;

public:
    ShaderProgram() = default;

    bool Load(const std::string& vertName, const std::string& fragName);


private:
    bool _CompileShader(const std::string& fileName,
                        GLenum shaderType, unsigned int& shader);
}
```

Oltre a rendere più semplice le fasi di creazione di uno shader program, mette a disposizione anche alcune funzioni per settare i valori delle variabili `uniform` presenti nel programma.

## Classi cartella `shaders`

### `BasicShader`

Questa classe può essere utilizzata come base per la creazione di altre classi che rappresentano degli shader specifici.

```cpp
class BasicShader
{
protected:
    ShaderProgram* _shaderProgram;

    std::string _name;

    Engine::Game* _game;

public:
    BasicShader(Engine::Game* game, std::string& name) :
            _game{game},
            _name{name}
    {
    }

    BasicShader(Engine::Game* game, ShaderProgram* shaderProgram) :
            _game{game},
            _shaderProgram{shaderProgram}
    {
    }

    virtual void Use();

    virtual void SetModel(class SubModel* model);

    void SetModelMatrix(glm::mat4 modelMatrix);
};
```

Il costruttore di default prende in ingresso il riferimento alla classe `Game` ed il nome che si vuole associare allo shader, saranno poi le classi che la specializzano a salvare nel campo protetto `ShaderProgram* _shaderProgram` il programma shader al quale fanno riferimento. Attualmente abbiamo implementato tre diverse classi che estendono `BasicShader`:

-   `MaterialShader`. Questa classe viene utilizzata nei vari modelli del gioco come la macchina, gli alberi ed il cartello con la foto. Permette sia di utilizzare le texture che di utilizzare dei semplici valori di albedo, diffuse e specular per colorare l'oggetto.
-   `TerrainShader`. E' la classe che rappresenta lo shader per la visualizzazione del terreno. Dato che nello shader GLSL è stato utilizzato il meccanismo della _blend map_ per applicare diverse texture allo stesso terreno, la classe si occupa anche di impostare l'uniform intero associato ad ognuna delle texture utilizzate.
-   `GUIShader`. Si tratta della classe più semplice delle tre in quanto lo shader GLSL che utilizza ha solamente un uniform che rappresenta la texture da applicare all'elemento dell'interfaccia grafica.

Ognuna di queste classi specializza il metodo `Use()`, il quale sfrutta i metodi `SetUniformXXX()` messi a disposizione da `ShaderProgram` per cambiare i valori dei diversi `uniform` che fanno parte dello shader GLSL.

### Esampio di shader GLSL

#### Vertex Shader

Ora vediamo velocemente il vertex shader utilizzano nella classe `MaterialShader`. Si tratta di un programma molto semplice, infatti prende in ingresso la posizione del vertice, la sua normale e le coordinate della texture ad esso associata (ricordiamo che il vertex shader viene chiamato per ognuno dei vertici che fanno parte della mesh).

Lo shader richiede anche le matrici del modello, della vista e quella di proiezione. Queste matrici vengono passate direttamente dal codice C++ essendo degli `uniform`.

In uscita dal vertex shader abbiamo:

-   La normale con la trasformazione applicata. Questo valore verrà utilizzato nel fragment shader per diversi calcoli, come ad esempio definire quanto incide il valore della specular da un determinato punto di vista.
-   Le coordinate della texture e le coordinate del fragment.
-   Infine la coordinata del vertice con le tre trasformazioni applicate.

```GLSL
#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord0;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    Normal = mat3(transpose(inverse(model))) * vNormal;
    TexCoord0 = vTexCoord;
    FragPos = vec3(model * vec4(vPosition, 1.0));
    gl_Position = projection * view * model *  vec4(vPosition, 1.0);

}
```

#### Fragment shader

Questo programma è più complesso di quello precedente, e presenta al suo interno anche alcune strutture che permettono una maggiore comprensione dei valori con i quali stiamo lavorando. Abbiamo infatti:

```GLSL
#version 330

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 2

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord0;

struct Material {
    sampler2D diffuseTex;
    sampler2D specularTex;

    bool hasTextures;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

struct Light {

    // positon per point light
    // direction per directional light
    // con spot light si usano entrambe
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // per point light
    float constant;
    float linear;
    float quadratic;

    // per spot light
    float cutOff;
    float outerCutOff;
};

uniform Material material;

uniform Light dirLight;

uniform Light pointLigths[NR_POINT_LIGHTS];
uniform Light spotLigths[NR_SPOT_LIGHTS];

uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirectionalLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calcPointLight(pointLigths[i], norm, FragPos, viewDir);
    }

    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        calcSpotLight(spotLigths[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}
```

Lo shader in questione calcola il colore per ognuno dei punti come somma dei valori dati: dalla luce direzione (ad esempio il Sole), da tutte le luci puntiformi (una lampadina) ed infine dalle luci di tipo spot (tipo torcia).

Come esempio prendiamo ora la funzione per calcolare come il colore dell'oggeto varia in base alla luce direzionale. Abbiamo quindi la funzione:

```GLSL
vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient, diffuse, specular;
    if (material.hasTextures)
    {
        ambient = light.ambient * vec3(texture(material.diffuseTex, TexCoord0));
        diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, TexCoord0));
        specular = light.specular * spec * vec3(texture(material.specularTex, TexCoord0));
    }
    else
    {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }

     return (ambient + diffuse + specular);
}
```

Questa funzione prende come ingresso la luce direzionale, il vettore che rappresenta la normale del vertice ed il vettore che rappresenta la direzione della quale stiamo osservando la scena.

Per prima cosa normalizziamo il vettore direzione della luce, dopodiché dobbiamo calcolare con quale angolo la luce colpisce l'oggetto. Per fare questo utilizziamo il prodotto scalare insieme alla funzione `max` (questo perchè se l'angolo è maggiore di 90° otteniamo un valore negativo ed in questo modo mettiamo come valore minimo possibile 0.0, ovvero la luce direzione non aggiunge nulla al colore finale).

Adesso che abbiamo calcolato la componenete diffuse del colore, passiamo a quella della specular. Prima di tutto calcoliamo il prodotto scalare fra la direzione di vista e la direzione del riflesso, e come abbiamo fatto in precedenza controlliamo che non sia negativo. A questo punto eleviamo tutto per il coefficiente di lucentezza. Più alto è questo valore e maggiore sarà la riflessione della luce da parte dell'oggetto.

Infine controlliamo se la mesh fa uso o meno di texture e calcoliamo le componenti colore finali in base a questo.
