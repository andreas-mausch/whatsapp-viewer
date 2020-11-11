#pragma once

#include <wx/wx.h>

namespace UI::ButtonWithSpinner {

class ButtonWithSpinner : public wxPanel {
public:
  ButtonWithSpinner(wxSize size, const std::string &label, wxWindow *parent);

  void setLoading(bool loading);
private:
  void onClick(wxCommandEvent &event);
};

} // namespace UI
