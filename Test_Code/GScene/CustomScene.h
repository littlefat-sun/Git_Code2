#pragma once
#include <vector>
#include "commandlistener.h"
using namespace std;

class CustomScene
{
public:
	CustomScene(void);
	~CustomScene(void);
	void addCommandListener(ICommandListener* cmdListener);
	void command(char* name);
private:
	vector<ICommandListener*> m_CommandListenerVector;

};

