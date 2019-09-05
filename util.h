#include <map>
#include <vector>

using namespace std;

typedef unsigned char byte;

// Esta estructura guarda el valor en la estructura K-V
struct Value
{
	size_t size;
	vector<byte> data;
};

// Definición de la estructura K-V utilizada
// Clave es numérica y valor es dado por estructura Value.
typedef map<unsigned long, Value> KVStore;
