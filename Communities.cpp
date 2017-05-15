#include "Communities.h"

#include <cctype>
#include "Communities.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;


void Communities::loadInfomap(const char * fn)
{
	clear();

	FILE *fp = fopen(fn, "r");
	if (!fp)
		return;
	char buff[512];
	fgets(buff, 512, fp);
	fgets(buff, 512, fp);

	int n;
	int lastn = 0;
	int node;
	int nc = -1;

	while (fgets(buff, 512, fp))
	{
		//定位到倒数第二个数字 三层时1:2:3 会读出2
		int i = 0;
		while (buff[i] != ' ')
			++i;
		while (buff[i] != ':')
			--i;
		--i;
		while (isdigit(buff[i]))
		{
			--i;
			if (i == -1)
				break;
		}

		++i;
		//社团编号
		sscanf(buff + i, "%d", &n);

		i = strlen(buff);
		while (buff[i] != ' ')
			--i;
		++i;
		//节点号
		sscanf(buff + i, "%d", &node);

		if (n != lastn)	//开始新的社团
		{
			++nc;
			Community c;
			c.add(node);
			addCommunity(c);
		}
		else
		{
			comms[nc].add(node);
		}
		lastn = n;
	}
}

void Communities::loadLinkComm(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);

		for (size_t i = 0; i < s.size(); ++i)
		{
			if (s[i] == ',') s[i] = ' ';
		}
		stringstream stream;
		stream << s;
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);

	}
}

void Communities::loadOSLOM2(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);

		if (s.size() > 0 && s[0] == '#')
			continue;

		stringstream stream;
		stream << s;
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		//c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);
	}
}

void Communities::print()
{
	printf("-------------------------\n");
	printf("%d communities:\n", comms.size());
	for (size_t i = 0; i < comms.size(); ++i)
	{
		for (size_t j = 0; j < comms[i].nodes.size(); ++j)
		{
			printf("%d ", comms[i].nodes[j]);
		}
		printf("\n");
	}
}
