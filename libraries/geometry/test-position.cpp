// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
using namespace ::testing;

// System under test:
#include "position.hpp"
using namespace MRA::Geometry;


TEST(MRAGeometryPositionTest, constructorNoArgs)
{
    // Arrange
    auto p = Position();

    // Act

    // Assert
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
    EXPECT_EQ(p.z, 0.0);
    EXPECT_EQ(p.rx, 0.0);
    EXPECT_EQ(p.ry, 0.0);
    EXPECT_EQ(p.rz, 0.0);
}

TEST(MRAGeometryPositionTest, constructorSixArgs)
{
    // Arrange
    auto p = Position(1.0, 2.0, 13.0, 4.0, 5.0, 6.0);

    // Act

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 13.0);
    EXPECT_FLOAT_EQ(p.rx, -2.2831852); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -1.2831852); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.2831852); // wrapped
}

TEST(MRAGeometryPositionTest, constructorProtobufComplete)
{
    // Arrange
    auto pd = MRA::Datatypes::Pose();
    pd.set_x(1.0);
    pd.set_y(2.0);
    pd.set_z(13.0);
    pd.set_rx(4.0);
    pd.set_ry(5.0);
    pd.set_rz(6.0);

    // Act
    auto p = Position(pd);

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 13.0);
    EXPECT_FLOAT_EQ(p.rx, -2.2831852); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -1.2831852); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.2831852); // wrapped
}

TEST(MRAGeometryPositionTest, constructorProtobufIncomplete)
{
    // Arrange
    auto pd = MRA::Datatypes::Pose();
    pd.set_y(2.0);
    pd.set_rz(6.0);

    // Act
    auto p = Position(pd);

    // Assert
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 0.0);
    EXPECT_EQ(p.rx, 0.0);
    EXPECT_EQ(p.ry, 0.0);
    EXPECT_FLOAT_EQ(p.rz, -0.2831852); // wrapped
}

TEST(MRAGeometryPositionTest, constructorFromPose)
{
    // Arrange
    auto pp = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    auto p = Position(pp);

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_FLOAT_EQ(p.rx, -2.2831852); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -1.2831852); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.2831852); // wrapped
}

TEST(MRAGeometryPositionTest, constructorWrapping)
{
    // Arrange
    auto p = Position(111.0, 222.0, 333.0, 444.0, 555.0, 666.0);

    // Act

    // Assert
    EXPECT_EQ(p.x, 111.0);
    EXPECT_EQ(p.y, 222.0);
    EXPECT_EQ(p.z, 333.0);
    EXPECT_FLOAT_EQ(p.rx, -2.1061568); // wrapped
    EXPECT_FLOAT_EQ(p.ry, 2.0796931); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.017642561); // wrapped
}

TEST(MRAGeometryPositionTest, size)
{
    // Arrange
    // (make use of 2 well-known pythagorean triples)
    auto p = Position(-3.0, 4.0, -12.0, 444.0, 555.0, 666.0);

    // Act
    float v = p.size();

    // Assert
    EXPECT_EQ(v, 13.0);
}

TEST(MRAGeometryPositionTest, operatorPlus)
{
    // Arrange
    auto p1 = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    auto p2 = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    Position p = p1 + p2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_FLOAT_EQ(p.rx, 1.7168146); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -2.5663706); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.5663706); // wrapped
    EXPECT_EQ(p1.x, 1.0);
    EXPECT_EQ(p2.x, 1.0);
}

TEST(MRAGeometryPositionTest, operatorPlusAssign)
{
    // Arrange
    auto p = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    auto p2 = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    p += p2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_FLOAT_EQ(p.rx, 1.7168146); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -2.5663707); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.5663707); // wrapped
    EXPECT_EQ(p2.x, 1.0);
}

TEST(MRAGeometryPositionTest, operatorMinus)
{
    // Arrange
    auto p1 = Position(4.0, 6.0, 8.0, -1.0, -2.0, -3.0);
    auto p2 = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    Position p = p1 - p2;

    // Assert
    EXPECT_EQ(p.x, 3.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 5.0);
    EXPECT_FLOAT_EQ(p.rx, 1.2831854); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -0.7168147); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -2.7168148); // wrapped
    EXPECT_EQ(p1.x, 4.0);
    EXPECT_EQ(p2.x, 1.0);
}

TEST(MRAGeometryPositionTest, operatorMinusAssign)
{
    // Arrange
    auto p = Position(4.0, 6.0, 8.0, -1.0, -2.0, -3.0);
    auto p2 = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    p -= p2;

    // Assert
    EXPECT_EQ(p.x, 3.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 5.0);
    EXPECT_FLOAT_EQ(p.rx, 1.2831854); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -0.7168147); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -2.7168148); // wrapped
    EXPECT_EQ(p2.x, 1.0);
}

TEST(MRAGeometryPositionTest, operatorTimesScalar)
{
    // Arrange
    auto p1 = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    Position p = p1 * 2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_FLOAT_EQ(p.rx, 1.7168146); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -2.5663707); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.5663707); // wrapped
    EXPECT_EQ(p1.x, 1.0);
}

TEST(MRAGeometryPositionTest, operatorTimesScalarAssign)
{
    // Arrange
    auto p = Position(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    p *= 2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_FLOAT_EQ(p.rx, 1.7168146); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -2.5663707); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.5663707); // wrapped
}

TEST(MRAGeometryPositionTest, operatorDivideScalar)
{
    // Arrange
    auto p1 = Position(2.0, 4.0, 6.0, 8.0, 10.0, 12.0);

    // Act
    Position p = p1 / 2;

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_FLOAT_EQ(p.rx, 0.8584073); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -1.283185); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.2831853); // wrapped
    EXPECT_EQ(p1.x, 2.0);
}

