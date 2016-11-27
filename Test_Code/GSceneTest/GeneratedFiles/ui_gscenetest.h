/********************************************************************************
** Form generated from reading UI file 'gscenetest.ui'
**
** Created: Tue Nov 1 23:56:31 2016
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GSCENETEST_H
#define UI_GSCENETEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GSceneTestClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GSceneTestClass)
    {
        if (GSceneTestClass->objectName().isEmpty())
            GSceneTestClass->setObjectName(QString::fromUtf8("GSceneTestClass"));
        GSceneTestClass->resize(600, 400);
        menuBar = new QMenuBar(GSceneTestClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        GSceneTestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GSceneTestClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        GSceneTestClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GSceneTestClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        GSceneTestClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GSceneTestClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GSceneTestClass->setStatusBar(statusBar);

        retranslateUi(GSceneTestClass);

        QMetaObject::connectSlotsByName(GSceneTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *GSceneTestClass)
    {
        GSceneTestClass->setWindowTitle(QApplication::translate("GSceneTestClass", "GSceneTest", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GSceneTestClass: public Ui_GSceneTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GSCENETEST_H
