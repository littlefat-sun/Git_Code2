#include "PluginsManager.h"


PluginsManager::PluginsManager(void)
{

}

PluginsManager::~PluginsManager(void)
{
}
void PluginsManager::initPlugins()
{
	m_customScene->addCommandListener(static_cast<ICommandListener*>(this));
}
void PluginsManager::onCommand(char* name)
{
	if (name=="sun")
	{
		if (NULL==m_adminDlg)
		{
			m_adminDlg=new GAdminDlg(m_customScene);
		}
	}
}