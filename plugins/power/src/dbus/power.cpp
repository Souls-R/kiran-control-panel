/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.kylinsec.Kiran.SessionDaemon.Power.xml -p Power
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "power.h"
#include <kiran-session-daemon/power-i.h>
/*
 * Implementation of interface class PowerInterface
 */

PowerInterface *PowerInterface::instance()
{
    static QMutex mutex;
    static QScopedPointer<PowerInterface> pInst;

    if(Q_UNLIKELY(!pInst)){
        QMutexLocker locker(&mutex);
        if(pInst.isNull()){
            pInst.reset(new PowerInterface(QDBusConnection::sessionBus()));
        }
    }

    return pInst.data();
}

PowerInterface::PowerInterface(const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface("com.kylinsec.Kiran.SessionDaemon", "/com/kylinsec/Kiran/SessionDaemon/Power", staticInterfaceName(), connection, parent)
{
    qRegisterMetaType<IdleAction>("IdleAction");
    qDBusRegisterMetaType<IdleAction>();
}

PowerInterface::~PowerInterface()
{

}