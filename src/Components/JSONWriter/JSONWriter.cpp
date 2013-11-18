/*!
 * \file
 * \brief
 * \author Micha Laszkowski
 */

#include <memory>
#include <string>

#include "JSONWriter.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

namespace Processors {
namespace JSONWriter {

JSONWriter::JSONWriter(const std::string & name) :
		Base::Component(name)  {

}

JSONWriter::~JSONWriter() {
}

void JSONWriter::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_cloud_xyzrgbsift", &in_cloud_xyzrgbsift);
	registerStream("in_cloud_xyz", &in_cloud_xyz);
	registerStream("in_cloud_xyzrgb", &in_cloud_xyzrgb);
	// Register handlers
	h_write_xyz.setup(boost::bind(&JSONWriter::write_xyz, this));
	registerHandler("write_xyz", &h_write_xyz);
	addDependency("write_xyz", &in_cloud_xyz);
	h_write_xyzrgb.setup(boost::bind(&JSONWriter::write_xyzrgb, this));
	registerHandler("write_xyzrgb", &h_write_xyzrgb);
	addDependency("write_xyzrgb", &in_cloud_xyzrgb);
	h_write_xyzrgbsift.setup(boost::bind(&JSONWriter::write_xyzrgbsift, this));
	registerHandler("write_xyzrgbsift", &h_write_xyzrgbsift);
	addDependency("write_xyzrgbsift", &in_cloud_xyzrgbsift);

}

bool JSONWriter::onInit() {
	i=0;
	return true;
}

bool JSONWriter::onFinish() {
	return true;
}

bool JSONWriter::onStop() {
	return true;
}

bool JSONWriter::onStart() {
	return true;
}

void JSONWriter::write_xyz() {
}

void JSONWriter::write_xyzrgb() {
}

void JSONWriter::write_xyzrgbsift() {
	cout<<"JSONWriter::write_xyzrgbsift()"<<endl;
	pcl::PointCloud<PointXYZRGBSIFT>::Ptr cloud = in_cloud_xyzrgbsift.read();
	
	ptree ptree_file;
	try{
		read_json("/home/mlaszkow/pcd/test.json", ptree_file);
	}
	catch(std::exception const& e){}
	cout<<"I: "<<++i<<endl;
	
	pcl::PointCloud<PointXYZRGBSIFT>::iterator pt_iter = cloud->begin();
	
	ptree ptree_cloud;
	for (int v = 0; v < (int) cloud->height; ++v) {
		for (int u = 0; u < (int) cloud->width; ++u) {
			PointXYZRGBSIFT& pt = *pt_iter++;
			if(isnan(pt.x)){
				continue;				
			}		
			ptree ptree_point; 
			ptree_point.put ("x", pt.x);
			ptree_point.put ("y", pt.y);
			ptree_point.put ("z", pt.z);
			ptree_point.put ("RGB", pt.rgba); //TODO
			//ptree_point.put ("R", rand()%255);
			//ptree_point.put ("G", rand()%255);
			//ptree_point.put ("B", rand()%255);
			cv::Mat descriptor = pt.descriptor;
			
			if(!descriptor.empty()) {
				ptree descriptor;
				for(int j = 0; j<128;j++){
					ptree child;
					child.put("", pt.descriptor.at<float>(j));
					descriptor.push_back(std::make_pair("", child));	
					//cout<<pt.descriptor.at<float>(j)<<" ";
				}
				ptree_point.add_child("SIFT", descriptor);
			}
			ptree_cloud.push_back(std::make_pair("", ptree_point));
		}
	}
	ptree_file.add_child("cloud", ptree_cloud);
	
	write_json ("/home/mlaszkow/pcd/test.json", ptree_file);
/*	
	ptree ptree_cloud;
	for (size_t i = 0; i < 10 ; ++i){ //cloud_xyzrgbsift->points.size(); ++i){
		if (isnan(cloud_xyzrgbsift->points[i].x))
			continue;
		ptree ptree_point; 
		ptree_point.put ("x", cloud_xyzrgbsift->points[i].x);
		ptree_point.put ("y", cloud_xyzrgbsift->points[i].y);
		ptree_point.put ("z", cloud_xyzrgbsift->points[i].z);
		ptree_point.put ("R", rand()%255);
		ptree_point.put ("G", rand()%255);
		ptree_point.put ("B", rand()%255);
		if(rand()%3==1){
			ptree descriptor;
			for(int k=0;k<120;k++){
				ptree child;
				child.put("", rand()%250);
				descriptor.push_back(std::make_pair("", child));	
			}
			ptree_point.add_child("SIFT", descriptor);
		}
		ptree_cloud.push_back(std::make_pair("", point));
	}
	ptree_file.add_child("cloud", cloud);
	
	write_json ("/home/mlaszkow/pcd/test.json", ptree_file);	
 */
}



} //: namespace JSONWriter
} //: namespace Processors