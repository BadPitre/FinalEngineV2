#include "gameplay.hh"
#include "../core/collision.hh"
#include "../core/debug//perf_monitor.hh"
#include "../core/debug/debug_menu.hh"
#include "../core/object/gameobject_manager.hh"
#include "../core/raycast.hh"
#include "../render/colour.hh"
#include "../render/renderer.hh"
#include "../sound/sound_manager.hh"
#include "psyqo/alloc.h"
#include "psyqo/xprintf.h"

using namespace psyqo::fixed_point_literals;
using namespace psyqo::trig_literals;

namespace {
constexpr psyqo::FixedPoint<> c_moveSpeed = 0.05_fp;
constexpr psyqo::Angle c_yawSpeed = 0.02_pi;
constexpr psyqo::Angle c_spinSpeed = 0.01_pi;
}

void GameplayScene::start(StartReason reason) {
  Renderer::Instance().StartScene();

  g_madnightEngine.m_input.setOnEvent([&](auto event) {
    if (event.type != psyqo::AdvancedPad::Event::ButtonReleased)
      return;
    if (event.button == psyqo::AdvancedPad::Button::Start)
      m_menu.Activate();
  });

  // the below only needs to happen if this was a freshly created scene
  if (reason != StartReason::Create)
    return;

  m_heapSizeText = m_debugHUD.AddTextHUDElement(TextHUDElement("HEAP", {.pos = {5, 0}, .size = {100, 100}}));
  m_fpsText = m_debugHUD.AddTextHUDElement(TextHUDElement("FPS", {.pos = {5, 15}, .size = {100, 100}}));

  m_camera = new Camera();
  m_camera->SetFreeLook({.x = 0, .y = 0, .z = 0});
  Renderer::Instance().SetActiveCamera(m_camera);
}

void GameplayScene::teardown(TearDownReason reason) { g_madnightEngine.m_input.setOnEvent(nullptr); }

void GameplayScene::frame() {
  auto &renderInstance = Renderer::Instance();
  auto &gpu = Renderer::Instance().GPU();
  uint32_t deltaTime = renderInstance.Process();
  if (deltaTime == 0)
    return;

  // simple FPS-style controls: arrows translate, L2/R2 yaw, L1/R1 vertical
  using Btn = psyqo::AdvancedPad::Button;
  constexpr auto P = psyqo::AdvancedPad::Pad::Pad1a;
  auto &pad = g_madnightEngine.m_input;

  if (pad.isButtonPressed(P, Btn::L2)) m_camera->UpdateAngles(0, -c_yawSpeed, 0);
  if (pad.isButtonPressed(P, Btn::R2)) m_camera->UpdateAngles(0,  c_yawSpeed, 0);

  auto forward = m_camera->forwardVector();
  auto right = m_camera->rightVector();
  auto pos = m_camera->pos();

  if (pad.isButtonPressed(P, Btn::Up))    { pos.x += forward.x * c_moveSpeed; pos.y += forward.y * c_moveSpeed; pos.z += forward.z * c_moveSpeed; }
  if (pad.isButtonPressed(P, Btn::Down))  { pos.x -= forward.x * c_moveSpeed; pos.y -= forward.y * c_moveSpeed; pos.z -= forward.z * c_moveSpeed; }
  if (pad.isButtonPressed(P, Btn::Right)) { pos.x += right.x   * c_moveSpeed; pos.y += right.y   * c_moveSpeed; pos.z += right.z   * c_moveSpeed; }
  if (pad.isButtonPressed(P, Btn::Left))  { pos.x -= right.x   * c_moveSpeed; pos.y -= right.y   * c_moveSpeed; pos.z -= right.z   * c_moveSpeed; }
  if (pad.isButtonPressed(P, Btn::L1))    pos.y -= c_moveSpeed; // +Y is screen-down on PSX, so L1 lifts the camera
  if (pad.isButtonPressed(P, Btn::R1))    pos.y += c_moveSpeed;

  m_camera->SetPosition(pos);

  // spin every Suzanne in place around its Y axis (other ENVIRONMENT
  // objects, like the room geometry, stay still)
  for (auto *obj : GameObjectManager::GetGameObjectsWithTag(GameObjectTag::ENVIRONMENT)) {
    if (__builtin_strncmp(obj->name().c_str(), "SUZANNE", 7) != 0)
      continue;
    auto rot = obj->rotation();
    rot.y += c_spinSpeed;
    obj->SetRotation(rot);
  }

  // process camera inputs
  m_camera->Process(deltaTime);

  // process debug menu
  DebugMenu::Process();

  // raycast
  const auto &raycastDistance = DebugMenu::RaycastDistance();
  Ray ray = {
      .origin = m_camera->pos(), .direction = m_camera->forwardVector(), .maxDistance = raycastDistance * ONE_METRE};
  RayHit hit = {0};

  // bool didHit = Raycast::RaycastScene(ray, GameObjectTag::ENVIRONMENT, &hit);
  // printf("did hit=%d\n", didHit);

  // collision detection test...
  // auto objects = GameObjectManager::GetGameObjectsWithTag(GameObjectTag::ENVIRONMENT);
  // bool collision = Collision::IsSATCollision(objects[0]->obb(), objects[1]->obb());
  // printf("collision=%d\n", collision);

  // the central point for rendering gameobjects etc
  renderInstance.Render();

  if (DebugMenu::IsEnabled())
    return;

  if (DebugMenu::DisplayDebugHUD())
    PerfMonitor::Render(deltaTime);
}
