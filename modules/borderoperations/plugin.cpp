#include "plugin.h"

#include "operations/operationfactorymanager.h"
#include "operations/bevels.h"
#include "operations/dropshadow.h"

namespace Piwap {
namespace BorderOperations {

Plugin::Plugin(QObject *parent) : QObject (parent)
{

}
void Plugin::loadPluginOperations(OperationFactoryManager* operationManager) {

    operationManager->insertFactory(new BevelOpFactory(operationManager));
    operationManager->insertFactory(new DropShadowOpFactory(operationManager));

}

} // namespace BorderOperations
} // namespace Piwap
