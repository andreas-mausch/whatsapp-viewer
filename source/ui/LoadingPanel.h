#pragma once

#include <optional>

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
  std::optional<async::task<void> *> loading;
};

} // namespace UI
