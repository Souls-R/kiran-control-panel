#include "accounts-global-info.h"
#include "accounts-interface.h"
#include "accounts-user-interface.h"
#include "log.h"
#include "config.h"

#include <unistd.h>
#include <QDBusObjectPath>
#include <QDBusPendingCall>
#include <QList>
#include <QMutex>
#include <QScopedPointer>

AccountsGlobalInfo::AccountsGlobalInfo(QObject *parent)
    : QObject(parent),
      m_accountsInterface(QDBusConnection::systemBus())
{
}

AccountsGlobalInfo::~AccountsGlobalInfo()
{
}

AccountsGlobalInfo *AccountsGlobalInfo::instance()
{
    static QMutex mutex;
    static QScopedPointer<AccountsGlobalInfo> pInst;

    if (Q_UNLIKELY(!pInst))
    {
        QMutexLocker locker(&mutex);
        if (pInst.isNull())
        {
            pInst.reset(new AccountsGlobalInfo);
        }
    }

    return pInst.data();
}

bool AccountsGlobalInfo::init()
{
    connect(&m_accountsInterface, &AccountsInterface::UserAdded, [this](const QDBusObjectPath &user) {
        addUserToMap(user);
    });
    connect(&m_accountsInterface, &AccountsInterface::UserDeleted, [this](const QDBusObjectPath &user) {
        deleteUserFromMap(user);
    });

    ///判断是否显示ROOT用户
    QSettings settings(CONFIG_FILE_PATH,QSettings::IniFormat);
    if(settings.status()!=QSettings::NoError)
    {
        LOG_WARNING_S() << "parse" << CONFIG_FILE_PATH << "failed!";
    }
    else
    {
        settings.beginGroup("Common");
        if(settings.contains("show-root"))
        {
            m_showRoot = settings.value("show-root").toBool();
        }
        settings.endGroup();
    };
    LOG_INFO("show root:%s",m_showRoot?"true":"false");

    ///加载账户
    QList<QDBusObjectPath> accounts;
    QDBusPendingReply<QList<QDBusObjectPath>> pendingReply;
    QList<QDBusObjectPath> objList;
    QList<QDBusObjectPath>::iterator objListIter;
    pendingReply = m_accountsInterface.GetNonSystemUsers();
    pendingReply.waitForFinished();
    if (pendingReply.isError())
    {
        LOG_WARNING_S() << "GetNonSystemUsers Error:"
                        << pendingReply.error();
        return false;
    }
    objList = pendingReply.value();

    if(m_showRoot)
    {
        auto getRootReply = m_accountsInterface.FindUserById(0);
        getRootReply.waitForFinished();
        if( !getRootReply.isError() )
        {
            objList.insert(0,getRootReply.value());
        }
        else
        {
            LOG_ERROR_S() << "cant find root by id:" << getRootReply.error();
        }
    }

    for (objListIter = objList.begin();
         objListIter != objList.end();
         objListIter++)
    {
        addUserToMap(*objListIter);
    }

    ///获取当前用户
    uid_t uid = getuid();
    QDBusPendingReply<QDBusObjectPath> findUserReply;
    findUserReply = m_accountsInterface.FindUserById(uid);
    findUserReply.waitForFinished();
    if (findUserReply.isError())
    {
        LOG_WARNING_S() << "FinduserById failed," << findUserReply.error();
    }
    else
    {
        UserInterface userInterface(findUserReply.value().path(), QDBusConnection::systemBus());
        m_curUserName = userInterface.user_name();
    }

    return true;
}

QList<QString> AccountsGlobalInfo::getUserList()
{
    QList<QString> userObjPathList;
    for (auto iter = m_usersMap.begin();
         iter != m_usersMap.end();
         iter++)
    {
        userObjPathList.append((*iter)->path());
    }
    return userObjPathList;
}

bool AccountsGlobalInfo::checkUserNameAvaliable(const QString &userName)
{
    bool isValid = true;

    for (auto &iter : m_usersMap)
    {
        if (iter->user_name() == userName)
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

QString AccountsGlobalInfo::getCurrentUser()
{
    return m_curUserName;
}

void AccountsGlobalInfo::addUserToMap(const QDBusObjectPath &user)
{
    if(m_usersMap.find(user.path()) != m_usersMap.end() )
    {
        return;
    }
    UserInterface *userInterface = new UserInterface(user.path(),
                                                     QDBusConnection::systemBus(),
                                                     this);
    connect(userInterface,
            &UserInterface::propertyChanged,
            this,
            &AccountsGlobalInfo::handlerPropertyChanged);
    m_usersMap.insert(user.path(),userInterface);
    emit UserAdded(user);
}

void AccountsGlobalInfo::deleteUserFromMap(const QDBusObjectPath &user)
{
    if(m_usersMap.find(user.path()) == m_usersMap.end())
    {
        return;
    }

    UserInterface *interface = m_usersMap.take(user.path());
    disconnect(interface,
               &UserInterface::propertyChanged,
               this,
               &AccountsGlobalInfo::handlerPropertyChanged);
    delete interface;

    emit UserDeleted(user);
}

void AccountsGlobalInfo::handlerPropertyChanged(QString userPath, QString propertyName, QVariant value)
{
    emit UserPropertyChanged(userPath, propertyName, value);
}
