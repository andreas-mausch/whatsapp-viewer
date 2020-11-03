#pragma once

#include <memory>
#include <optional>

#include <async++.h>
#include <wx/wx.h>

namespace async {
  template<class Result> class task;
}

namespace UI {

class LoadingPanel : public wxPanel {
public:
  LoadingPanel(wxWindow *parent);

  void setChild(wxWindow *child);
  void setTask(async::task<void> loading, std::unique_ptr<async::cancellation_token> cancellationToken);

  void cancel();

private:
  std::optional<async::task<void>> loading;
  std::unique_ptr<async::cancellation_token> cancellationToken;

  void finally(wxCommandEvent &event);
};

} // namespace UI
