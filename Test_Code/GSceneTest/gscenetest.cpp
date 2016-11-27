#include "gscenetest.h"

GSceneTest::GSceneTest(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_customScene=new CustomScene();
	m_customScene->command("sun");

}

GSceneTest::~GSceneTest()
{

}
