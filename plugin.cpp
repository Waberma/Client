#include "plugin.h"

Plugin::Plugin()
{
}

Plugin::~Plugin()
{
}

QString Plugin::getNamePlugin()
{
    return "Клиент";
}

QObject *Plugin::getPluginWidget()
{
    MyClient *widg = new MyClient();
    return qobject_cast<QObject *>(widg);
}

QMenu *Plugin::getMenu()
{
}
