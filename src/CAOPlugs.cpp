#include "CAOPlugs.hpp"

Plugin *pluginInstance;

void init(rack::Plugin *p) {
    pluginInstance = p;

    p->addModel(modelCLatch);
    p->addModel(modelDualCVShift);
    p->addModel(modelDualVarShift);
    p->addModel(modelDualQuadOsc);
    p->addModel(modelAddAll);
    p->addModel(modelDualPBFixer);
}
