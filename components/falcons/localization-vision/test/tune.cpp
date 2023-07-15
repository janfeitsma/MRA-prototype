#include "FalconsLocalizationVision.hpp"
#include "tuningtool.hpp"



int main(int argc, char **argv)
{
    MRA::FalconsLocalizationVision::Params p = MRA::FalconsLocalizationVision::defaultParams();
    // TODO: option to call loadParams(filename)?

    // setup tuning tool
    // only solver settings, no sliders for field dimensions etc
    TuningTool t(p.solver());
    t.run();
    return 0;
}

