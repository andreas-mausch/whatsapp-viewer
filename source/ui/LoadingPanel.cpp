#include <async++.h>

#include <wx/animate.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "LoadingPanel.h"

namespace UI {

wxDEFINE_EVENT(LOADING_PANEL_LOADED, wxCommandEvent);
wxDEFINE_EVENT(LOADING_PANEL_ERROR, wxCommandEvent);
wxDEFINE_EVENT(LOADING_PANEL_FINALLY, wxCommandEvent);

LoadingPanel::LoadingPanel(wxWindow *parent) : loading(false) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("LoadingPanel"));
  Bind(LOADING_PANEL_FINALLY, &LoadingPanel::finally, this);
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Hide();
}

void LoadingPanel::setChild(wxWindow *child) {
  wxWindow *previousChild = this->FindWindow("child");
  if (previousChild != nullptr) {
    previousChild->Destroy();
  }

  wxXmlResource::Get()->AttachUnknownControl("child", child);
}

void LoadingPanel::setTask(async::task<void> &loading) {
  XRCCTRL(*this, "child", wxWindow)->Hide();
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Show();
  GetSizer()->Layout();
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Play();
  this->loading = true;
  loading.then([this](async::task<void> task) {
    try {
      task.get();
      wxPostEvent(this, wxCommandEvent(LOADING_PANEL_LOADED));
    } catch (const async::task_canceled& exception) {
      wxPostEvent(this, wxCommandEvent(LOADING_PANEL_ERROR));
    }

    wxPostEvent(this, wxCommandEvent(LOADING_PANEL_FINALLY));
  });
}

void LoadingPanel::finally(wxCommandEvent &event) {
  XRCCTRL(*this, "child", wxWindow)->Show();
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Hide();
  GetSizer()->Layout();
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Stop();
  this->loading = false;
}

} // namespace UI
