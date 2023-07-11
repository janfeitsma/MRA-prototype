#ifndef _MRA_FALCONS_LOCALIZATION_VISION_GUESSING_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_GUESSING_HPP

#include "datatypes.hpp"
#include "tracker.hpp"

namespace MRA::FalconsLocalizationVision
{

class Guesser
{
public:
    Guesser(Params const &params);
    ~Guesser() {};

    void run(std::vector<Tracker> &trackers, bool initial = true) const;

private:
    GuessingParams _config;
    float _floorMaxX;
    float _floorMaxY;

    std::optional<MRA::Datatypes::Point> createRandomGuess(std::vector<MRA::Geometry::Point> const &pointsToAvoid) const;

}; // class Guesser

} // namespace MRA::FalconsLocalizationVision

#endif

