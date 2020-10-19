#pragma once

#include <cxxopts.hpp>
#include <wx/wx.h>

namespace UI {

class MainFrame;

class App : public wxApp {
public:
  App();
  virtual bool OnInit();
  virtual int OnRun();
private:
  MainFrame *frame;
  cxxopts::Options options;
};

} // namespace UI
