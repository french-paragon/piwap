#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include "application/operationplugin.h"

namespace Piwap {
namespace BorderOperations {

class Plugin : public QObject, public Piwap::OperationPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID APP_OP_PLUGIN_INTERFACE_NAME)
	Q_INTERFACES(Piwap::OperationPlugin)
public:
	Plugin(QObject * parent = nullptr);

	void loadPluginOperations(OperationFactoryManager* operationManager);
};

} // namespace BorderOperations
} // namespace Piwap

#endif // PLUGIN_H
