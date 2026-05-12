/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CameraParser
*/

#include "CameraParser.hpp"
#include <cmath>
#include <memory>
#include <numbers>
#include "components/camera/perspective/PerspectiveCamera.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

namespace math = raytracer::math;

constexpr double halfTurn = 2.0;
constexpr double degreesPerRadian = 180.0 / std::numbers::pi;
constexpr float cameraForwardAxis = -1.F;
constexpr float cameraUpAxis = 1.F;

math::Vector3D toVector3D(const aiVector3D& vector) {
  return {vector.x, vector.y, vector.z};
}

}  // namespace

namespace raytracer::scene {

void CameraParser::parse(const aiScene* scene, SceneBuilder& builder,
                         math::RenderSettings& settings) {
  if (scene->mNumCameras == 0) {
    return;
  }

  aiCamera* cam = scene->mCameras[0];
  aiMatrix4x4 camTransform = buildCameraTransform(scene, cam);
  addCamera(builder, cam, camTransform, settings);
}

aiMatrix4x4 CameraParser::buildCameraTransform(const aiScene* scene,
                                               const aiCamera* cam) {
  aiMatrix4x4 camTransform;
  aiNode* camNode = scene->mRootNode->FindNode(cam->mName);

  if (camNode != nullptr) {
    aiNode* current = camNode;
    while (current != nullptr) {
      camTransform = current->mTransformation * camTransform;
      current = current->mParent;
    }
  }

  return camTransform;
}

double CameraParser::computeVerticalFov(const aiCamera* cam,
                                        double aspectRatio) {
  const double hfov = cam->mHorizontalFOV;
  const double vfovRad =
      halfTurn * std::atan(std::tan(hfov / halfTurn) / aspectRatio);
  return vfovRad * degreesPerRadian;
}

void CameraParser::addCamera(SceneBuilder& builder, const aiCamera* cam,
                             const aiMatrix4x4& camTransform,
                             math::RenderSettings& settings) {
  aiVector3D pos;
  aiVector3D scale;
  aiQuaternion rot;
  camTransform.Decompose(scale, rot, pos);

  aiMatrix3x3 rotMat(rot.GetMatrix());
  aiVector3D forward = rotMat * aiVector3D(0.F, 0.F, cameraForwardAxis);
  aiVector3D up = rotMat * aiVector3D(0.F, cameraUpAxis, 0.F);

  math::Vector3D position = toVector3D(pos);
  math::Vector3D target(pos.x + forward.x, pos.y + forward.y,
                        pos.z + forward.z);
  math::Vector3D upVec = toVector3D(up);

  double aspectRatio = static_cast<double>(settings.imageWidth) /
                       static_cast<double>(settings.imageHeight);

  double vfov = computeVerticalFov(cam, aspectRatio);

  auto cameraObj =
      std::make_shared<raytracer::components::camera::perspective::Perspective>(
          position, target, upVec, vfov, aspectRatio);
  builder.addCamera(std::move(cameraObj));
}

}  // namespace raytracer::scene
