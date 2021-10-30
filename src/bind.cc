// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "bind.h"

// Unit Testing includes
#include "doctest.h"

#ifndef DOCTEST_CONFIG_DISABLE
SCENARIO("class bind")
{
  GIVEN("a bind object bound to a key")
  {
    bool pressed  = false;
    bool released = false;
    double delta  = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) { delta += d; },
        "Category",
        "Name");
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bind->bindKey(0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bind->bound() == true);
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind->combinedName() == "Category:Name");
    }

    WHEN("the key is pressed and released")
    {
      // just that the same integer is used when sending the key pressed event
      nebula::bind::keyboardEvent(0, nebula::bind::action::press, modifier);
      nebula::bind::keyboardEvent(0, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(pressed == true);
        REQUIRE(released == true);
      }
    }

    WHEN("a different key is pressed and released")
    {
      nebula::bind::keyboardEvent(1, nebula::bind::action::press, modifier);
      nebula::bind::keyboardEvent(1, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }

    WHEN("it is unbound")
    {
      bind->unbind();

      THEN("it should report as unbound")
      {
        REQUIRE(bind->bound() == false);
      }

      THEN("it should not react to the key it was bound to")
      {
        nebula::bind::keyboardEvent(0, nebula::bind::action::press, modifier);
        nebula::bind::keyboardEvent(0, nebula::bind::action::release, modifier);
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a mouse button")
  {
    bool pressed  = false;
    bool released = false;
    double delta  = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) { delta += d; },
        "Category",
        "Name");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindMButton(0, modifier);

    WHEN("the button is pressed and released")
    {
      // just that the same integer is used when sending the key pressed event
      nebula::bind::mouseButtonEvent(0, nebula::bind::action::press, modifier);
      nebula::bind::mouseButtonEvent(
          0, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(pressed == true);
        REQUIRE(released == true);
      }
    }

    WHEN("a different button is pressed and released")
    {
      nebula::bind::mouseButtonEvent(1, nebula::bind::action::press, modifier);
      nebula::bind::mouseButtonEvent(
          1, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a joystick/gamepad button")
  {
    bool pressed  = false;
    bool released = false;
    double delta  = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) { delta += d; },
        "Category",
        "Name");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindJButton(0, 0, modifier);

    WHEN("the button is pressed and released")
    {
      // just that the same integer is used when sending the key pressed event
      nebula::bind::joystickButtonEvent(
          0, 0, nebula::bind::action::press, modifier);
      nebula::bind::joystickButtonEvent(
          0, 0, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(pressed == true);
        REQUIRE(released == true);
      }
    }

    WHEN("a different button is pressed and released")
    {
      nebula::bind::joystickButtonEvent(
          0, 1, nebula::bind::action::press, modifier);
      nebula::bind::joystickButtonEvent(
          0, 1, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }

    WHEN("the right button on the wrong controller is pressed and released")
    {
      nebula::bind::joystickButtonEvent(
          1, 0, nebula::bind::action::press, modifier);
      nebula::bind::joystickButtonEvent(
          1, 0, nebula::bind::action::release, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a mouse axis")
  {
    bool pressed  = false;
    bool released = false;
    double delta  = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) { delta += d; },
        "Category",
        "Name");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindMAxis(0, modifier);

    WHEN("the axis is moved")
    {
      // just that the same integer is used when sending the key pressed event
      nebula::bind::mouseAxisEvent(0, 1.0, modifier);

      THEN("the deltaHandler code should have run")
      {
        REQUIRE(delta == 1.0);
      }
    }

    WHEN("a different axis is moved")
    {
      nebula::bind::mouseAxisEvent(1, 1.0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(delta == 0.0);
      }
    }
  }

  GIVEN("a bind object bound to a joystick/controller axis")
  {
    bool pressed  = false;
    bool released = false;
    double delta  = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) { delta += d; },
        "Category",
        "Name");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindJAxis(0, 0, modifier);

    WHEN("the axis is moved")
    {
      // just that the same integer is used when sending the key pressed event
      nebula::bind::joystickAxisEvent(0, 0, 1.0, modifier);

      THEN("the deltaHandler code should have run")
      {
        REQUIRE(delta == 1.0);
      }
    }

    WHEN("a different axis is moved")
    {
      nebula::bind::joystickAxisEvent(0, 1, 1.0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(delta == 0.0);
      }
    }

    WHEN("the right axis on a different controller is moved")
    {
      nebula::bind::joystickAxisEvent(1, 0, 1.0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(delta == 0.0);
      }
    }
  }
}
#endif

namespace nebula {

void bind::bindKey(int key, modifier mods)
{
  unbind();
  auto k              = std::make_tuple(key, mods.bits);
  const auto &keyBind = _keyBinds.find(k);
  if (keyBind != _keyBinds.end())
    keyBind->second.lock()->unbind();
  _keyBinds[k] = _bindList[combinedName()];
  _key         = key;
  _modifiers   = mods;
  _bound       = true;
}

void bind::bindMButton(int mBtn, modifier mods)
{
  unbind();
  auto k               = std::make_tuple(mBtn, mods.bits);
  const auto &mBtnBind = _mBtnBinds.find(k);
  if (mBtnBind != _mBtnBinds.end())
    mBtnBind->second.lock()->unbind();
  _mBtnBinds[k] = _bindList[combinedName()];
  _mBtn         = mBtn;
  _modifiers    = mods;
  _bound        = true;
}

void bind::bindMAxis(int mAxis, modifier mods)
{
  unbind();
  auto k                = std::make_tuple(mAxis, mods.bits);
  const auto &mAxisBind = _mAxisBinds.find(k);
  if (mAxisBind != _mAxisBinds.end())
    mAxisBind->second.lock()->unbind();
  _mAxisBinds[k] = _bindList[combinedName()];
  _mAxis         = mAxis;
  _modifiers     = mods;
  _bound         = true;
}

void bind::bindJButton(int jid, int jBtn, modifier mods)
{
  unbind();
  auto k               = std::make_tuple(jid, jBtn, mods.bits);
  const auto &jBtnBind = _jBtnBinds.find(k);
  if (jBtnBind != _jBtnBinds.end())
    jBtnBind->second.lock()->unbind();
  _jBtnBinds[k] = _bindList[combinedName()];
  _jid          = jid;
  _jBtn         = jBtn;
  _modifiers    = mods;
  _bound        = true;
}

void bind::bindJAxis(int jid, int jAxis, modifier mods)
{
  unbind();
  auto k                = std::make_tuple(jid, jAxis, mods.bits);
  const auto &jAxisBind = _jAxisBinds.find(k);
  if (jAxisBind != _jAxisBinds.end())
    jAxisBind->second.lock()->unbind();
  _jAxisBinds[k] = _bindList[combinedName()];
  _jid           = jid;
  _jAxis         = jAxis;
  _modifiers     = mods;
  _bound         = true;
}

void bind::unbind()
{
  _bound = false;
  if (_key) {
    _keyBinds.erase(std::make_tuple(_key.value(), _modifiers.bits));
    _key.reset();
    return;
  }
  if (_mBtn) {
    _mBtnBinds.erase(std::make_tuple(_mBtn.value(), _modifiers.bits));
    _mBtn.reset();
    return;
  }
  if (_mAxis) {
    _mAxisBinds.erase(std::make_tuple(_mAxis.value(), _modifiers.bits));
    _mAxis.reset();
    return;
  }
  if (_jBtn) {
    _jBtnBinds.erase(
        std::make_tuple(_jid.value(), _jBtn.value(), _modifiers.bits));
    _jid.reset();
    _jBtn.reset();
    return;
  }
  if (_jAxis) {
    _jAxisBinds.erase(
        std::make_tuple(_jid.value(), _jAxis.value(), _modifiers.bits));
    _jid.reset();
    _jAxis.reset();
    return;
  }
}

void bind::press()
{
  if (_pressHandler)
    _pressHandler();
}

void bind::release()
{
  if (_releaseHandler)
    _releaseHandler();
}

void bind::delta(double delta)
{
  if (_deltaHandler)
    _deltaHandler(delta);
}

void bind::keyboardEvent(int key, action event, modifier mods)
{
  const auto &keyBind = _keyBinds.find(std::make_tuple(key, mods.bits));
  if (keyBind != _keyBinds.end()) {
    if (event == action::press)
      keyBind->second.lock()->press();
    else
      keyBind->second.lock()->release();
  }
}

void bind::mouseButtonEvent(int mBtn, action event, modifier mods)
{
  const auto &mBtnBind = _mBtnBinds.find(std::make_tuple(mBtn, mods.bits));
  if (mBtnBind != _mBtnBinds.end()) {
    if (event == action::press)
      mBtnBind->second.lock()->press();
    else
      mBtnBind->second.lock()->release();
  }
}

void bind::mouseAxisEvent(int mAxis, double delta, modifier mods)
{
  const auto &mAxisBind = _mAxisBinds.find(std::make_tuple(mAxis, mods.bits));
  if (mAxisBind != _mAxisBinds.end()) {
    mAxisBind->second.lock()->delta(delta);
  }
}

void bind::joystickButtonEvent(int jid, int jBtn, action event, modifier mods)
{
  const auto &jBtnBind = _jBtnBinds.find(std::make_tuple(jid, jBtn, mods.bits));
  if (jBtnBind != _jBtnBinds.end()) {
    if (event == action::press)
      jBtnBind->second.lock()->press();
    else
      jBtnBind->second.lock()->release();
  }
}

void bind::joystickAxisEvent(int jid, int jAxis, double delta, modifier mods)
{
  const auto &jAxisBind
      = _jAxisBinds.find(std::make_tuple(jid, jAxis, mods.bits));
  if (jAxisBind != _jAxisBinds.end()) {
    jAxisBind->second.lock()->delta(delta);
  }
}

std::shared_ptr<bind> bind::create(std::function<void()> pressHandler,
    std::function<void()> releaseHandler,
    std::function<void(double)> deltaHandler,
    const std::string &category,
    const std::string &name)
{
  std::string key = category + ":" + name;
  _bindList.emplace(key,
      new bind(pressHandler, releaseHandler, deltaHandler, category, name));
  return _bindList[key];
}

std::map<std::tuple<int, int>, std::weak_ptr<bind>> bind::_keyBinds;
std::map<std::tuple<int, int>, std::weak_ptr<bind>> bind::_mBtnBinds;
std::map<std::tuple<int, int>, std::weak_ptr<bind>> bind::_mAxisBinds;
std::map<std::tuple<int, int, int>, std::weak_ptr<bind>> bind::_jBtnBinds;
std::map<std::tuple<int, int, int>, std::weak_ptr<bind>> bind::_jAxisBinds;
std::map<std::string, std::shared_ptr<bind>> bind::_bindList;

} // namespace nebula
