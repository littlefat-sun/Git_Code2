#ifndef GSCENETEST_H
#define GSCENETEST_H

#include <QtGui/QMainWindow>
#include "ui_gscenetest.h"
#include "CustomScene.h"

class GSceneTest : public QMainWindow
{
	Q_OBJECT

public:
	GSceneTest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GSceneTest();

private:
	Ui::GSceneTestClass ui;
	CustomScene* m_customScene;
};

#endif // GSCENETEST_H
