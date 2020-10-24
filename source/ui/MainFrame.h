#pragma once

#include <optional>

class wxCommandEvent;

namespace UI {

class LoadingPanel;

class MainFrame : public wxFrame {
public:
  MainFrame(wxWindow *parent);
  void openDatabase(const std::string &filename);

private:
  void setMainPanel(wxPanel *panel);
  LoadingPanel *getLoadingPanel();

  void OnOpenDatabase(wxCommandEvent &event);
  void OnDropFiles(wxDropFilesEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
};

} // namespace UI
