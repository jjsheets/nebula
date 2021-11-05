// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_BIND_H
#define NEBULA_BIND_H

#include <functional>
#include <optional>
#include <map>
#include <tuple>
#include <string>
#include <memory>

namespace nebula {

class bindManager {
public:
  class bind {
  private:
    std::function<void()> _pressHandler;
    std::function<void()> _releaseHandler;
    std::function<void(double)> _deltaHandler;
    const std::string _category;
    const std::string _name;
    std::optional<std::pair<int, int>> _keyBind;
    std::optional<std::pair<int, int>> _mBtnBind;
    std::optional<std::tuple<int, int, int>> _cBtnBind;
    std::optional<std::pair<int, int>> _mAxisBind;
    std::optional<std::tuple<int, int, int>> _cAxisBind;

    friend class bindManager;

    void press();
    void release();
    void delta(double d);

  public:
    bind()
        : _pressHandler(nullptr), _releaseHandler(nullptr),
          _deltaHandler(nullptr), _category(""), _name("")
    {
    }
    bind(std::function<void()> pressHandler,
        std::function<void()> releaseHandler,
        std::function<void(double)> deltaHandler,
        const std::string &category,
        const std::string &name)
        : _pressHandler(pressHandler), _releaseHandler(releaseHandler),
          _deltaHandler(deltaHandler), _category(category), _name(name)
    {
    }
    std::string combinedName();
  };
  class modifier {
  public:
    operator int()
    {
      return 0;
    }
  };

private:
  std::map<std::string, bindManager::bind> _bindList;
  std::map<std::pair<int, int>, bindManager::bind &> _keyList;
  std::map<std::pair<int, int>, bindManager::bind &> _mBtnList;
  std::map<std::tuple<int, int, int>, bindManager::bind &> _cBtnList;
  std::map<std::pair<int, int>, bindManager::bind &> _mAxisList;
  std::map<std::tuple<int, int, int>, bindManager::bind &> _cAxisList;

public:
  bindManager::bind &create(std::function<void()> pressHandler,
      std::function<void()> releaseHandler,
      std::function<void(double)> deltaHandler,
      const std::string &category,
      const std::string &name);
  void bindKey(bindManager::bind &b, int key, modifier mod);
  void bindMBtn(bindManager::bind &b, int mBtn, modifier mod);
  void bindCBtn(bindManager::bind &b, int cID, int cBtn, modifier mod);
  void bindMAxis(bindManager::bind &b, int mAxis, modifier mod);
  void bindCAxis(bindManager::bind &b, int cID, int cAxis, modifier mod);
  bool keyBindIs(int key, modifier mod, bindManager::bind &b);
  bool mBtnBindIs(int mBtn, modifier mod, bindManager::bind &b);
  bool cBtnBindIs(int cID, int cBtn, modifier mod, bindManager::bind &b);
  bool mAxisBindIs(int mAxis, modifier mod, bindManager::bind &b);
  bool cAxisBindIs(int cID, int cAxis, modifier mod, bindManager::bind &b);
  bool bindExists(const std::string &category, const std::string &name);
  bool keyBindExists(int key, modifier mod);
  bool mBtnBindExists(int mBtn, modifier mod);
  bool cBtnBindExists(int cID, int cBtn, modifier mod);
  bool mAxisBindExists(int mAxis, modifier mod);
  bool cAxisBindExists(int cID, int cAxis, modifier mod);
  void keyboardPress(int key, modifier mod);
  void keyboardRelease(int key, modifier mod);
  void mButtonPress(int mBtn, modifier mod);
  void mButtonRelease(int mBtn, modifier mod);
  void cButtonPress(int cID, int cBtn, modifier mod);
  void cButtonRelease(int cID, int cBtn, modifier mod);
  void mAxisDelta(int mAxis, modifier mod, double delta);
  void cAxisDelta(int cID, int cAxis, modifier mod, double delta);
  void clearBinds();
  bool bound(bindManager::bind &b);
  void unbind(bindManager::bind &b);
};

} // namespace nebula

#endif // NEBULA_BIND_H
