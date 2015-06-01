/**
 * spaint: ImageProcessor_CUDA.h
 */

#ifndef H_SPAINT_IMAGEPROCESSOR_CUDA
#define H_SPAINT_IMAGEPROCESSOR_CUDA

#include "../interface/ImageProcessor.h"

namespace spaint {

/**
 * \brief An instance of this class may be used to apply image processing algorithms to images with CUDA.
 */
class ImageProcessor_CUDA : public ImageProcessor
{
  //#################### PUBLIC MEMBER FUNCTIONS ####################
public:
  /** Override. */
  virtual void calculate_absolute_difference(const ITMFloatImage_CPtr& firstInputImage, const ITMFloatImage_CPtr& secondInputImage, const AFImage_Ptr& outputImage) const;

  /** Override. */
  virtual void set_on_threshold(const ITMFloatImage_CPtr& input, ComparisonOperator op, float threshold, float value, const ITMFloatImage_Ptr& output) const;
};

}

#endif
