#ifndef __ACCOUNT_SUBITEM_H__
#define __ACCOUNT_SUBITEM_H__

#include <QDBusObjectPath>
#include <QObject>

#include "panel-interface.h"
#include "plugin-subitem-interface.h"

class KiranAccountManager;
class AccountSubItem : public QObject,
                       public KiranControlPanel::PluginSubitemInterface
{
    Q_OBJECT
public:
    AccountSubItem(KiranControlPanel::PanelInterface* interface, QObject* parent = nullptr);
    ~AccountSubItem();

    virtual bool eventFilter(QObject* watched, QEvent* event);

    QString getID() override;
    QString getName() override;
    QString getCategory() override;
    QString getDesc() override;
    QString getIcon() override;
    int getWeight() override;

    // 获取自定义搜索关键词
    //  QVector< 显示文本(已翻译)，搜索跳转标识ID >
    QVector<QPair<QString, QString>> getSearchKeys() override;

    QWidget* createWidget() override;
    bool jumpToSearchEntry(const QString& key) override;
    bool haveUnSavedOptions() override;

private:
    void handleUserAdded(const QDBusObjectPath& userObject);
    void handleUserDeleted(const QDBusObjectPath& userObject);

private:
    KiranControlPanel::PanelInterface* m_interface = nullptr;
    KiranAccountManager* m_subitemWidget = nullptr;
};

#endif