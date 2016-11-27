#include "CustomScene.h"


CustomScene::CustomScene(void)
{
}
CustomScene::~CustomScene(void)
{
}
void CustomScene::addCommandListener(ICommandListener* cmdListener)
{
	vector<ICommandListener*>::iterator it;
	for (it=m_CommandListenerVector.begin();it!=m_CommandListenerVector.end();it++)
	{
		if (*it==cmdListener)
		{
			continue;
		}
		else
		{
			m_CommandListenerVector.push_back(cmdListener);
		}
	}
}
void CustomScene::command(char* name)
{
	vector<ICommandListener*>::iterator it;
	for (it=m_CommandListenerVector.begin();it!=m_CommandListenerVector.end();it++)
	{
		(*it)->onCommand(name);
	}
}
