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
    bool pressed  = false;
    bool released = false;
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind = bindMgr.create(
        [&]() {
          LOG_S(INFO) << "press event";
          pressed = true;
        },
        [&]() {
          LOG_S(INFO) << "release event";
          released = true;
        },
        [&](double d) {},
        "Category",
        "Keyboard Test");
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bindMgr.bindKey(bind, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bindMgr.bound(bind) == true);
      REQUIRE(bindMgr.bindExists("Category", "Keyboard Test"));
      REQUIRE(bindMgr.keyBindExists(0, modifier));
      REQUIRE(bindMgr.keyBindIs(0, modifier, bind));
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind.combinedName() == "Category:Keyboard Test");
    }

    WHEN("the key is pressed and released")
    {
      pressed  = false;
      released = false;
      // just that the same integer is used when sending the key pressed event
      bindMgr.keyboardPress(0, modifier);
      bindMgr.keyboardRelease(0, modifier);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(pressed == true);
        REQUIRE(released == true);
      }
    }

    WHEN("a different key is pressed and released")
    {
      pressed  = false;
      released = false;
      bindMgr.keyboardPress(1, modifier);
      bindMgr.keyboardRelease(1, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }

    WHEN("it is unbound")
    {
      bindMgr.unbind(bind);

      THEN("it should report as unbound")
      {
        REQUIRE(bindMgr.bound(bind) == false);
        REQUIRE(!bindMgr.keyBindExists(0, modifier));
        REQUIRE(!bindMgr.keyBindIs(0, modifier, bind));
      }

      THEN("it should not react to the key it was bound to")
      {
        pressed  = false;
        released = false;
        bindMgr.keyboardPress(0, modifier);
        bindMgr.keyboardRelease(0, modifier);
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a mouse button")
  {
    bool pressed  = false;
    bool released = false;
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind = bindMgr.create(
        [&]() {
          LOG_S(INFO) << "press event";
          pressed = true;
        },
        [&]() {
          LOG_S(INFO) << "release event";
          released = true;
        },
        [&](double d) {},
        "Category",
        "MButton Test");
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bindMgr.bindMBtn(bind, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bindMgr.bound(bind) == true);
      REQUIRE(bindMgr.bindExists("Category", "MButton Test"));
      REQUIRE(bindMgr.mBtnBindExists(0, modifier));
      REQUIRE(bindMgr.mBtnBindIs(0, modifier, bind));
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind.combinedName() == "Category:MButton Test");
    }

    WHEN("the mouse button is pressed and released")
    {
      pressed  = false;
      released = false;
      // just that the same integer is used when sending the key pressed event
      bindMgr.mButtonPress(0, modifier);
      bindMgr.mButtonRelease(0, modifier);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(pressed == true);
        REQUIRE(released == true);
      }
    }

    WHEN("a different mouse button is pressed and released")
    {
      pressed  = false;
      released = false;
      bindMgr.mButtonPress(1, modifier);
      bindMgr.mButtonRelease(1, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }

    WHEN("it is unbound")
    {
      bindMgr.unbind(bind);

      THEN("it should report as unbound")
      {
        REQUIRE(bindMgr.bound(bind) == false);
        REQUIRE(!bindMgr.mBtnBindExists(0, modifier));
        REQUIRE(!bindMgr.mBtnBindIs(0, modifier, bind));
      }

      THEN("it should not react to the mouse button it was bound to")
      {
        pressed  = false;
        released = false;
        bindMgr.mButtonPress(0, modifier);
        bindMgr.mButtonRelease(0, modifier);
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a controller button")
  {
    bool pressed  = false;
    bool released = false;
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind = bindMgr.create(
        [&]() {
          LOG_S(INFO) << "press event";
          pressed = true;
        },
        [&]() {
          LOG_S(INFO) << "release event";
          released = true;
        },
        [&](double d) {},
        "Category",
        "CButton Test");
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bindMgr.bindCBtn(bind, 0, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bindMgr.bound(bind) == true);
      REQUIRE(bindMgr.bindExists("Category", "CButton Test"));
      REQUIRE(bindMgr.cBtnBindExists(0, 0, modifier));
      REQUIRE(bindMgr.cBtnBindIs(0, 0, modifier, bind));
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind.combinedName() == "Category:CButton Test");
    }

    WHEN("the controller button is pressed and released")
    {
      pressed  = false;
      released = false;
      // just that the same integer is used when sending the key pressed event
      bindMgr.cButtonPress(0, 0, modifier);
      bindMgr.cButtonRelease(0, 0, modifier);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(pressed == true);
        REQUIRE(released == true);
      }
    }

    WHEN("a different controller button is pressed and released")
    {
      pressed  = false;
      released = false;
      bindMgr.cButtonPress(1, 0, modifier);
      bindMgr.cButtonRelease(1, 0, modifier);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }

    WHEN("it is unbound")
    {
      bindMgr.unbind(bind);

      THEN("it should report as unbound")
      {
        REQUIRE(bindMgr.bound(bind) == false);
        REQUIRE(!bindMgr.cBtnBindExists(0, 0, modifier));
        REQUIRE(!bindMgr.cBtnBindIs(0, 0, modifier, bind));
      }

      THEN("it should not react to the controller button it was bound to")
      {
        pressed  = false;
        released = false;
        bindMgr.cButtonPress(0, 0, modifier);
        bindMgr.cButtonRelease(0, 0, modifier);
        REQUIRE(pressed == false);
        REQUIRE(released == false);
      }
    }
  }

  GIVEN("a bind object bound to a mouse axis")
  {
    bool handled = false;
    double delta = 0.0;
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind = bindMgr.create([&]() {},
        [&]() {},
        [&](double d) {
          LOG_S(INFO) << "delta event";
          handled = true;
          delta += d;
        },
        "Category",
        "MAxis Test");
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bindMgr.bindMAxis(bind, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bindMgr.bound(bind) == true);
      REQUIRE(bindMgr.bindExists("Category", "MAxis Test"));
      REQUIRE(bindMgr.mAxisBindExists(0, modifier));
      REQUIRE(bindMgr.mAxisBindIs(0, modifier, bind));
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind.combinedName() == "Category:MAxis Test");
    }

    WHEN("the mouse axis is moved")
    {
      handled = false;
      delta   = 0.0;
      // just that the same integer is used when sending the key pressed event
      bindMgr.mAxisDelta(0, modifier, 1.0);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(handled == true);
        REQUIRE(delta == 1.0);
      }
    }

    WHEN("a different mouse axis is moved")
    {
      handled = false;
      delta   = 0.0;
      bindMgr.mAxisDelta(1, modifier, 1.0);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }

    WHEN("it is unbound")
    {
      bindMgr.unbind(bind);

      THEN("it should report as unbound")
      {
        REQUIRE(bindMgr.bound(bind) == false);
        REQUIRE(!bindMgr.mAxisBindExists(0, modifier));
        REQUIRE(!bindMgr.mAxisBindIs(0, modifier, bind));
      }

      THEN("it should not react to the mouse axis it was bound to")
      {
        handled = false;
        delta   = 0.0;
        bindMgr.mAxisDelta(0, modifier, 1.0);
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }
  }

  GIVEN("a bind object bound to a controller axis")
  {
    bool handled = false;
    double delta = 0.0;
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind = bindMgr.create([&]() {},
        [&]() {},
        [&](double d) {
          LOG_S(INFO) << "delta event";
          handled = true;
          delta += d;
        },
        "Category",
        "CAxis Test");
    // the integer used as the key is intended to be a GLFW constant, but its
    // value doesn't matter for testing purposes
    bindMgr.bindCAxis(bind, 0, 0, modifier);

    THEN("it should report as bound")
    {
      REQUIRE(bindMgr.bound(bind) == true);
      REQUIRE(bindMgr.bindExists("Category", "CAxis Test"));
      REQUIRE(bindMgr.cAxisBindExists(0, 0, modifier));
      REQUIRE(bindMgr.cAxisBindIs(0, 0, modifier, bind));
    }

    THEN("it should report its name in <Category>:<Name> format")
    {
      REQUIRE(bind.combinedName() == "Category:CAxis Test");
    }

    WHEN("the controller axis is moved")
    {
      handled = false;
      delta   = 0.0;
      // just that the same integer is used when sending the key pressed event
      bindMgr.cAxisDelta(0, 0, modifier, 1.0);

      THEN("the pressHandler and releaseHandler code should have run")
      {
        REQUIRE(handled == true);
        REQUIRE(delta == 1.0);
      }
    }

    WHEN("a different controller axis is moved")
    {
      handled = false;
      delta   = 0.0;
      bindMgr.cAxisDelta(1, 0, modifier, 1.0);

      THEN("the pressHandler and releaseHandler code should not have run")
      {
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }

    WHEN("it is unbound")
    {
      bindMgr.unbind(bind);

      THEN("it should report as unbound")
      {
        REQUIRE(bindMgr.bound(bind) == false);
        REQUIRE(!bindMgr.cAxisBindExists(0, 0, modifier));
        REQUIRE(!bindMgr.cAxisBindIs(0, 0, modifier, bind));
      }

      THEN("it should not react to the controller axis it was bound to")
      {
        handled = false;
        delta   = 0.0;
        bindMgr.cAxisDelta(0, 0, modifier, 1.0);
        REQUIRE(handled == false);
        REQUIRE(delta == 0.0);
      }
    }
  }

  GIVEN("multiple bind objects mapped to input events")
  {
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind1 = bindMgr.create(
        nullptr, nullptr, nullptr, "Category", "Keyboard Test");
    auto bind2
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "MButton Test");
    auto bind3
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "CButton Test");
    auto bind4
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "MAxis Test");
    auto bind5
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "CAxis Test");
    bindMgr.bindKey(bind1, 0, modifier);
    bindMgr.bindMBtn(bind2, 0, modifier);
    bindMgr.bindCBtn(bind3, 0, 0, modifier);
    bindMgr.bindMAxis(bind4, 0, modifier);
    bindMgr.bindCAxis(bind5, 0, 0, modifier);
    WHEN("all binds are cleared")
    {
      REQUIRE_NOTHROW(bindMgr.clearBinds());
      THEN("none of the binds should be found")
      {
        REQUIRE(!bindMgr.bindExists("Category", "Keyboard Test"));
        REQUIRE(!bindMgr.bindExists("Category", "MButton Test"));
        REQUIRE(!bindMgr.bindExists("Category", "CButton Test"));
        REQUIRE(!bindMgr.bindExists("Category", "MAxis Test"));
        REQUIRE(!bindMgr.bindExists("Category", "CAxis Test"));
        REQUIRE(!bindMgr.keyBindExists(0, modifier));
        REQUIRE(!bindMgr.mBtnBindExists(0, modifier));
        REQUIRE(!bindMgr.cBtnBindExists(0, 0, modifier));
        REQUIRE(!bindMgr.mAxisBindExists(0, modifier));
        REQUIRE(!bindMgr.cAxisBindExists(0, 0, modifier));
      }
    }
  }

  GIVEN("multiple bind objects mapped to input events")
  {
    nebula::bindManager bindMgr;
    nebula::bindManager::modifier modifier;
    auto bind1 = bindMgr.create(
        nullptr, nullptr, nullptr, "Category", "Keyboard Test");
    auto bind2
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "MButton Test");
    auto bind3
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "CButton Test");
    auto bind4
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "MAxis Test");
    auto bind5
        = bindMgr.create(nullptr, nullptr, nullptr, "Category", "CAxis Test");
    bindMgr.bindKey(bind1, 0, modifier);
    bindMgr.bindMBtn(bind2, 0, modifier);
    bindMgr.bindCBtn(bind3, 0, 0, modifier);
    bindMgr.bindMAxis(bind4, 0, modifier);
    bindMgr.bindCAxis(bind5, 0, 0, modifier);
    WHEN("each event is mapped to new bind objects")
    {
      auto bind6 = bindMgr.create(
          nullptr, nullptr, nullptr, "Category", "Keyboard Test 2");
      auto bind7 = bindMgr.create(
          nullptr, nullptr, nullptr, "Category", "MButton Test 2");
      auto bind8 = bindMgr.create(
          nullptr, nullptr, nullptr, "Category", "CButton Test 2");
      auto bind9 = bindMgr.create(
          nullptr, nullptr, nullptr, "Category", "MAxis Test 2");
      auto bind10 = bindMgr.create(
          nullptr, nullptr, nullptr, "Category", "CAxis Test 2");
      bindMgr.bindKey(bind6, 0, modifier);
      bindMgr.bindMBtn(bind7, 0, modifier);
      bindMgr.bindCBtn(bind8, 0, 0, modifier);
      bindMgr.bindMAxis(bind9, 0, modifier);
      bindMgr.bindCAxis(bind10, 0, 0, modifier);
      THEN("the new binds should be found instead")
      {
        REQUIRE(bindMgr.keyBindIs(0, modifier, bind6));
        REQUIRE(bindMgr.mBtnBindIs(0, modifier, bind7));
        REQUIRE(bindMgr.cBtnBindIs(0, 0, modifier, bind8));
        REQUIRE(bindMgr.mAxisBindIs(0, modifier, bind9));
        REQUIRE(bindMgr.cAxisBindIs(0, 0, modifier, bind10));
      }
    }
  }
}
#endif

