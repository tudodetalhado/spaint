/**
 * spaintgui: RiftRenderer.h
 */

#ifndef H_SPAINTGUI_RIFTRENDERER
#define H_SPAINTGUI_RIFTRENDERER

#include <string>

#include <OVR.h>

#include <spaint/ogl/WrappedGL.h>

#include "Renderer.h"

/**
 * \brief An instance of this class can be used to render the scene constructed by the spaint engine to the Oculus Rift.
 */
class RiftRenderer : public Renderer
{
  //#################### TYPEDEFS ####################
private:
  typedef spaint::shared_ptr<ITMUChar4Image> ITMUChar4Image_Ptr;

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
  /** The images in which to store the eye textures each frame. */
  ITMUChar4Image_Ptr m_eyeImages[ovrEye_Count];

  /** The eye texture IDs. */
  GLuint m_eyeTextureIDs[ovrEye_Count];

  /** The Rift handle. */
  ovrHmd m_hmd;

  //#################### CONSTRUCTORS ####################
public:
  /**
   * \brief Constructs a Rift renderer.
   *
   * \param title         The title to give the window.
   * \param spaintEngine  The spaint engine.
   * \param renderingMode The rendering mode to use.
   */
  RiftRenderer(const std::string& title, const spaint::SpaintEngine_Ptr& spaintEngine, RiftRenderingMode renderingMode);

  //#################### DESTRUCTOR ####################
public:
  ~RiftRenderer();

  //#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
  // Deliberately private and unimplemented.
  RiftRenderer(const RiftRenderer&);
  RiftRenderer& operator=(const RiftRenderer&);

  //#################### PUBLIC MEMBER FUNCTIONS ####################
public:
  /** Override */
  virtual void render(const spaint::SpaintEngine_Ptr& spaintEngine) const;
};

#endif