// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "bind.h"

#include "loguru.hpp"
// Unit Testing includes
#include "doctest.h"

#ifndef DOCTEST_CONFIG_DISABLE
  #include <iostream>

SCENARIO("class bind")
{
  GIVEN("a bind object bound to a key")
  {
    LOG_S(INFO) << "FOO 1";
    bool pressed  = false;
    bool released = false;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) {},
        "Category",
        "Keyboard Test");
    LOG_S(INFO) << "FOO 2";
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bind->bindKey(0, modifier);
    LOG_S(INFO) << "FOO 3";

    THEN("it should report as bound")
    {
      LOG_S(INFO) << "FOO 4";
      REQUIRE(bind->bound() == true);
      LOG_S(INFO) << "FOO 5";
      REQUIRE(!!nebula::bind::findBind("Category", "Keyboard Test"));
      LOG_S(INFO) << "FOO 6";
      REQUIRE(!!nebula::bind::findKey(0, modifier));
      LOG_S(INFO) << "FOO 7";
      REQUIRE(nebula::bind::findKey(0, modifier).get() == bind.get());
      LOG_S(INFO) << "FOO 8";
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      LOG_S(INFO) << "FOO 9";
      REQUIRE(bind->combinedName() == "Category:Keyboard Test");
      LOG_S(INFO) << "FOO 10";
    }

    WHEN("the key is pressed and released")
    {
      LOG_S(INFO) << "FOO 11";
      // just that the same integer is used when sending the key pressed event
      nebula::bind::keyboardEvent(0, nebula::bind::action::press, modifier);
      LOG_S(INFO) << "FOO 12";
      nebula::bind::keyboardEvent(0, nebula::bind::action::release, modifier);
      LOG_S(INFO) << "FOO 13";

      THEN("the pressHandler and releaseHandler code should have run")
      {
        LOG_S(INFO) << "FOO 14";
        REQUIRE(pressed == true);
        LOG_S(INFO) << "FOO 15";
        REQUIRE(released == true);
        LOG_S(INFO) << "FOO 16";
      }
      pressed  = false;
      released = false;
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
        REQUIRE(!!nebula::bind::findBind("Category", "Keyboard Test"));
        REQUIRE(!nebula::bind::findKey(0, modifier));
        REQUIRE(nebula::bind::findKey(0, modifier).get() != bind.get());
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
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) {},
        "Category",
        "Mouse Button Test");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindMButton(0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bind->bound() == true);
      REQUIRE(!!nebula::bind::findBind("Category", "Mouse Button Test"));
      REQUIRE(!!nebula::bind::findMButton(0, modifier));
      REQUIRE(nebula::bind::findMButton(0, modifier).get() == bind.get());
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind->combinedName() == "Category:Mouse Button Test");
    }

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
      pressed  = false;
      released = false;
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

    WHEN("it is unbound")
    {
      bind->unbind();

      THEN("it should report as unbound")
      {
        REQUIRE(bind->bound() == false);
        REQUIRE(!!nebula::bind::findBind("Category", "Mouse Button Test"));
        REQUIRE(!nebula::bind::findMButton(0, modifier));
        REQUIRE(nebula::bind::findMButton(0, modifier).get() != bind.get());
      }

      THEN("it should not react to the button it was bound to")
      {
        nebula::bind::mouseButtonEvent(
            0, nebula::bind::action::press, modifier);
        nebula::bind::mouseButtonEvent(
            0, nebula::bind::action::release, modifier);
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a joystick/gamepad button")
  {
    bool pressed  = false;
    bool released = false;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() { pressed = true; },
        [&]() { released = true; },
        [&](double d) {},
        "Category",
        "Joystick Button Test");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindJButton(0, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bind->bound() == true);
      REQUIRE(!!nebula::bind::findBind("Category", "Joystick Button Test"));
      REQUIRE(!!nebula::bind::findJButton(0, 0, modifier));
      REQUIRE(nebula::bind::findJButton(0, 0, modifier).get() == bind.get());
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind->combinedName() == "Category:Joystick Button Test");
    }

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
      pressed  = false;
      released = false;
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

    WHEN("it is unbound")
    {
      bind->unbind();

      THEN("it should report as unbound")
      {
        REQUIRE(bind->bound() == false);
        REQUIRE(!!nebula::bind::findBind("Category", "Joystick Button Test"));
        REQUIRE(!nebula::bind::findJButton(0, 0, modifier));
        REQUIRE(nebula::bind::findJButton(0, 0, modifier).get() != bind.get());
      }

      THEN("it should not react to the button it was bound to")
      {
        nebula::bind::joystickButtonEvent(
            0, 0, nebula::bind::action::press, modifier);
        nebula::bind::joystickButtonEvent(
            0, 0, nebula::bind::action::release, modifier);
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a mouse axis")
  {
    bool handled = false;
    double delta = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() {},
        [&]() {},
        [&](double d) {
          handled = true;
          delta += d;
        },
        "Category",
        "Mouse Axis Test");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindMAxis(0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bind->bound() == true);
      REQUIRE(!!nebula::bind::findBind("Category", "Mouse Axis Test"));
      REQUIRE(!!nebula::bind::findMAxis(0, modifier));
      REQUIRE(nebula::bind::findMAxis(0, modifier).get() == bind.get());
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind->combinedName() == "Category:Mouse Axis Test");
    }

    WHEN("the axis is moved")
    {
      // just that the same integer is used when sending the key pressed event
      nebula::bind::mouseAxisEvent(0, 1.0, modifier);

      THEN("the deltaHandler code should have run")
      {
        REQUIRE(handled == true);
        REQUIRE(delta == 1.0);
      }
      handled = false;
    }

    WHEN("a different axis is moved")
    {
      nebula::bind::mouseAxisEvent(1, 1.0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }

    WHEN("it is unbound")
    {
      bind->unbind();

      THEN("it should report as unbound")
      {
        REQUIRE(bind->bound() == false);
        REQUIRE(!!nebula::bind::findBind("Category", "Mouse Axis Test"));
        REQUIRE(!nebula::bind::findMAxis(0, modifier));
        REQUIRE(nebula::bind::findMAxis(0, modifier).get() != bind.get());
      }

      THEN("it should not react to the axis it was bound to")
      {
        nebula::bind::mouseAxisEvent(0, 1.0, modifier);
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }
  }

  GIVEN("a bind object bound to a joystick/controller axis")
  {
    bool handled = false;
    double delta = 0.0;
    nebula::bind::modifier modifier;
    auto bind = nebula::bind::create([&]() {},
        [&]() {},
        [&](double d) {
          handled = true;
          delta += d;
        },
        "Category",
        "Joystick Axis Test");
    // the integer used as the mouse button is intended to be a GLFW constant,
    // but its value doesn't matter for testing purposes
    bind->bindJAxis(0, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bind->bound() == true);
      REQUIRE(!!nebula::bind::findBind("Category", "Joystick Axis Test"));
      REQUIRE(!!nebula::bind::findJAxis(0, 0, modifier));
      REQUIRE(nebula::bind::findJAxis(0, 0, modifier).get() == bind.get());
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind->combinedName() == "Category:Joystick Axis Test");
    }

    WHEN("the axis is moved")
    {
      handled = false;
      // just that the same integer is used when sending the key pressed event
      nebula::bind::joystickAxisEvent(0, 0, 1.0, modifier);

      THEN("the deltaHandler code should have run")
      {
        REQUIRE(handled == true);
        REQUIRE(delta == 1.0);
      }
    }

    WHEN("a different axis is moved")
    {
      handled = false;
      nebula::bind::joystickAxisEvent(0, 1, 1.0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }

    WHEN("the right axis on a different controller is moved")
    {
      handled = false;
      nebula::bind::joystickAxisEvent(1, 0, 1.0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }

    WHEN("it is unbound")
    {
      bind->unbind();

      THEN("it should report as unbound")
      {
        REQUIRE(bind->bound() == false);
        REQUIRE(!!nebula::bind::findBind("Category", "Joystick Axis Test"));
        REQUIRE(!nebula::bind::findJAxis(0, 0, modifier));
        REQUIRE(nebula::bind::findJAxis(0, 0, modifier).get() != bind.get());
      }

      THEN("it should not react to the axis it was bound to")
      {
        nebula::bind::joystickAxisEvent(0, 0, 1.0, modifier);
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }
  }

  GIVEN("multiple bind objects mapped to input events")
  {
    nebula::bind::modifier modifier;
    auto bind1 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Keyboard Test");
    auto bind2 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Mouse Button Test");
    auto bind3 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Joystick Button Test");
    auto bind4 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Mouse Axis Test");
    auto bind5 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Joystick Axis Test");
    bind1->bindKey(0, modifier);
    bind2->bindMButton(0, modifier);
    bind3->bindJButton(0, 0, modifier);
    bind4->bindMAxis(0, modifier);
    bind5->bindJAxis(0, 0, modifier);
    WHEN("all binds are cleared")
    {
      nebula::bind::clearBinds();
      THEN("none of the binds should be found")
      {
        REQUIRE(nebula::bind::findKey(0, modifier) == nullptr);
        REQUIRE(nebula::bind::findBind("Category", "Keyboard Test") == nullptr);
        REQUIRE(nebula::bind::findMButton(0, modifier) == nullptr);
        REQUIRE(
            nebula::bind::findBind("Category", "Mouse Button Test") == nullptr);
        REQUIRE(nebula::bind::findJButton(0, 0, modifier) == nullptr);
        REQUIRE(nebula::bind::findBind("Category", "Joystick Button Test")
                == nullptr);
        REQUIRE(nebula::bind::findMAxis(0, modifier) == nullptr);
        REQUIRE(
            nebula::bind::findBind("Category", "Mouse Axis Test") == nullptr);
        REQUIRE(nebula::bind::findJAxis(0, 0, modifier) == nullptr);
        REQUIRE(nebula::bind::findBind("Category", "Joystick Axis Test")
                == nullptr);
      }
    }
  }

  GIVEN("multiple bind objects mapped to input events")
  {
    nebula::bind::modifier modifier;
    auto bind1 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Keyboard Test");
    auto bind2 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Mouse Button Test");
    auto bind3 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Joystick Button Test");
    auto bind4 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Mouse Axis Test");
    auto bind5 = nebula::bind::create(
        nullptr, nullptr, nullptr, "Category", "Joystick Axis Test");
    bind1->bindKey(0, modifier);
    bind2->bindMButton(0, modifier);
    bind3->bindJButton(0, 0, modifier);
    bind4->bindMAxis(0, modifier);
    bind5->bindJAxis(0, 0, modifier);
    WHEN("each event is mapped to new bind objects")
    {
      auto bind6 = nebula::bind::create(
          nullptr, nullptr, nullptr, "Category", "Keyboard Test 2");
      auto bind7 = nebula::bind::create(
          nullptr, nullptr, nullptr, "Category", "Mouse Button Test 2");
      auto bind8 = nebula::bind::create(
          nullptr, nullptr, nullptr, "Category", "Joystick Button Test 2");
      auto bind9 = nebula::bind::create(
          nullptr, nullptr, nullptr, "Category", "Mouse Axis Test 2");
      auto bind10 = nebula::bind::create(
          nullptr, nullptr, nullptr, "Category", "Joystick Axis Test 2");
      bind6->bindKey(0, modifier);
      bind7->bindMButton(0, modifier);
      bind8->bindJButton(0, 0, modifier);
      bind9->bindMAxis(0, modifier);
      bind10->bindJAxis(0, 0, modifier);
      THEN("the new binds should be found instead")
      {
        REQUIRE(nebula::bind::findKey(0, modifier) == bind6);
        REQUIRE(nebula::bind::findMButton(0, modifier) == bind7);
        REQUIRE(nebula::bind::findJButton(0, 0, modifier) == bind8);
        REQUIRE(nebula::bind::findMAxis(0, modifier) == bind9);
        REQUIRE(nebula::bind::findJAxis(0, 0, modifier) == bind10);
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

bool bind::press()
{
  if (_pressHandler) {
    _pressHandler();
    LOG_S(INFO) << "_pressHandler() executed";
    return true;
  } else {
    LOG_S(INFO) << "_pressHandler() not executed";
    return false;
  }
}

bool bind::release()
{
  if (_releaseHandler) {
    _releaseHandler();
    LOG_S(INFO) << "_releaseHandler() executed";
    return true;
  } else {
    LOG_S(INFO) << "_releaseHandler() not executed";
    return false;
  }
}

bool bind::delta(double delta)
{
  if (_deltaHandler) {
    _deltaHandler(delta);
    LOG_S(INFO) << "_deltaHandler() executed";
    return true;
  } else {
    LOG_S(INFO) << "_deltaHandler() not executed";
    return false;
  }
}

void bind::keyboardEvent(int key, action event, modifier mods)
{
  const auto keyBind = findKey(key, mods);
  if (keyBind) {
    LOG_S(INFO) << "Found: " << keyBind->_category << ": " << keyBind->_name;
    if (event == action::press)
      keyBind->press();
    else
      keyBind->release();
  }
}

void bind::mouseButtonEvent(int mBtn, action event, modifier mods)
{
  const auto mBtnBind = findMButton(mBtn, mods);
  if (mBtnBind) {
    LOG_S(INFO) << "Found: " << mBtnBind->_category << ": " << mBtnBind->_name;
    if (event == action::press)
      mBtnBind->press();
    else
      mBtnBind->release();
  }
}

void bind::mouseAxisEvent(int mAxis, double delta, modifier mods)
{
  const auto mAxisBind = findMAxis(mAxis, mods);
  if (mAxisBind) {
    LOG_S(INFO) << "Found: " << mAxisBind->_category << ": "
                << mAxisBind->_name;
    mAxisBind->delta(delta);
  }
}

void bind::joystickButtonEvent(int jid, int jBtn, action event, modifier mods)
{
  const auto jBtnBind = findJButton(jid, jBtn, mods);
  if (jBtnBind) {
    LOG_S(INFO) << "Found: " << jBtnBind->_category << ": " << jBtnBind->_name;
    if (event == action::press)
      jBtnBind->press();
    else
      jBtnBind->release();
  }
}

void bind::joystickAxisEvent(int jid, int jAxis, double delta, modifier mods)
{
  const auto jAxisBind = findJAxis(jid, jAxis, mods);
  if (jAxisBind) {
    LOG_S(INFO) << "Found: " << jAxisBind->_category << ": "
                << jAxisBind->_name;
    jAxisBind->delta(delta);
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

std::shared_ptr<bind> bind::findKey(int key, modifier mods)
{
  auto b = _keyBinds.find(std::make_tuple(key, mods.bits));
  if (b == _keyBinds.end())
    return nullptr;
  return b->second.lock();
}

std::shared_ptr<bind> bind::findMButton(int mBtn, modifier mods)
{
  auto b = _mBtnBinds.find(std::make_tuple(mBtn, mods.bits));
  if (b == _mBtnBinds.end())
    return nullptr;
  return b->second.lock();
}

std::shared_ptr<bind> bind::findMAxis(int mAxis, modifier mods)
{
  auto b = _mAxisBinds.find(std::make_tuple(mAxis, mods.bits));
  if (b == _mAxisBinds.end())
    return nullptr;
  return b->second.lock();
}

std::shared_ptr<bind> bind::findJButton(int jid, int jBtn, modifier mods)
{
  auto b = _jBtnBinds.find(std::make_tuple(jid, jBtn, mods.bits));
  if (b == _jBtnBinds.end())
    return nullptr;
  return b->second.lock();
}

std::shared_ptr<bind> bind::findJAxis(int jid, int jAxis, modifier mods)
{
  auto b = _jAxisBinds.find(std::make_tuple(jid, jAxis, mods.bits));
  if (b == _jAxisBinds.end())
    return nullptr;
  return b->second.lock();
}

std::shared_ptr<bind> bind::findBind(
    const std::string &category, const std::string &name)
{
  auto b = _bindList.find(category + ":" + name);
  if (b == _bindList.end())
    return nullptr;
  return b->second;
}

void bind::clearBinds()
{
  _keyBinds.clear();
  _mBtnBinds.clear();
  _mAxisBinds.clear();
  _jBtnBinds.clear();
  _jAxisBinds.clear();
  _bindList.clear();
}

std::map<std::tuple<int, int>, std::weak_ptr<bind>> bind::_keyBinds;
std::map<std::tuple<int, int>, std::weak_ptr<bind>> bind::_mBtnBinds;
std::map<std::tuple<int, int>, std::weak_ptr<bind>> bind::_mAxisBinds;
std::map<std::tuple<int, int, int>, std::weak_ptr<bind>> bind::_jBtnBinds;
std::map<std::tuple<int, int, int>, std::weak_ptr<bind>> bind::_jAxisBinds;
std::map<std::string, std::shared_ptr<bind>> bind::_bindList;

} // namespace nebula
