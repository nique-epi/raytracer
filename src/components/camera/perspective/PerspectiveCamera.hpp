/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** perspectiveCamera
*/

#pragma once

#include "components/camera/ICamera.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

/**
 * @brief Perspective camera utilities.
 */
namespace raytracer::components::camera::perspective {

/**
 * @brief Default vertical field of view in degrees.
 */
constexpr double DEFAULT_FOV = 90.0;

/**
 * @brief Generate rays for a thin-lens perspective camera.
 *
 * The camera supports a configurable position, look-at target, up vector,
 * vertical field of view, aspect ratio, aperture and focus distance.
 */
class Perspective : public ICamera {
 public:
  /**
   * @brief Construct a default perspective camera.
   *
   * The camera uses a unit aspect ratio, a 90-degree vertical field of view,
   * no aperture and a focus distance of 1.
   */
  Perspective();

  /**
   * @brief Construct a perspective camera from scene parameters.
   *
   * @param [in] lookFrom   Camera position.
   * @param [in] lookAt     Point the camera looks at.
   * @param [in] vup        Up direction used to build the camera basis.
   * @param [in] vfov       Vertical field of view in degrees.
   * @param [in] aspectRatio Image aspect ratio.
   * @param [in] aperture   Lens aperture used for depth of field.
   * @param [in] focusDist   Focus distance from the camera origin.
   */
  Perspective(const raytracer::math::Vector3D& lookFrom,
              const raytracer::math::Vector3D& lookAt,
              const raytracer::math::Vector3D& vup, double vfov,
              double aspectRatio, double aperture = 0.0,
              double focusDist = 1.0);

  Perspective(const Perspective&) = default;
  Perspective& operator=(const Perspective&) = default;
  Perspective(Perspective&&) = default;
  Perspective& operator=(Perspective&&) = default;
  ~Perspective() override = default;

  /**
   * @brief Compute a ray passing through normalized screen coordinates.
   *
   * @param [in] u Horizontal coordinate in the image plane.
   * @param [in] v Vertical coordinate in the image plane.
   * @returns The generated ray.
   */
  [[nodiscard]] raytracer::math::Ray getRay(double u, double v) const override;

  /**
   * @brief Update the aspect ratio from an output resolution.
   *
   * @param [in] width  Output image width in pixels.
   * @param [in] height Output image height in pixels.
   */
  void setResolution(int width, int height) override;
  void setPosition(const raytracer::math::Vector3D& position);
  void setDirection(const raytracer::math::Vector3D& direction);
  void setFieldOfView(double fovDegrees);

 private:
  void updateCamera();
  [[nodiscard]] raytracer::math::Vector3D randomInUnitDisk() const;

  raytracer::math::Vector3D position = raytracer::math::Vector3D(0.0, 0.0, 0.0);
  raytracer::math::Vector3D target = raytracer::math::Vector3D(0.0, 0.0, -1.0);
  raytracer::math::Vector3D up = raytracer::math::Vector3D(0.0, 1.0, 0.0);
  double fov = DEFAULT_FOV;
  double aspectRatio = 1.0;
  double aperture = 0.0;
  double focusDist = 1.0;

  raytracer::math::Vector3D u = raytracer::math::Vector3D(1.0, 0.0, 0.0);
  raytracer::math::Vector3D v = raytracer::math::Vector3D(0.0, 1.0, 0.0);
  raytracer::math::Vector3D w = raytracer::math::Vector3D(0.0, 0.0, 1.0);
  raytracer::math::Vector3D lowerLeftCorner =
      raytracer::math::Vector3D(0.0, 0.0, 0.0);
  raytracer::math::Vector3D horizontal =
      raytracer::math::Vector3D(0.0, 0.0, 0.0);
  raytracer::math::Vector3D vertical = raytracer::math::Vector3D(0.0, 0.0, 0.0);
};

}  // namespace raytracer::components::camera::perspective
