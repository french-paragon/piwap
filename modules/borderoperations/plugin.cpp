#include "plugin.h"

#include "operations/operationfactorymanager.h"
#include "operations/bevels.h"

namespace Piwap {
namespace BorderOperations {

Plugin::Plugin(QObject *parent) : QObject (parent)
{

}
void Plugin::loadPluginOperations(OperationFactoryManager* operationManager) {

	operationManager->insertFactory(new BevelOpFactory(operationManager));

}

} // namespace BorderOperations
} // namespace Piwap
