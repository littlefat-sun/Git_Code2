#pragma once
#include "commandlistener.h"
#include "GAdminDlg.h"

class PluginsManager : public ICommandListener
{
public:
	PluginsManager(void);
	~PluginsManager(void);
	void initPlugins();
	void onCommand(char* name);
private:
	GAdminDlg* m_adminDlg;
	CustomScene* m_customScene;
};

