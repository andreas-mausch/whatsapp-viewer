#pragma once

#include <optional>

class wxCommandEvent;

namespace UI {

class MainFrame : public wxFrame {
public:
  MainFrame(wxWindow *parent);
  void openDatabase(const std::string &filename);

private:
  std::optional<wxPanel *> panel;
  void setPanel(wxPanel *panel);

  void onExit(wxCommandEvent &event);
  void onAbout(wxCommandEvent &event);
  void onOpenDatabase(wxCommandEvent &event);
  void onDropFiles(wxDropFilesEvent &event);
};

} // namespace UI
