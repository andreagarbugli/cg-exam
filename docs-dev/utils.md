# Modulo di Utility

Per il momento questo modulo contiene solamente una libreria che permette il caricamento di mesh e materiali a partire da file `.obj`.

## `ObjFileLoader`

Una volta create un'istanza della classe è possibile caricare e leggere un file `.obj` attraverso il metodo:

```cpp
bool LoadObj(std::string fileName, bool indexed = false);
```

Questa metodo richiama al suo interno un secondo metodo privato `_ReadObjFile(const std::string& fileName)` che legge il file riga per riga, e per ognuna di esse richiama un metodo apposito in base al tipo di elemento che deve essere letto e salvato (ad esempio vertici, faccie, materiali ecc.).

Per il momento la classe permette di leggere file contenenti solamente degli oggetti preventivamente triangolati (e possibile effettuare questo passaggio attraverso [Blender](https://www.blender.org/) in fase di esportazione dell'oggetto come file `.obj`). Comunque abbiamo implementato anche il fatto di leggere più di un oggetto all'interno del file, ma solamente se questo è salvato come oggetto singolo e non come gruppo.

Alla fine dell'esecuzione, se tutto andrà come previsto, otterremo due vettori. Il primo contenente tutti gli oggetti espressi come mesh nel seguente formato:

```cpp
struct RawMesh
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::string materialName;
    std::string name;
}
```

Il secondo è invece il vettore dei materiali, questo volta definiti secondo la seguente struttura:

```cpp
struct Material
{
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float ns;
    std::string mapKd;
    std::string mapKs;
    float d;
    int illum;
    std::string name;
};
```

Come è possibile vedere nella struttura del materiale è presente sia la rappresentazione mediante colori (`ka`, `kd`, `ks`) , o direttamente quella che fa uso di texture (`mapKd`, `mapKs`).

Questi due vettori fanno parte direttamente dell'istanza della classe `ObjFileLoader` creata inizialmente, e possono essere richiesti rispettivamente tramite i metodi:

```cpp
std::vector<RawMesh*> GetMeshes();

std::vector<Material*> GetMaterials();
```