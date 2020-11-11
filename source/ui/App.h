#pragma once

#include <wx/wx.h>

#include <cxxopts.hpp>

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
