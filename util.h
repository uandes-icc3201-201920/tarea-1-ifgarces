#include <map>
#include <vector>

using namespace std;

struct StructVal
{
	size_t size;
	vector<unsigned char> data;
};
//typedef map<unsigned long, StructVal> KVStore;

class KeyValue
{
	public:
		int key;
		char* value;
		
		KeyValue(int k, char* v)   // constructor
		{
			key = k;
			value = v;
		}
};

typedef map<int, KeyValue> KVStore;