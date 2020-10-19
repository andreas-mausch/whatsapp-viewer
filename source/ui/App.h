#pragma once

#include <cxxopts.hpp>
#include <wx/wx.h>

namespace UI {

class App : public wxApp {
public:
  App();
  virtual bool OnInit();
  virtual int OnRun();
private:
  cxxopts::Options options;
};

} // namespace UI
