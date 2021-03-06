#pragma once
#include <iostream>
#include <vector>

#include "../models/lidarParams.cpp"

using namespace std;

void linear_entire(vector<vector<double>> &grid, LidarParams &lidarParams)
{
    for (int i = 4; i < lidarParams.height; i += 4)
    {
        for (int j = 0; j < lidarParams.width; j++)
        {
            // Only front
            /*
            if (j < lidarParams.width * (180 - 42) / 360 || j > lidarParams.width * (180 + 42) / 360)
            {
                continue;
            }
            */

            if (grid[i - 4][j] == 0 && grid[i][j] == 0)
            {
                continue;
            }

            double angle1 = lidarParams.vertical_res * (i - 4) - lidarParams.bottom_angle;
            double r1 = grid[i - 4][j];
            double z1 = grid[i - 4][j] * tan(angle1 * M_PI / 180);

            double angle2 = lidarParams.vertical_res * i - lidarParams.bottom_angle;
            double r2 = grid[i][j];
            double z2 = grid[i][j] * tan(angle2 * M_PI / 180);

            for (int k = i - 3; k < i; k++)
            {
                if (grid[k][j] > 0)
                {
                    continue;
                }

                double angle_tmp = lidarParams.vertical_res * k - lidarParams.bottom_angle;
                double tan_tmp = tan(angle_tmp * M_PI / 180);
                double divider = z1 - r1 * tan_tmp + r2 * tan_tmp - z2;
                if (divider != 0)
                {
                    double s = (r2 * tan_tmp - z2) / divider;
                    grid[k][j] = s * r1 + (1 - s) * r2;
                }
            }
        }
    }

    for (int j = 0; j < lidarParams.width; j++)
    {
        for (int i = lidarParams.height - 4 + 1; i < lidarParams.height; i++)
        {
            grid[i][j] = grid[lidarParams.height - 4][j];
        }
    }
}