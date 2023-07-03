#include "guessing.hpp"

using namespace MRA::FalconsLocalizationVision;


Guesser::Guesser(Params const &params)
{
    _config = params.solver().guessing();
    _floorMaxX = 0.5 * params.model().b();
    _floorMaxY = 0.5 * params.model().a();
}

std::optional<MRA::Datatypes::Point> Guesser::createRandomGuess(std::vector<MRA::Geometry::Point> const &pointsToAvoid) const
{
    std::optional<MRA::Datatypes::Point> result;
    for (int iAttempt = 0; iAttempt < _config.random().maxtries(); ++iAttempt)
    {
        MRA::Geometry::Point candidate(
            (2.0 * rand() / RAND_MAX - 1.0) * _floorMaxX,
            (2.0 * rand() / RAND_MAX - 1.0) * _floorMaxY);
        bool tooClose = false;
        for (auto const &pt: pointsToAvoid)
        {
            tooClose |= ((candidate - pt).size() < _config.random().exclusionradius());
        }
        if (!tooClose)
        {
            result = (MRA::Datatypes::Point)candidate;
        }
    }
    return result;
}

std::vector<MRA::Datatypes::Circle> Guesser::run(std::vector<MRA::Geometry::Point> pointsToAvoid, bool initial) const
{
    // note: pointsToAvoid is a vector copy that might be growing
    auto gvec = _config.structural();
    std::vector<MRA::Datatypes::Circle> result(gvec.begin(), gvec.end());
    // initial guesses only at the very first tick
    if (initial)
    {
        gvec = _config.initial();
        std::copy(gvec.begin(), gvec.end(), std::back_inserter(result));
    }
    // register result so far as avoidance
    for (auto const &circle: result)
    {
        pointsToAvoid.push_back(MRA::Geometry::Point(circle.center()));
    }
    // add random guesses?
    auto rgParams = _config.random();
    bool doRandom = rgParams.count() && rgParams.searchradius() && rgParams.maxtries();
    for (int iRandom = 0; iRandom < doRandom * rgParams.count(); ++iRandom)
    {
        // draw a random pos within the field range
        std::optional<MRA::Datatypes::Point> pt = createRandomGuess(pointsToAvoid);
        if (pt)
        {
            // store
            MRA::Datatypes::Circle gc;
            gc.mutable_center()->set_x(pt->x());
            gc.mutable_center()->set_y(pt->y());
            gc.set_radius(rgParams.searchradius());
            result.push_back(gc);
            pointsToAvoid.push_back(*pt);
        }
    }
    return result;
}

