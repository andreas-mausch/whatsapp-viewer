#include <async++.h>

#include <wx/animate.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "LoadingPanel.h"

namespace UI::LoadingPanel {

wxDEFINE_EVENT(LOADING_PANEL_LOADED, wxCommandEvent);
wxDEFINE_EVENT(LOADING_PANEL_ERROR, wxCommandEvent);
wxDEFINE_EVENT(LOADING_PANEL_FINALLY, wxCommandEvent);

LoadingPanel::LoadingPanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("LoadingPanel"));
  Bind(LOADING_PANEL_FINALLY, &LoadingPanel::finally, this);
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Hide();
}

void LoadingPanel::setChild(wxWindow *child) {
  wxXmlResource::Get()->AttachUnknownControl("child", child, this);
}

void LoadingPanel::setTask(async::task<void> loading, std::unique_ptr<async::cancellation_token> cancellationToken) {
  XRCCTRL(*this, "child", wxWindow)->Hide();
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Show();
  GetSizer()->Layout();
  XRCCTRL(*this, "spinner", wxAnimationCtrl)->Play();
  this->cancellationToken = std::move(cancellationToken);
  this->loading = loading.then([this](async::task<void> task) {
    try {
      task.get();
      async::interruption_point(*this->cancellationToken);
      wxPostEvent(this, wxCommandEvent(LOADING_PANEL_LOADED));
    } catch (const std::runtime_error& exception) {
      async::interruption_point(*this->cancellationToken);
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
  this->loading = std::nullopt;
  this->cancellationToken.reset();
}

void LoadingPanel::cancel() {
  if (loading && cancellationToken) {
    cancellationToken->cancel();
    loading->wait();

    loading = std::nullopt;
    cancellationToken.reset();
  }
}

} // namespace UI
