/*!
 * \file
 * \brief 
 * \author tkornuta
 */

#ifndef PairwiseRegistration_HPP_
#define PairwiseRegistration_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include "Types/HomogMatrix.hpp"

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

namespace Processors {
namespace PairwiseRegistration {

/*!
 * \class PairwiseRegistration
 * \brief PairwiseRegistration processor class.
 *
 * PairwiseRegistration processor.
 */
class PairwiseRegistration: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	PairwiseRegistration(const std::string & name = "PairwiseRegistration");

	/*!
	 * Destructor
	 */
	virtual ~PairwiseRegistration();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	/// Input data stream containing XYZ cloud.
	Base::DataStreamIn<pcl::PointCloud<pcl::PointXYZ>::Ptr, Base::DataStreamBuffer::Newest> in_cloud_xyz;

	/// Input data stream containing XYZRGB cloud.
	Base::DataStreamIn<pcl::PointCloud<pcl::PointXYZRGB>::Ptr, Base::DataStreamBuffer::Newest> in_cloud_xyzrgb;

	/// Initial transformation between clouds.
	Base::DataStreamIn<Types::HomogMatrix, Base::DataStreamBuffer::Newest> in_transformation;

	/// Resulting transformation between XYZ clouds.
	Base::DataStreamOut <Types::HomogMatrix> out_transformation_xyz;

	/// Resulting transformation between XYZRGB clouds.
	Base::DataStreamOut <Types::HomogMatrix> out_transformation_xyzrgb;

	// Properties.	
	Base::Property<bool> negative;
	Base::Property<float> StddevMulThresh;
	Base::Property<float> MeanK;
	
	/// Registration handler - activated when 
	void pairwise_registration();

	/// Aligns XYZ clouds.
	void registration_xyz(Types::HomogMatrix hm_);

	/// Aligns XYZRGB clouds.
	void registration_xyzrgb(Types::HomogMatrix hm_);

};

} //: namespace PairwiseRegistration
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("PairwiseRegistration", Processors::PairwiseRegistration::PairwiseRegistration)

#endif /* PairwiseRegistration_HPP_ */