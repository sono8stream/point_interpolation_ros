#pragma once
#include <iostream>
#include <vector>

#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

#include <Open3D/Open3D.h>

#include "../models/lidarParams.cpp"

using namespace std;
using namespace open3d;

vector<vector<double>> grid_entire(geometry::PointCloud &pcd, LidarParams &lidarParams)
{
    vector<vector<double>> full_grid(lidarParams.height, vector<double>(lidarParams.width, 0));
    for (int i = 0; i < pcd.points_.size(); i++)
    {
        double x = pcd.points_[i][0];
        double y = pcd.points_[i][1];
        double z = pcd.points_[i][2];
        double vertical_angle = atan2(z, sqrt(x * x + y * y)) * 180 / M_PI;
        int rowIdx = (int)((vertical_angle + lidarParams.bottom_angle) / lidarParams.vertical_res + 0.5);

        if (rowIdx < 0 || rowIdx >= lidarParams.height || rowIdx % 4 > 0)
        {
            continue;
        }

        double horizon_angle = atan2(y, x) * 180 / M_PI;

        int columnIdx = horizon_angle / lidarParams.horizon_res;
        if (columnIdx < 0)
        {
            columnIdx += lidarParams.width;
        }
        if (columnIdx < 0 || columnIdx >= lidarParams.width)
        {
            continue;
        }

        full_grid[rowIdx][columnIdx] = sqrt(x * x + y * y);
    }

    return full_grid;
}

void grid_entire2(const sensor_msgs::PointCloud2 &src, vector<vector<double>> &dst, LidarParams &lidarParams)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(src, *cloud);

    dst = vector<vector<double>>(lidarParams.height, vector<double>(lidarParams.width, 0));
    for (int i = 0; i < cloud->size(); i++)
    {
        double x = (*cloud)[i].x;
        double y = (*cloud)[i].y;
        double z = (*cloud)[i].z;
        double vertical_angle = atan2(z, sqrt(x * x + y * y)) * 180 / M_PI;
        int rowIdx = (int)((vertical_angle + lidarParams.bottom_angle) / lidarParams.vertical_res + 0.5);

        if (rowIdx < 0 || rowIdx >= lidarParams.height)
        {
            continue;
        }

        double horizon_angle = atan2(y, x) * 180 / M_PI;

        int columnIdx = horizon_angle / lidarParams.horizon_res;
        if (columnIdx < 0)
        {
            columnIdx += lidarParams.width;
        }
        if (columnIdx < 0 || columnIdx >= lidarParams.width)
        {
            continue;
        }

        dst[rowIdx][columnIdx] = sqrt(x * x + y * y);
    }
}