#include "CAOPlugs.hpp"

Plugin *pluginInstance;

void init(rack::Plugin *p) {
    pluginInstance = p;

    p->addModel(modelDualCVShift);
    p->addModel(modelDualVarShift);
    p->addModel(modelAddAll);
}
