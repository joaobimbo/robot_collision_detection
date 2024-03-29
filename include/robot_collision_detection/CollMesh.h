//
// Created by joao on 30/05/18.
//

#ifndef ROBOT_COLLISION_DETECTION_COLLMESH_H
#define ROBOT_COLLISION_DETECTION_COLLMESH_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/scene.h>           // Output data structure
#include <visualization_msgs/MarkerArray.h>
#include <urdf/model.h>
#include <kdl/frames.hpp>
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/features/feature.h>
#include <pcl_ros/transforms.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud.h>

class CollMesh {
public:
    typedef pcl::PointXYZINormal PType;
    typedef pcl::PointCloud<PType> PointCloud;
    CollMesh(){}
    CollMesh(aiMesh* in_mesh);
    CollMesh(std::string fname, urdf::LinkConstSharedPtr link);
    ~CollMesh();
    visualization_msgs::Marker getMarker();
    sensor_msgs::PointCloud2 getPointCloud();
    aiMesh* getMesh();
    bool getLikelihoods(std::vector<KDL::Wrench> force, PointCloud::Ptr points, KDL::Wrench force_meas,std::vector<float> &likelihood);
    float getLikelihood(KDL::Wrench force, KDL::Wrench force_meas, int idx);
    std::string link_name_;
    void setPose(KDL::Frame in);
    KDL::Frame getPose();
    unsigned int getMeshSize();
    std::vector<int> getNearestKidx(int K, PType p);
    PointCloud::Ptr  getNearestK(int K,PType p);
    std::vector<int> getPointsInRadius(PType p,double radius);
    void setPointCloudIntensity(std::vector<float> intensities);
    KDL::Wrench ForceToMeasurement(PType p,KDL::Vector f, float local_torque);
    KDL::Wrench ForceToMeasurement(KDL::Wrench f_global);
    KDL::Wrench ForceAtPoint(unsigned long idx, double F, float local_torque);

   //make protected
    KDL::Wrench mask_;
    PointCloud::Ptr pcloud_;
    pcl::KdTreeFLANN<PType> kdtree_;

protected:
    aiMesh ass_mesh_;
    KDL::Frame pose_;
    Assimp::Importer importer_;
    visualization_msgs::Marker marker_;
    sensor_msgs::PointCloud2 pointcloud2_;
    std::vector<KDL::Vector> normals_kdl_, points_kdl_;

    void init(aiMesh* in_mesh,KDL::Frame T);
    void copyMesh(aiMesh* in_mesh,aiMesh* out_mesh);
    visualization_msgs::Marker createMarker();
    PointCloud::Ptr createPointCloud(aiMesh* in_mesh,const std::string name);

};


#endif //ROBOT_COLLISION_DETECTION_COLLMESH_H
