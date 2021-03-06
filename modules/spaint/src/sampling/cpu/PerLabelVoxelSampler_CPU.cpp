/**
 * spaint: PerLabelVoxelSampler_CPU.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2015. All rights reserved.
 */

#include "sampling/cpu/PerLabelVoxelSampler_CPU.h"

#include "sampling/shared/PerLabelVoxelSampler_Shared.h"

namespace spaint {

//#################### CONSTRUCTORS ####################

PerLabelVoxelSampler_CPU::PerLabelVoxelSampler_CPU(size_t maxLabelCount, size_t maxVoxelsPerLabel, int raycastResultSize, unsigned int seed)
: PerLabelVoxelSampler(maxLabelCount, maxVoxelsPerLabel, raycastResultSize, seed)
{}

//#################### PRIVATE MEMBER FUNCTIONS ####################

void PerLabelVoxelSampler_CPU::calculate_voxel_mask_prefix_sums(const ORUtils::MemoryBlock<bool>& labelMaskMB) const
{
  const bool *labelMask = labelMaskMB.GetData(MEMORYDEVICE_CPU);
  const unsigned char *voxelMasks = m_voxelMasksMB->GetData(MEMORYDEVICE_CPU);
  unsigned int *voxelMaskPrefixSums = m_voxelMaskPrefixSumsMB->GetData(MEMORYDEVICE_CPU);

  // For each possible label:
  const int stride = m_raycastResultSize + 1;
  for(size_t k = 0; k < m_maxLabelCount; ++k)
  {
    // If the label is not currently in use, continue.
    if(!labelMask[k]) continue;

    // Calculate the prefix sum of the voxel mask.
    const size_t offset = k * stride;
    voxelMaskPrefixSums[offset] = 0;
    for(int i = 1; i < stride; ++i)
    {
      voxelMaskPrefixSums[offset + i] = voxelMaskPrefixSums[offset + (i - 1)] + voxelMasks[offset + (i - 1)];
    }
  }
}

void PerLabelVoxelSampler_CPU::calculate_voxel_masks(const ITMFloat4Image *raycastResult,
                                                     const SpaintVoxel *voxelData,
                                                     const ITMVoxelIndex::IndexData *indexData) const
{
  const Vector4f *raycastResultData = raycastResult->GetData(MEMORYDEVICE_CPU);
  unsigned char *voxelMasks = m_voxelMasksMB->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
  #pragma omp parallel for
#endif
  for(int voxelIndex = 0; voxelIndex < m_raycastResultSize; ++voxelIndex)
  {
    // Update the voxel masks based on the contents of the voxel.
    update_masks_for_voxel(
      voxelIndex,
      raycastResultData,
      m_raycastResultSize,
      voxelData,
      indexData,
      m_maxLabelCount,
      voxelMasks
    );
  }
}

void PerLabelVoxelSampler_CPU::write_candidate_voxel_counts(const ORUtils::MemoryBlock<bool>& labelMaskMB,
                                                            ORUtils::MemoryBlock<unsigned int>& voxelCountsForLabelsMB) const
{
  const bool *labelMask = labelMaskMB.GetData(MEMORYDEVICE_CPU);
  const unsigned int *voxelMaskPrefixSums = m_voxelMaskPrefixSumsMB->GetData(MEMORYDEVICE_CPU);
  unsigned int *voxelCountsForLabels = voxelCountsForLabelsMB.GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
  #pragma omp parallel for
#endif
  for(int k = 0; k < static_cast<int>(m_maxLabelCount); ++k)
  {
    write_candidate_voxel_count(k, m_raycastResultSize, labelMask, voxelMaskPrefixSums, voxelCountsForLabels);
  }
}

void PerLabelVoxelSampler_CPU::write_candidate_voxel_locations(const ITMFloat4Image *raycastResult) const
{
  const Vector4f *raycastResultData = raycastResult->GetData(MEMORYDEVICE_CPU);
  const unsigned char *voxelMasks = m_voxelMasksMB->GetData(MEMORYDEVICE_CPU);
  const unsigned int *voxelMaskPrefixSums = m_voxelMaskPrefixSumsMB->GetData(MEMORYDEVICE_CPU);
  Vector3s *candidateVoxelLocations = m_candidateVoxelLocationsMB->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
  #pragma omp parallel for
#endif
  for(int voxelIndex = 0; voxelIndex < m_raycastResultSize; ++voxelIndex)
  {
    write_candidate_voxel_location(
      voxelIndex,
      raycastResultData,
      m_raycastResultSize,
      voxelMasks,
      voxelMaskPrefixSums,
      m_maxLabelCount,
      candidateVoxelLocations
    );
  }
}

void PerLabelVoxelSampler_CPU::write_sampled_voxel_locations(const ORUtils::MemoryBlock<bool>& labelMaskMB,
                                                             ORUtils::MemoryBlock<Vector3s>& sampledVoxelLocationsMB) const
{
  const Vector3s *candidateVoxelLocations = m_candidateVoxelLocationsMB->GetData(MEMORYDEVICE_CPU);
  const int *candidateVoxelIndices = m_candidateVoxelIndicesMB->GetData(MEMORYDEVICE_CPU);
  const bool *labelMask = labelMaskMB.GetData(MEMORYDEVICE_CPU);
  Vector3s *sampledVoxelLocations = sampledVoxelLocationsMB.GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
  #pragma omp parallel for
#endif
  for(int voxelIndex = 0; voxelIndex < static_cast<int>(m_maxVoxelsPerLabel); ++voxelIndex)
  {
    copy_sampled_voxel_locations(
      voxelIndex,
      labelMask,
      m_maxLabelCount,
      m_maxVoxelsPerLabel,
      m_raycastResultSize,
      candidateVoxelLocations,
      candidateVoxelIndices,
      sampledVoxelLocations
    );
  }
}

}
