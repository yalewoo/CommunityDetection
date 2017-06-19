#include "Config.h"

void Config::updateConfig(const char * config_path)
{
	FILE* fin = fopen(config_path, "r");
	if (!fin)
		return;


	char buff[1024];
	while (fgets(buff, 1024, fin))
	{
		if (strlen(buff) > 0 && buff[strlen(buff) - 1] == '\n')
			buff[strlen(buff) - 1] = '\0';
		string s(buff);

		if (s.compare(0, 1, "#") == 0)
			continue;
		size_t pos = s.find('=');
		string name = s.substr(0, pos);
		string value = s.substr(pos + 1);
		config[name] = value;
	}

	fclose(fin);

}

vector<string> Config::split(string & str, char seq)
{
	char * c = &seq;
	char *cstr, *p;
	vector<string> res;
	cstr = new char[str.size() + 1];
	strcpy(cstr, str.c_str());
	p = strtok(cstr, c);
	while (p != NULL)
	{
		res.push_back(p);
		p = strtok(NULL, c);
	}
	delete[] cstr;
	return res;
}
