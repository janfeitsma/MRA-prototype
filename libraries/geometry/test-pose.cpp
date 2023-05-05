// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
using namespace ::testing;

// System under test:
#include "pose.hpp"
using namespace MRA::Geometry;


TEST(MRAGeometryPoseTest, constructorNoArgs)
{
    // Arrange
    auto p = Pose();

    // Act

    // Assert
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
    EXPECT_EQ(p.z, 0.0);
    EXPECT_EQ(p.rx, 0.0);
    EXPECT_EQ(p.ry, 0.0);
    EXPECT_EQ(p.rz, 0.0);
}

TEST(MRAGeometryPoseTest, constructorSixArgs)
{
    // Arrange
    auto p = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_EQ(p.rx, 4.0);
    EXPECT_EQ(p.ry, 5.0);
    EXPECT_EQ(p.rz, 6.0);
}

TEST(MRAGeometryPoseTest, constructorProtobufComplete)
{
    // Arrange
    auto pd = MRA::Datatypes::Pose();
    pd.set_x(1.0);
    pd.set_y(2.0);
    pd.set_z(3.0);
    pd.set_rx(4.0);
    pd.set_ry(5.0);
    pd.set_rz(6.0);

    // Act
    auto p = Pose(pd);

    // Assert
    EXPECT_EQ(p.x, 1.0);
    EXPECT_EQ(p.y, 2.0);
    EXPECT_EQ(p.z, 3.0);
    EXPECT_EQ(p.rx, 4.0);
    EXPECT_EQ(p.ry, 5.0);
    EXPECT_EQ(p.rz, 6.0);
}

TEST(MRAGeometryPoseTest, constructorProtobufIncomplete)
{
    // Arrange
    auto pd = MRA::Datatypes::Pose();
    pd.set_y(2.0);
    pd.set_rz(6.0);

    // Act
    auto p = Pose(pd);

    // Assert
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 2.0);
(p.z, 0.0);
    EXPECT_EQ(p.rx, 0.0);
    EXPECT_EQ(p.ry, 0.0);
    EXPECT_EQ(p.rz, 6.0);
}

TEST(MRAGeometryPoseTest, constructorNoWrapping)
{
    // Arrange
    auto p = Pose(111.0, 222.0, 333.0, 444.0, 555.0, 666.0);

    // Act

    // Assert
    EXPECT_EQ(p.x, 111.0);
    EXPECT_EQ(p.y, 222.0);
    EXPECT_EQ(p.z, 333.0);
    EXPECT_EQ(p.rx, 444.0);
    EXPECT_EQ(p.ry, 555.0);
    EXPECT_EQ(p.rz, 666.0);
}

TEST(MRAGeometryPoseTest, size)
{
    // Arrange
    // (make use of 2 well-known pythagorean triples)
    auto p = Pose(-3.0, 4.0, -12.0, 444.0, 555.0, 666.0);

    // Act
    float v = p.size();

    // Assert
    EXPECT_EQ(v, 13.0);
}

TEST(MRAGeometryPoseTest, operatorPlus)
{
    // Arrange
    auto p1 = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    auto p2 = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    auto p = p1 + p2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_EQ(p.rx, 8.0);
    EXPECT_EQ(p.ry, 10.0);
    EXPECT_EQ(p.rz, 12.0);
    EXPECT_EQ(p1.x, 1.0);
    EXPECT_EQ(p2.x, 1.0);
}

TEST(MRAGeometryPoseTest, operatorPlusAssign)
{
    // Arrange
    auto p = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    auto p2 = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    p += p2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_EQ(p.rx, 8.0);
    EXPECT_EQ(p.ry, 10.0);
    EXPECT_EQ(p.rz, 12.0);
    EXPECT_EQ(p2.x, 1.0);
}

TEST(MRAGeometryPoseTest, operatorTimesScalar)
{
    // Arrange
    auto p1 = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    auto p = p1 * 2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_EQ(p.rx, 8.0);
    EXPECT_EQ(p.ry, 10.0);
    EXPECT_EQ(p.rz, 12.0);
    EXPECT_EQ(p1.x, 1.0);
}

TEST(MRAGeometryPoseTest, operatorTimesScalarAssign)
{
    // Arrange
    auto p = Pose(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Act
    p *= 2;

    // Assert
    EXPECT_EQ(p.x, 2.0);
    EXPECT_EQ(p.y, 4.0);
    EXPECT_EQ(p.z, 6.0);
    EXPECT_EQ(p.rx, 8.0);
    EXPECT_EQ(p.ry, 10.0);
    EXPECT_EQ(p.rz, 12.0);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

