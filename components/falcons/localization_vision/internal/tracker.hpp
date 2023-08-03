#ifndef _MRA_FALCONS_LOCALIZATION_VISION_TRACKER_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_TRACKER_HPP

#include "datatypes.hpp"
#include "geometry.hpp"

namespace MRA::FalconsLocalizationVision
{

class Tracker
{
public:
    Tracker(Params const &params);
    Tracker(Params const &params, TrackerState const &st);
    ~Tracker() {};

    Params params;

    // data loaded from TrackerState
    int id;
    google::protobuf::Timestamp creation;
    google::protobuf::Timestamp lastActive;
    MRA::Geometry::Pose guess;
    MRA::Geometry::Pose step;

    // fit result, which is filled in by FitAlgorithm::run
    MRA::Geometry::Pose fitResult;
    bool fitValid;
    float fitScore;

    // score heuristic: combine fitScore, age and freshness
    float confidence() const;
    bool operator<(Tracker const &other) { return confidence() > other.confidence(); }

}; // class Tracker

} // namespace MRA::FalconsLocalizationVision

#endif

