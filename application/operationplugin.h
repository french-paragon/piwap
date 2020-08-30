/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2020  Paragon<french.paragon@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef OPERATIONPLUGIN_H
#define OPERATIONPLUGIN_H

#include <QtPlugin>

namespace Piwap {

class OperationFactoryManager;

class OperationPlugin
{
public:
	virtual ~OperationPlugin();

	virtual void loadPluginOperations(OperationFactoryManager* operationManager) = 0;
};

} // namespace Piwap

#define APP_OP_PLUGIN_INTERFACE_NAME "Piwap.OperationPlugin"

Q_DECLARE_INTERFACE(Piwap::OperationPlugin, APP_OP_PLUGIN_INTERFACE_NAME)

#endif // OPERATIONPLUGIN_H
