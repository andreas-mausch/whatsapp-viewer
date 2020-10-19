#pragma once

#include <optional>

class wxCommandEvent;

namespace UI {

class MainFrame : public wxFrame {
public:
  MainFrame(wxWindow *parent);
  void openDatabase(const std::string &filename);

private:
  std::optional<wxPanel *> mainPanel;

  void setMainPanel(wxPanel *panel);

  void OnOpenDatabase(wxCommandEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
};

} // namespace UI
