#include <map>
#include <string>
#include <vector>


using std::map;
using std::string;
using std::vector;

class Config
{
public:
	map<string, string> config;

	void updateConfig(const char * fn);
	void updateConfig(string & fn)
	{
		updateConfig(fn.c_str());
	}

	string & operator[](string & key) {
		return config[key];
	}
	string & operator[](const char * key) {
		string s = key;
		return config[s];
	}

	vector<string> getValue(string & key)
	{
		return split(config[key], ';');
	}
	vector<string> getValue(const char * key)
	{
		return split(config[key], ';');
	}

	vector<string> split(string& str, char seq);

};