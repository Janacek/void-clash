/********************************************************************
**
**              ModuleFactory.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_MODULE_FACTORY
#define VOID_CLASH_MODULE_FACTORY

#include <map>
#include "SingletonBase.hpp"
#include "GraphicHelper.hpp"
#include "Module.hpp"

namespace VoidClashGraphics
{
    class CModuleFactory
    {
    public:
        CModuleFactory(void);
        ~CModuleFactory(void);

        AModule* createModule(gObjectType type);
    };
}

typedef SingletonBase<VoidClashGraphics::CModuleFactory> ModuleFactorySingleton;
#define S_ModuleFactory ModuleFactorySingleton::self()

#endif /* end of include guard: VOID_CLASH_MODULE_FACTORY */
