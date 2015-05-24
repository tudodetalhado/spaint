/**
 * spaintgui: Renderer.cpp
 */

#include "Renderer.h"

//#################### CONSTRUCTORS ####################

Renderer::Renderer(const spaint::SpaintModel_CPtr& model, const spaint::SpaintRaycaster_CPtr& raycaster)
: m_cameraMode(CM_FOLLOW), m_model(model), m_phongEnabled(false), m_raycaster(raycaster)
{
  // Create an image into which to temporarily store visualisations of the scene.
  m_image.reset(new ITMUChar4Image(m_model->get_depth_image_size(), true, true));
}

//#################### DESTRUCTOR ####################

Renderer::~Renderer() {}

//#################### PUBLIC MEMBER FUNCTIONS ####################

Renderer::CameraMode Renderer::get_camera_mode() const
{
  return m_cameraMode;
}

bool Renderer::get_phong_enabled() const
{
  return m_phongEnabled;
}

void Renderer::set_camera_mode(CameraMode cameraMode)
{
  m_cameraMode = cameraMode;
}

void Renderer::set_phong_enabled(bool phongEnabled)
{
  m_phongEnabled = phongEnabled;
}

//#################### PROTECTED MEMBER FUNCTIONS ####################

void Renderer::begin_2d()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDepthMask(false);
}

void Renderer::end_2d()
{
  glDepthMask(true);

  // We assume that the matrix mode is still set to GL_MODELVIEW at the start of this function.
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void Renderer::render_scene_to_buffer(const ITMPose& pose, const spaint::SpaintInteractor_CPtr& interactor, GLuint frameBufferID)
{
  // TODO
}

void Renderer::render_texture_to_buffer(GLuint textureID, GLuint frameBufferID)
{
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // Draw a quad textured with the specified texture.
  begin_2d();
    glEnable(GL_TEXTURE_2D);
    {
      glBindTexture(GL_TEXTURE_2D, textureID);
      glColor3f(1.0f, 1.0f, 1.0f);
      glBegin(GL_QUADS);
      {
        glTexCoord2f(0, 1); glVertex2f(0, 0);
        glTexCoord2f(1, 1); glVertex2f(1, 0);
        glTexCoord2f(1, 0); glVertex2f(1, 1);
        glTexCoord2f(0, 0); glVertex2f(0, 1);
      }
      glEnd();
    }
    glDisable(GL_TEXTURE_2D);
  end_2d();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//#################### PRIVATE MEMBER FUNCTIONS ####################

void Renderer::set_projection_matrix(const ITMIntrinsics& intrinsics, int width, int height)
{
  double nearVal = 0.1;
  double farVal = 1000.0;

  // To rederive these equations, use similar triangles. Note that fx = f / sx and fy = f / sy,
  // where sx and sy are the dimensions of a pixel on the image plane.
  double leftVal = -intrinsics.projectionParamsSimple.px * nearVal / intrinsics.projectionParamsSimple.fx;
  double rightVal = (width - intrinsics.projectionParamsSimple.px) * nearVal / intrinsics.projectionParamsSimple.fx;
  double bottomVal = -intrinsics.projectionParamsSimple.py * nearVal / intrinsics.projectionParamsSimple.fy;
  double topVal = (height - intrinsics.projectionParamsSimple.py) * nearVal / intrinsics.projectionParamsSimple.fy;

  glLoadIdentity();
  glFrustum(leftVal, rightVal, bottomVal, topVal, nearVal, farVal);
}
