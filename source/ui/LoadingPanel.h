#pragma once

#include <optional>

#include <wx/wx.h>

namespace async {
  template<class Result> class task;
}

namespace UI {

class LoadingPanel : public wxPanel {
public:
  LoadingPanel(wxWindow *parent);

  void setChild(wxWindow *child);
  void setTask(async::task<void> &loading);

private:
  bool loading;

  void finally(wxCommandEvent &event);
};

} // namespace UI
