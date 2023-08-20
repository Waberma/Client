#ifndef PLUGIN_H
#define PLUGIN_H

#include "MenuInterface.h"
#include "myclient.h"
class Plugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "com.mysoft.Application.interface" FILE "Plugin.json")
public:
    explicit Plugin();
    ~Plugin();
private:

    // PluginInterface interface
public:
    QString getNamePlugin();
    QObject *getPluginWidget();
    QMenu *getMenu();
};
#endif // PLUGIN_H
