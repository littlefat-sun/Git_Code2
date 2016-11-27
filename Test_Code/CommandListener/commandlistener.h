#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

//#include "commandlistener_global.h"

class ICommandListener
{
public:

	virtual void onCommand(char* name) = 0;

private:

};

#endif // COMMANDLISTENER_H
