#ifndef H_CommandConsts
#define H_CommandConsts
class CommandConsts
{
public:
	static char * LEADER_KEY;
	static char * COMMAND_KEY;
	static char * BROADCAST_KEY;
	static char * TARGET_KEY;
	static char * ABILITYSET_KEY;
	static char * INDEX_KEY;
	static char * RANGE_KEY;
};
char * CommandConsts::LEADER_KEY = "l";
char * CommandConsts::COMMAND_KEY = "c";
char * CommandConsts::BROADCAST_KEY = "b";
char * CommandConsts::TARGET_KEY = "t";
char * CommandConsts::ABILITYSET_KEY = "a";
char * CommandConsts::INDEX_KEY = "i";
char * CommandConsts::RANGE_KEY = "r";
#endif