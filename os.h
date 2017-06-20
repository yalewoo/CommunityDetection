#include <cstdlib>

#define SHOW_CMD true

struct os {
	
#ifdef _WIN32
	static void deleteFile(const string & dir)
	{
		cmd("del /S/Q \"" + dir + "\"");
	}
	static void deleteDir(const string & dir)
	{
		cmd("rd /S/Q \"" + dir + "\"");
	}
	static void moveFile(const string & source, const string & target)
	{
		cmd("move /Y " + source + " " + target);
	}
	static void moveDir(const string & dirname, const string & target)
	{

		deleteDir(target + dirname);
		string dir = dirname;
		if (dir[dir.size() - 1] == '/')
			dir[dir.size() - 1] = ' ';
		moveFile(dir, "\""+target+"\"");
	}
	static void mkdir(const string &dir)
	{
		deleteFile(dir);

		string dir_cmd = dir;
		for (size_t i = 0; i < dir_cmd.size(); ++i)
		{
			if (dir_cmd[i] == '/') dir_cmd[i] = '\\';
		}
		cmd("mkdir " + dir_cmd);
	}

	static void cmd(const string s)
	{
		//s += " >> log.txt";
		cmd(s.c_str());
	}

	static void cmd(const char * s)
	{
		if (SHOW_CMD)
			printf("Call: %s\n", s);
		system(s);
	}

#else


#endif

};