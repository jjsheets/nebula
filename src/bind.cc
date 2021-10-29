// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "bind.h"

namespace nebula {

void bind::bindKey(int key)
{
  _key = key;
  _mBtn.reset();
  _mAxis.reset();
  _jid.reset();
  _jBtn.reset();
  _jAxis.reset();
}

void bind::bindMButton(int mBtn)
{
  _key.reset();
  _mBtn = mBtn;
  _mAxis.reset();
  _jid.reset();
  _jBtn.reset();
  _jAxis.reset();
}

void bind::bindMAxis(int mAxis)
{
  _key.reset();
  _mBtn.reset();
  _mAxis = mAxis;
  _jid.reset();
  _jBtn.reset();
  _jAxis.reset();
}

void bind::bindJButton(int jid, int jBtn)
{
  _key.reset();
  _mBtn.reset();
  _mAxis.reset();
  _jid  = jid;
  _jBtn = jBtn;
  _jAxis.reset();
}

void bind::bindJAxis(int jid, int jAxis)
{
  _key.reset();
  _mBtn.reset();
  _mAxis.reset();
  _jid = jid;
  _jBtn.reset();
  _jAxis = jAxis;
}

void bind::unbind()
{
  _key.reset();
  _mBtn.reset();
  _mAxis.reset();
  _jid.reset();
  _jBtn.reset();
  _jAxis.reset();
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

} // namespace nebula
