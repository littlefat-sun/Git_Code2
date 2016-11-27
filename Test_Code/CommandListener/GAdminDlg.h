#pragma once
#include <QDialog>
#include "commandlistener.h"
#include "CustomScene.h"


class GAdminDlg : public QDialog 
{
public:
	GAdminDlg(CustomScene* cs);
	~GAdminDlg(void);
private:
	CustomScene* m_customScene;
};

