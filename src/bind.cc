// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "bind.h"

namespace nebula {

void bind::bindKey(int key, modifier mods)
{
  unbind();
  auto k              = std::make_tuple(key, mods.bits);
  const auto &keyBind = _keyBinds.find(k);
  if (keyBind == _keyBinds.end())
    keyBind->second->unbind();
  _keyBinds[k] = shared_from_this();
  _key         = key;
  _modifiers   = mods;
}

void bind::bindMButton(int mBtn, modifier mods)
{
  unbind();
  auto k               = std::make_tuple(mBtn, mods.bits);
  const auto &mBtnBind = _mBtnBinds.find(k);
  if (mBtnBind == _mBtnBinds.end())
    mBtnBind->second->unbind();
  _mBtnBinds[k] = shared_from_this();
  _mBtn         = mBtn;
  _modifiers    = mods;
  _bound        = true;
}

void bind::bindMAxis(int mAxis, modifier mods)
{
  unbind();
  auto k                = std::make_tuple(mAxis, mods.bits);
  const auto &mAxisBind = _mAxisBinds.find(k);
  if (mAxisBind == _mAxisBinds.end())
    mAxisBind->second->unbind();
  _mAxisBinds[k] = shared_from_this();
  _mAxis         = mAxis;
  _modifiers     = mods;
  _bound         = true;
}

void bind::bindJButton(int jid, int jBtn, modifier mods)
{
  unbind();
  auto k               = std::make_tuple(jid, jBtn, mods.bits);
  const auto &jBtnBind = _jBtnBinds.find(k);
  if (jBtnBind == _jBtnBinds.end())
    jBtnBind->second->unbind();
  _jBtnBinds[k] = shared_from_this();
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
  if (jAxisBind == _jAxisBinds.end())
    jAxisBind->second->unbind();
  _jAxisBinds[k] = shared_from_this();
  _jid           = jid;
  _jAxis         = jAxis;
  _modifiers     = mods;
  _bound         = true;
}

void bind::unbind()
{
  _bound = false;
  if (!_key) {
    _keyBinds.erase(std::make_tuple(_key.value(), _modifiers.bits));
    _key.reset();
    return;
  }
  if (!_mBtn) {
    _mBtnBinds.erase(std::make_tuple(_mBtn.value(), _modifiers.bits));
    _mBtn.reset();
    return;
  }
  if (!_mAxis) {
    _mAxisBinds.erase(std::make_tuple(_mAxis.value(), _modifiers.bits));
    _mAxis.reset();
    return;
  }
  if (!_jBtn) {
    _jBtnBinds.erase(
        std::make_tuple(_jid.value(), _jBtn.value(), _modifiers.bits));
    _jid.reset();
    _jBtn.reset();
    return;
  }
  if (!_jAxis) {
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
      keyBind->second->press();
    else
      keyBind->second->release();
  }
}

void bind::mouseButtonEvent(int mBtn, action event, modifier mods)
{
  const auto &mBtnBind = _mBtnBinds.find(std::make_tuple(mBtn, mods.bits));
  if (mBtnBind != _mBtnBinds.end()) {
    if (event == action::press)
      mBtnBind->second->press();
    else
      mBtnBind->second->release();
  }
}

void bind::mouseAxisEvent(int mAxis, double delta, modifier mods)
{
  const auto &mAxisBind = _mAxisBinds.find(std::make_tuple(mAxis, mods.bits));
  if (mAxisBind != _mAxisBinds.end()) {
    mAxisBind->second->delta(delta);
  }
}

void bind::joystickButtonEvent(int jid, int jBtn, action event, modifier mods)
{
  const auto &jBtnBind = _jBtnBinds.find(std::make_tuple(jid, jBtn, mods.bits));
  if (jBtnBind != _jBtnBinds.end()) {
    if (event == action::press)
      jBtnBind->second->press();
    else
      jBtnBind->second->release();
  }
}

void bind::joystickAxisEvent(int jid, int jAxis, double delta, modifier mods)
{
  const auto &jAxisBind
      = _jAxisBinds.find(std::make_tuple(jid, jAxis, mods.bits));
  if (jAxisBind != _jAxisBinds.end()) {
    jAxisBind->second->delta(delta);
  }
}

std::map<std::tuple<int, int>, std::shared_ptr<bind>> bind::_keyBinds;
std::map<std::tuple<int, int>, std::shared_ptr<bind>> bind::_mBtnBinds;
std::map<std::tuple<int, int>, std::shared_ptr<bind>> bind::_mAxisBinds;
std::map<std::tuple<int, int, int>, std::shared_ptr<bind>> bind::_jBtnBinds;
std::map<std::tuple<int, int, int>, std::shared_ptr<bind>> bind::_jAxisBinds;

} // namespace nebula
