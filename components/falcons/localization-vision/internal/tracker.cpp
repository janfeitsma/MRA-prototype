#include "tracker.hpp"

using namespace MRA::FalconsLocalizationVision;


static int s_id = 0;

Tracker::Tracker(Params const &params)
{
    id = s_id++;
}

Tracker::Tracker(Params const &params, TrackerState const &st)
{
    this->params = params;
    guess = st.pose();
    step = params.solver().actionradius();
    creation = st.creation();
    lastActive = st.lastactive();
    id = st.id();
}

float Tracker::confidence() const
{
    return 1.0; // TODO
}

