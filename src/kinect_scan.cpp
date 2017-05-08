#include <ros/ros.h>
#include <octomap/octomap.h>
#include <octomap/AbstractOcTree.h>
#include <octomap/math/Pose6D.h>

#include "kinect_ros/CameraImage.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using std::cin;
using std::cout;
using std::cerr;

using std::vector;
using std::string;

using octomap::OcTree;
using octomap::point3d;

using octomath::Pose6D;
using octomath::Vector3;
using octomath::Quaternion;

using sensor_msgs::Image;

const int kinect_camera_h = 480;
const int kinect_camera_w = 640;

const double pi = acos(-1.0);
const double kinect_view_angle_h = 43.0 / 180.0 * pi;
const double kinect_view_angle_w = 57.0 / 180.0 * pi;

const double kinect_view_angle_h_step = 2 * kinect_view_angle_h / kinect_camera_h;
const double kinect_view_angle_w_step = 2 * kinect_view_angle_w / kinect_camera_w;


void getCameraPosition(const Pose6D& camera, const point3d& point, int& i, int& j) {
    Vector3 vec_to_point = point - camera.trans();
    Vector3 oriented = camera.rot().rotate(vec_to_point);
    
    i = kinect_camera_h * (0.5 + oriented.y() / (2 * oriented.z() * tan(kinect_view_angle_h)));
    j = kinect_camera_w * (0.5 + oriented.x() / (2 * oriented.z() * tan(kinect_view_angle_w)));
}

void processPoint(const Pose6D camera, const point3d& center, const double& radius, CameraImage& image) {
    int blockSz = int(radius / 0.01);
    for (int dx = -blockSz; dx <= blockSz; dx++) {
        for (int dy = -blockSz; dy <= blockSz; dy++) {
            for (int dz = -blockSz; dz <= blockSz; dz++) {
                if (abs(dx) < blockSz && abs(dy) < blockSz && abs(dz) < blockSz)
                    continue;

                point3d point(center.x() + dx * radius / blockSz, center.y() + dy * radius / blockSz, center.z() + dz * radius / blockSz);

                int i, j;
                getCameraPosition(camera, point, i, j);

                if (0 <= i && i < kinect_camera_h && 0 <= j && j < kinect_camera_w)
                    image.update(i, j, uint16_t(100.0 * camera.trans().distance(point)));
            }
        }
    }
}

void getDistanceMatrix(const Pose6D& camera, const OcTree& tree, CameraImage& image) {
    for (OcTree::leaf_iterator iter = tree.begin_leafs(); iter != tree.end_leafs(); iter++)
        processPoint(camera, iter.getCoordinate(), iter.getSize() / 2, image);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "kinect_scan");
    ros::Time::init();

    if (argc != 4) {
        cout << "Usage: kinect_scan [map.bt] [camera_pos] [output_file]" << std::endl;
        return 1;
    }

    try {
        std::ifstream ifs_data(argv[1]);
        OcTree tree(0.1);
        tree.readBinary(ifs_data);
        ifs_data.close();
        
        std::ifstream ifs_cam(argv[2]);
        Pose6D camera;
        camera.read(ifs_cam);
        ifs_cam.close();

        
        CameraImage image(kinect_camera_h, kinect_camera_w);
        getDistanceMatrix(camera, tree, image);
        
        std::ofstream ofs(argv[3]);
        image.write(ofs);
        ofs.close();
    } catch (std::exception& e) {
        cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