TEST(MRAGeometryPositionTest, operatorDivideScalarAssign)
{
    // Arrange
    auto p = Position(2.0, 4.0, 6.0, 8.0, 10.0, 12.0);

    // Act
    p /= 2;

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_FLOAT_EQ(p.rx, 0.8584073); // wrapped
    EXPECT_FLOAT_EQ(p.ry, -1.283185); // wrapped
    EXPECT_FLOAT_EQ(p.rz, -0.2831853); // wrapped
}

TEST(MRAGeometryPositionTest, transformRcsToFcsNominal)
{
    // Arrange
    auto pf = Position();
    auto pr = Position();

    // Act
    Position p = pf.transformRcsToFcs(pr);

    // Assert
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
    EXPECT_EQ(p.z, 0.0);
    EXPECT_EQ(p.rx, 0.0);
    EXPECT_EQ(p.ry, 0.0);
    EXPECT_EQ(p.rz, 0.0);
    EXPECT_EQ(pf.x, 0.0);
    EXPECT_EQ(pr.x, 0.0);
}

TEST(MRAGeometryPositionTest, transformFcsToRcsNominal)
{
    // Arrange
    auto pf = Position();
    auto pr = Position();

    // Act
    Position p = pf.transformFcsToRcs(pr);

    // Assert
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
    EXPECT_EQ(p.z, 0.0);
    EXPECT_EQ(p.rx, 0.0);
    EXPECT_EQ(p.ry, 0.0);
    EXPECT_EQ(p.rz, 0.0);
    EXPECT_EQ(pf.x, 0.0);
    EXPECT_EQ(pr.x, 0.0);
}

TEST(MRAGeometryPositionTest, transformRcsToFcsBasic1)
{
    // Arrange
    auto pf = Position(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    auto pr = Position();

    // Act
    Position p = pf.transformRcsToFcs(pr);

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_EQ(p.rx, 0.1);
    EXPECT_EQ(p.ry, 0.2);
    EXPECT_EQ(p.rz, 0.3);
    EXPECT_EQ(pf.x, 1.0);
    EXPECT_EQ(pr.x, 0.0);
}

TEST(MRAGeometryPositionTest, transformFcsToRcsBasic1)
{
    // Arrange
    auto pf = Position(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    auto pr = Position();

    // Act
    Position p = pf.transformFcsToRcs(pr);

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_EQ(p.rx, 0.1);
    EXPECT_EQ(p.ry, 0.2);
    EXPECT_EQ(p.rz, 0.3);
    EXPECT_EQ(pf.x, 1.0);
    EXPECT_EQ(pr.x, 0.0);
}

TEST(MRAGeometryPositionTest, transformRcsToFcsFull3D)
{
    // Arrange
    auto pf = Position(); // TODO nonzero
    auto pr = Position(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);

    // Act
    Position p = pf.transformRcsToFcs(pr);

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    //EXPECT_EQ(p.z, 3.0); // TODO make transformations fully 3d
    //EXPECT_EQ(p.rx, 0.1); // TODO make transformations fully 3d
    //EXPECT_EQ(p.ry, 0.2); // TODO make transformations fully 3d
    EXPECT_EQ(p.rz, 0.3);
    EXPECT_EQ(pf.x, 1.0);
    EXPECT_EQ(pr.x, 1.0);
}

TEST(MRAGeometryPositionTest, transformFcsToRcsKickoff)
{
    // Arrange
    // kickoff situation: two robots facing eachother around center line y=0, how does one robot see the other in his own coordinate system
    auto pf = Position(-2.0, 0.0, 0.0, 0.0, 0.0, -0.4 * M_PI); // teammate
    auto pr = Position(1.0, 0.1, 0.0, 0.0, 0.0, 0.5 * M_PI); // self

    // Act
    Position p = pf.transformFcsToRcs(pr); // transform teammate w.r.t. self

    // Assert
    EXPECT_FLOAT_EQ(p.x, -0.1);
    EXPECT_FLOAT_EQ(p.y, 3.0); // distance 3 meters
    EXPECT_FLOAT_EQ(p.rz, -0.9 * M_PI); // facing backwards
}

TEST(MRAGeometryPositionTest, addRcsToFcs)
{
    // Arrange
    auto pf = Position(1.0, 0.1, 0.0, 0.0, 0.0, 0.5 * M_PI);
    auto pr = Position(0.0, 2.0, 0.0, 0.0, 0.0, -M_PI);

    // Act
    Position p = pf.addRcsToFcs(pr);

    // Assert
    EXPECT_FLOAT_EQ(p.x, -1.0);
    EXPECT_FLOAT_EQ(p.y, 0.1);
    EXPECT_FLOAT_EQ(p.rz, -0.5 * M_PI);
}

TEST(MRAGeometryPositionTest, faceTowards)
{
    // Arrange
    auto pf = Position(1.0, 1.0);
    auto pr = Position(0.0, 0.0);

    // Act
    Position p = pr.faceTowards(pf);

    // Assert
    EXPECT_FLOAT_EQ(p.x, 0.0);
    EXPECT_FLOAT_EQ(p.y, 0.0);
    EXPECT_FLOAT_EQ(p.rz, 0.25 * M_PI);
}

TEST(MRAGeometryPositionTest, faceAwayFrom)
{
    // Arrange
    auto pf = Position(1.0, 1.0);
    auto pr = Position(0.0, 0.0);

    // Act
    Position p = pr.faceAwayFrom(pf);

    // Assert
    EXPECT_FLOAT_EQ(p.x, 0.0);
    EXPECT_FLOAT_EQ(p.y, 0.0);
    EXPECT_FLOAT_EQ(p.rz, -0.75 * M_PI);
}


int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

