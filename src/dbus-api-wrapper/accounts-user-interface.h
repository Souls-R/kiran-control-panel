/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p AccountsUserInterface com.unikylin.Kiran.SystemDaemon.Accounts.User.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef ACCOUNTSUSERINTERFACE_H_1602294762
#define ACCOUNTSUSERINTERFACE_H_1602294762

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.unikylin.Kiran.SystemDaemon.Accounts.User
 */
class UserInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    {
        return "com.kylinsec.Kiran.SystemDaemon.Accounts.User";
    }

public:
    UserInterface(const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    virtual ~UserInterface();

    Q_PROPERTY(int account_type READ account_type)

    inline int account_type() const
    {
        return qvariant_cast<int>(property("account_type"));
    }

    Q_PROPERTY(bool automatic_login READ automatic_login)

    inline bool automatic_login() const
    {
        return qvariant_cast<bool>(property("automatic_login"));
    }

    Q_PROPERTY(QString email READ email)

    inline QString email() const
    {
        return qvariant_cast<QString>(property("email"));
    }

    Q_PROPERTY(QString home_directory READ home_directory)

    inline QString home_directory() const
    {
        return qvariant_cast<QString>(property("home_directory"));
    }

    Q_PROPERTY(QString icon_file READ icon_file)

    inline QString icon_file() const
    {
        return qvariant_cast<QString>(property("icon_file"));
    }

    Q_PROPERTY(QString language READ language)

    inline QString language() const
    {
        return qvariant_cast<QString>(property("language"));
    }

    Q_PROPERTY(bool locked READ locked)

    inline bool locked() const
    {
        return qvariant_cast<bool>(property("locked"));
    }

    Q_PROPERTY(QString password_hint READ password_hint)

    inline QString password_hint() const
    {
        return qvariant_cast<QString>(property("password_hint"));
    }

    Q_PROPERTY(int password_mode READ password_mode)

    inline int password_mode() const
    {
        return qvariant_cast<int>(property("password_mode"));
    }

    Q_PROPERTY(QString real_name READ real_name)

    inline QString real_name() const
    {
        return qvariant_cast<QString>(property("real_name"));
    }

    Q_PROPERTY(QString session READ session)

    inline QString session() const
    {
        return qvariant_cast<QString>(property("session"));
    }

    Q_PROPERTY(QString session_type READ session_type)

    inline QString session_type() const
    {
        return qvariant_cast<QString>(property("session_type"));
    }

    Q_PROPERTY(QString shell READ shell)

    inline QString shell() const
    {
        return qvariant_cast<QString>(property("shell"));
    }

    Q_PROPERTY(bool system_account READ system_account)

    inline bool system_account() const
    {
        return qvariant_cast<bool>(property("system_account"));
    }

    Q_PROPERTY(qulonglong uid READ uid)

    inline qulonglong uid() const
    {
        return qvariant_cast<qulonglong>(property("uid"));
    }

    Q_PROPERTY(QString user_name READ user_name)

    inline QString user_name() const
    {
        return qvariant_cast<QString>(property("user_name"));
    }

    Q_PROPERTY(QString x_session READ x_session)

    inline QString x_session() const
    {
        return qvariant_cast<QString>(property("x_session"));
    }

    Q_PROPERTY(int auth_modes READ auth_modes)

    inline int auth_modes() const
    {
        return qvariant_cast<int>(property("auth_modes"));
    }

public Q_SLOTS:  // METHODS
    inline QDBusPendingReply<qlonglong, qlonglong, qlonglong, qlonglong, qlonglong, qlonglong>
    GetPasswordExpirationPolicy()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetPasswordExpirationPolicy"), argumentList);
    }

    inline QDBusReply<qlonglong>
    GetPasswordExpirationPolicy(qlonglong &last_change_time, qlonglong &min_days_between_changes,
                                qlonglong &max_days_between_changes, qlonglong &days_to_warn,
                                qlonglong &days_after_expiration_until_lock)
    {
        QList<QVariant> argumentList;
        QDBusMessage    reply = callWithArgumentList(QDBus::Block, QLatin1String("GetPasswordExpirationPolicy"),
                                                  argumentList);
        if (reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() == 6)
        {
            last_change_time                 = qdbus_cast<qlonglong>(reply.arguments().at(1));
            min_days_between_changes         = qdbus_cast<qlonglong>(reply.arguments().at(2));
            max_days_between_changes         = qdbus_cast<qlonglong>(reply.arguments().at(3));
            days_to_warn                     = qdbus_cast<qlonglong>(reply.arguments().at(4));
            days_after_expiration_until_lock = qdbus_cast<qlonglong>(reply.arguments().at(5));
        }
        return reply;
    }

    inline QDBusPendingReply<> SetAccountType(int account_type)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(account_type);
        return asyncCallWithArgumentList(QLatin1String("SetAccountType"), argumentList);
    }

    inline QDBusPendingReply<> SetAutomaticLogin(bool enabled)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(enabled);
        return asyncCallWithArgumentList(QLatin1String("SetAutomaticLogin"), argumentList);
    }

    inline QDBusPendingReply<> SetEmail(const QString &email)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(email);
        return asyncCallWithArgumentList(QLatin1String("SetEmail"), argumentList);
    }

    inline QDBusPendingReply<> SetHomeDirectory(const QString &homedir)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(homedir);
        return asyncCallWithArgumentList(QLatin1String("SetHomeDirectory"), argumentList);
    }

    inline QDBusPendingReply<> SetIconFile(const QString &filename)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(filename);
        return asyncCallWithArgumentList(QLatin1String("SetIconFile"), argumentList);
    }

    inline QDBusPendingReply<> SetLanguage(const QString &language)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(language);
        return asyncCallWithArgumentList(QLatin1String("SetLanguage"), argumentList);
    }

    inline QDBusPendingReply<> SetLocked(bool locked)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(locked);
        return asyncCallWithArgumentList(QLatin1String("SetLocked"), argumentList);
    }

    inline QDBusPendingReply<> SetPassword(const QString &password, const QString &hint)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(password) << QVariant::fromValue(hint);
        return asyncCallWithArgumentList(QLatin1String("SetPassword"), argumentList);
    }

    inline QDBusPendingReply<> SetPasswordHint(const QString &hint)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(hint);
        return asyncCallWithArgumentList(QLatin1String("SetPasswordHint"), argumentList);
    }

    inline QDBusPendingReply<> SetPasswordMode(int mode)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(mode);
        return asyncCallWithArgumentList(QLatin1String("SetPasswordMode"), argumentList);
    }

    inline QDBusPendingReply<> SetRealName(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QLatin1String("SetRealName"), argumentList);
    }

    inline QDBusPendingReply<> SetSession(const QString &session)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(session);
        return asyncCallWithArgumentList(QLatin1String("SetSession"), argumentList);
    }

    inline QDBusPendingReply<> SetSessionType(const QString &session_type)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(session_type);
        return asyncCallWithArgumentList(QLatin1String("SetSessionType"), argumentList);
    }

    inline QDBusPendingReply<> SetShell(const QString &shell)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(shell);
        return asyncCallWithArgumentList(QLatin1String("SetShell"), argumentList);
    }

    inline QDBusPendingReply<> SetUserName(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QLatin1String("SetUserName"), argumentList);
    }

    inline QDBusPendingReply<> SetXSession(const QString &x_session)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(x_session);
        return asyncCallWithArgumentList(QLatin1String("SetXSession"), argumentList);
    }

    inline QDBusPendingReply<> AddAuthItem(int mode, const QString &name, const QString &data_id)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(mode) << QVariant::fromValue(name) << QVariant::fromValue(data_id);
        return asyncCallWithArgumentList(QLatin1String("AddAuthItem"), argumentList);
    }

    inline QDBusPendingReply<> DelAuthItem(int mode, const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(mode) << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QLatin1String("DelAuthItem"), argumentList);
    }

    inline QDBusPendingReply<QString> GetAuthItems(int mode)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(mode);
        return asyncCallWithArgumentList(QLatin1String("GetAuthItems"), argumentList);
    }

    inline QDBusPendingReply<> EnableAuthMode(int mode, bool enabled)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(mode) << QVariant::fromValue(enabled);
        return asyncCallWithArgumentList(QLatin1String("EnableAuthMode"), argumentList);
    }

private Q_SLOTS:
    void handlePropertiesChanged(QDBusMessage msg);

Q_SIGNALS:  // SIGNALS
    void propertyChanged(QString path, QString propertyName, QVariant value);
    void AuthItemChanged(int mode);
};

namespace com
{
namespace kylinsec
{
namespace Kiran
{
namespace SystemDaemon
{
namespace Accounts
{
typedef ::UserInterface UserInterface;
}
}  // namespace SystemDaemon
}  // namespace Kiran
}  // namespace kylinsec
}  // namespace com
#endif
