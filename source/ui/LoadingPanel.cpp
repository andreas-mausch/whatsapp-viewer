#include <async++.h>

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "LoadingPanel.h"

namespace UI {

LoadingPanel::LoadingPanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("LoadingPanel"));
  XRCCTRL(*this, "loading", wxStaticText)->SetForegroundColour(*wxBLUE);
}

void LoadingPanel::setChild(wxWindow *child) {
  wxWindow *previousChild = this->FindWindow("child");
  if (previousChild != nullptr) {
    previousChild->Destroy();
  }

  wxXmlResource::Get()->AttachUnknownControl("child", child);
}

void LoadingPanel::setTask(async::task<void> &loading) {
  XRCCTRL(*this, "loading", wxStaticText)->SetForegroundColour(*wxYELLOW);
  this->loading = std::make_optional<async::task<void> *>(&loading);
  loading.then([this](async::task<void> task) {
    try {
      task.get();
      XRCCTRL(*this, "loading", wxStaticText)->SetForegroundColour(*wxGREEN);
    } catch (const async::task_canceled& exception) {
      XRCCTRL(*this, "loading", wxStaticText)->SetForegroundColour(*wxRED);
    }
    this->loading = std::nullopt;
  });
}

} // namespace UI
