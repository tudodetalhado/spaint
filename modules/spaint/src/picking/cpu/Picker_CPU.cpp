/**
 * spaint: Picker_CPU.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2015. All rights reserved.
 */

#include <stdexcept>

#include "picking/cpu/Picker_CPU.h"

#include "picking/shared/Picker_Shared.h"

namespace spaint {

//#################### PUBLIC MEMBER FUNCTIONS ####################

bool Picker_CPU::pick(int x, int y, const ITMLib::ITMRenderState *renderState, ORUtils::MemoryBlock<Vector3f>& pickPointsMB, size_t offset) const
{
  if(offset >= pickPointsMB.dataSize)
  {
    throw std::runtime_error("Error: The offset at which to write the pick point is out of range");
  }

  return get_pick_point(
    x, y,
    renderState->raycastResult->noDims.x,
    renderState->raycastResult->GetData(MEMORYDEVICE_CPU),
    *(pickPointsMB.GetData(MEMORYDEVICE_CPU) + offset)
  );
}

void Picker_CPU::to_short(const ORUtils::MemoryBlock<Vector3f>& pickPointsFloatMB, ORUtils::MemoryBlock<Vector3s>& pickPointsShortMB) const
{
  if(pickPointsFloatMB.dataSize != pickPointsShortMB.dataSize)
  {
    throw std::runtime_error("Error: The memory block into which to write the converted pick points must be of the right size");
  }

  const Vector3f *pickPointsFloat = pickPointsFloatMB.GetData(MEMORYDEVICE_CPU);
  Vector3s *pickPointsShort = pickPointsShortMB.GetData(MEMORYDEVICE_CPU);
  int pointCount = static_cast<int>(pickPointsFloatMB.dataSize);

#ifdef WITH_OPENMP
  #pragma omp parallel for
#endif
  for(int i = 0; i < pointCount; ++i)
  {
    pickPointsShort[i] = pickPointsFloat[i].toShortRound();
  }
}

}
