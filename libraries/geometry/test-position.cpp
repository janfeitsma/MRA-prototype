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

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