namespace nebula {

void bindManager::bind::press()
{
  if (_pressHandler)
    _pressHandler();
}

void bindManager::bind::release()
{
  if (_releaseHandler)
    _releaseHandler();
}

void bindManager::bind::delta(double d)
{
  if (_deltaHandler)
    _deltaHandler(d);
}

std::string bindManager::bind::combinedName()
{
  return _category + ":" + _name;
}

bindManager::bind &bindManager::create(std::function<void()> pressHandler,
    std::function<void()> releaseHandler,
    std::function<void(double)> deltaHandler,
    const std::string &category,
    const std::string &name)
{
  std::string key = category + ":" + name;
  _bindList.try_emplace(
      key, pressHandler, releaseHandler, deltaHandler, category, name);
  return _bindList.at(key);
}

void bindManager::bindKey(bindManager::bind &b, int key, modifier mod)
{
  auto k = std::make_pair(key, (int)mod);
  if (_keyList.count(k) > 0)
    unbind(_keyList.at(k));
  unbind(b);
  _keyList.emplace(k, b);
  b._keyBind = k;
}

void bindManager::bindMBtn(bindManager::bind &b, int mBtn, modifier mod)
{
  auto k = std::make_pair(mBtn, (int)mod);
  if (_mBtnList.count(k) > 0)
    unbind(_mBtnList.at(k));
  unbind(b);
  _mBtnList.emplace(k, b);
  b._mBtnBind = k;
}

void bindManager::bindCBtn(
    bindManager::bind &b, int cID, int cBtn, modifier mod)
{
  auto k = std::make_tuple(cID, cBtn, (int)mod);
  if (_cBtnList.count(k) > 0)
    unbind(_cBtnList.at(k));
  unbind(b);
  _cBtnList.emplace(k, b);
  b._cBtnBind = k;
}

void bindManager::bindMAxis(bindManager::bind &b, int mAxis, modifier mod)
{
  auto k = std::make_pair(mAxis, (int)mod);
  if (_mAxisList.count(k) > 0)
    unbind(_mAxisList.at(k));
  unbind(b);
  _mAxisList.emplace(k, b);
  b._mAxisBind = k;
}

void bindManager::bindCAxis(
    bindManager::bind &b, int cID, int cAxis, modifier mod)
{
  auto k = std::make_tuple(cID, cAxis, (int)mod);
  if (_cAxisList.count(k) > 0)
    unbind(_cAxisList.at(k));
  unbind(b);
  _cAxisList.emplace(k, b);
  b._cAxisBind = k;
}

bool bindManager::keyBindIs(int key, modifier mod, bindManager::bind &b)
{
  auto k = std::make_pair(key, (int)mod);
  if (_keyList.count(k) > 0)
    return &(_keyList.at(k)) == &b;
  return false;
}

bool bindManager::mBtnBindIs(int mBtn, modifier mod, bindManager::bind &b)
{
  auto k = std::make_pair(mBtn, (int)mod);
  if (_mBtnList.count(k) > 0)
    return &(_mBtnList.at(k)) == &b;
  return false;
}

bool bindManager::cBtnBindIs(
    int cID, int cBtn, modifier mod, bindManager::bind &b)
{
  auto k = std::make_tuple(cID, cBtn, (int)mod);
  if (_cBtnList.count(k) > 0)
    return &(_cBtnList.at(k)) == &b;
  return false;
}

bool bindManager::mAxisBindIs(int mAxis, modifier mod, bindManager::bind &b)
{
  auto k = std::make_pair(mAxis, (int)mod);
  if (_mAxisList.count(k) > 0)
    return &(_mAxisList.at(k)) == &b;
  return false;
}

bool bindManager::cAxisBindIs(
    int cID, int cAxis, modifier mod, bindManager::bind &b)
{
  auto k = std::make_tuple(cID, cAxis, (int)mod);
  if (_cAxisList.count(k) > 0)
    return &(_cAxisList.at(k)) == &b;
  return false;
}

bool bindManager::bindExists(
    const std::string &category, const std::string &name)
{
  return _bindList.count(category + ":" + name) > 0;
}

bool bindManager::keyBindExists(int key, modifier mod)
{
  return _keyList.count(std::make_pair(key, (int)mod)) > 0;
}

bool bindManager::mBtnBindExists(int mBtn, modifier mod)
{
  return _mBtnList.count(std::make_pair(mBtn, (int)mod)) > 0;
}

bool bindManager::cBtnBindExists(int cID, int cBtn, modifier mod)
{
  return _cBtnList.count(std::make_tuple(cID, cBtn, (int)mod)) > 0;
}

bool bindManager::mAxisBindExists(int mAxis, modifier mod)
{
  return _mAxisList.count(std::make_pair(mAxis, (int)mod)) > 0;
}

bool bindManager::cAxisBindExists(int cID, int cAxis, modifier mod)
{
  return _cAxisList.count(std::make_tuple(cID, cAxis, (int)mod)) > 0;
}

void bindManager::keyboardPress(int key, modifier mod)
{
  auto k = std::make_pair(key, (int)mod);
  if (_keyList.count(k) > 0)
    _keyList.at(k).press();
}

void bindManager::keyboardRelease(int key, modifier mod)
{
  auto k = std::make_pair(key, (int)mod);
  if (_keyList.count(k) > 0)
    _keyList.at(k).release();
}

void bindManager::mButtonPress(int mBtn, modifier mod)
{
  auto k = std::make_pair(mBtn, (int)mod);
  if (_mBtnList.count(k) > 0)
    _mBtnList.at(k).press();
}

void bindManager::mButtonRelease(int mBtn, modifier mod)
{
  auto k = std::make_pair(mBtn, (int)mod);
  if (_mBtnList.count(k) > 0)
    _mBtnList.at(k).release();
}

void bindManager::cButtonPress(int cID, int cBtn, modifier mod)
{
  auto k = std::make_tuple(cID, cBtn, (int)mod);
  if (_cBtnList.count(k) > 0)
    _cBtnList.at(k).press();
}

void bindManager::cButtonRelease(int cID, int cBtn, modifier mod)
{
  auto k = std::make_tuple(cID, cBtn, (int)mod);
  if (_cBtnList.count(k) > 0)
    _cBtnList.at(k).release();
}

void bindManager::mAxisDelta(int mAxis, modifier mod, double delta)
{
  auto k = std::make_pair(mAxis, (int)mod);
  if (_mAxisList.count(k) > 0)
    _mAxisList.at(k).delta(delta);
}

void bindManager::cAxisDelta(int cID, int cAxis, modifier mod, double delta)
{
  auto k = std::make_tuple(cID, cAxis, (int)mod);
  if (_cAxisList.count(k) > 0)
    _cAxisList.at(k).delta(delta);
}

void bindManager::clearBinds()
{
  _bindList.clear();
  _keyList.clear();
  _mBtnList.clear();
  _cBtnList.clear();
  _mAxisList.clear();
  _cAxisList.clear();
}

bool bindManager::bound(bindManager::bind &b)
{
  if (b._keyBind) {
    return true;
  }
  if (b._mBtnBind) {
    return true;
  }
  if (b._cBtnBind) {
    return true;
  }
  if (b._mAxisBind) {
    return true;
  }
  if (b._cAxisBind) {
    return true;
  }
  return false;
}

void bindManager::unbind(bindManager::bind &b)
{
  if (b._keyBind) {
    _keyList.erase(b._keyBind.value());
    b._keyBind.reset();
  }
  if (b._mBtnBind) {
    _mBtnList.erase(b._mBtnBind.value());
    b._mBtnBind.reset();
  }
  if (b._cBtnBind) {
    _cBtnList.erase(b._cBtnBind.value());
    b._cBtnBind.reset();
  }
  if (b._mAxisBind) {
    _mAxisList.erase(b._mAxisBind.value());
    b._mAxisBind.reset();
  }
  if (b._cAxisBind) {
    _cAxisList.erase(b._cAxisBind.value());
    b._cAxisBind.reset();
  }
}

} // namespace nebula
