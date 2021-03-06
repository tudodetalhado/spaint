/**
 * spaintgui: RiftRenderer.h
 * Copyright (c) Torr Vision Group, University of Oxford, 2015. All rights reserved.
 */

#ifndef H_SPAINTGUI_RIFTRENDERER
#define H_SPAINTGUI_RIFTRENDERER

#include <string>

#include <OVR_CAPI.h>

#include <rigging/CompositeCamera.h>

#include <spaint/ogl/FrameBuffer.h>

#include "Renderer.h"

/**
 * \brief An instance of this class can be used to render the spaint scene to the Oculus Rift.
 */
class RiftRenderer : public Renderer
{
  //#################### ENUMERATIONS ####################
public:
  /**
   * \brief An enumeration containing the various possible Rift rendering modes.
   */
  enum RiftRenderingMode
  {
    FULLSCREEN_MODE,
    WINDOWED_MODE
  };

  //#################### PRIVATE VARIABLES ####################
private:
  /** The eye frame buffers. */
  spaint::FrameBuffer_CPtr m_eyeFrameBuffers[ovrEye_Count];

  /** The Rift handle. */
  ovrHmd m_hmd;

  //#################### CONSTRUCTORS ####################
public:
  /**
   * \brief Constructs a Rift renderer.
   *
   * \param title                   The title of the window.
   * \param model                   The spaint model.
   * \param subwindowConfiguration  The sub-window configuration to use for visualising the scene.
   * \param renderingMode           The rendering mode to use.
   */
  RiftRenderer(const std::string& title, const Model_CPtr& model, const SubwindowConfiguration_Ptr& subwindowConfiguration, RiftRenderingMode renderingMode);

  //#################### DESTRUCTOR ####################
public:
  /**
   * \brief Destroys the renderer.
   */
  ~RiftRenderer();

  //#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
  // Deliberately private and unimplemented.
  RiftRenderer(const RiftRenderer&);
  RiftRenderer& operator=(const RiftRenderer&);

  //#################### PUBLIC MEMBER FUNCTIONS ####################
public:
  /** Override */
  virtual VoxelRenderState_CPtr get_monocular_render_state(size_t subwindowIndex) const;

  /** Override */
  virtual bool is_mono() const;

  /** Override */
  virtual void render(const Vector2f& fracWindowPos, bool renderFiducials) const;
};

#endif
