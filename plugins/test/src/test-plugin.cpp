#include "test-plugin.h"
#include <QDebug>

int TestPlugin::init(KcpInterface* interface)
{
    m_controlPanelInterface = interface;

    for (int i = 0; i < 4; i++)
    {
        QSharedPointer<TestSubItem> pointer(new TestSubItem());
        QString subitemID = QString("TestSubItem_%1").arg(i);

        pointer->setID(subitemID);
        pointer->setName(subitemID);
        pointer->setIcon("firefox");
        pointer->setWeight(i);
        pointer->setCategory(i % 2 == 0 ? "about-system" : "audio");

        m_subitems.append(pointer);
    }

    m_timer.setInterval(3000);
    m_timer.setSingleShot(false);
    m_timer.start();
    connect(&m_timer, &QTimer::timeout, this, &TestPlugin::handleTimerTimeout);
    return 0;
}

void TestPlugin::uninit()
{
}

QVector<KcpPluginSubItemPtr> TestPlugin::getSubItems()
{
    return m_subitems;
}

#include <QDateTime>
void TestPlugin::handleTimerTimeout()
{
    for (int i = 0; i < 1; i++)
    {
        QSharedPointer<TestSubItem> pointer(new TestSubItem());
        int rand = qrand();
        QString subitemID = QString("%1_%2").arg(QDateTime::currentDateTime().toString()).arg(rand);
        pointer->setID(subitemID);
        pointer->setName(subitemID);
        pointer->setIcon("firefox");
        pointer->setWeight(10);
        pointer->setCategory(rand % 2 ? "about-system" : "audio");
        m_subitems.append(pointer);
    }
    m_controlPanelInterface->handlePluginSubItemChanged();
}