#include <wx/animate.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "ButtonWithSpinner.h"

namespace UI::ButtonWithSpinner {

ButtonWithSpinner::ButtonWithSpinner(wxSize size, const std::string &label, wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("ButtonWithSpinner"));
  Bind(wxEVT_BUTTON, &ButtonWithSpinner::onClick, this, XRCID("button"));
  SetMinSize(size);
  XRCCTRL(*this, "button", wxButton)->SetLabel(label);
  setLoading(false);
}

void ButtonWithSpinner::setLoading(bool loading) {
  if (loading) {
    XRCCTRL(*this, "button", wxButton)->Disable();
    XRCCTRL(*this, "spinner", wxAnimationCtrl)->Show();
    XRCCTRL(*this, "spinner", wxAnimationCtrl)->Play();
  } else {
    XRCCTRL(*this, "button", wxButton)->Enable();
    XRCCTRL(*this, "spinner", wxAnimationCtrl)->Stop();
    XRCCTRL(*this, "spinner", wxAnimationCtrl)->Hide();
  }
}

void ButtonWithSpinner::onClick(wxCommandEvent &_) {
  wxCommandEvent event(wxEVT_BUTTON, GetId());
  event.SetEventObject(this);
  HandleWindowEvent(event);
}

} // namespace UI
