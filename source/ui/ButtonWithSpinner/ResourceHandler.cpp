#include "ResourceHandler.h"

#include <wx/xml/xml.h>

#include "ButtonWithSpinner.h"

namespace UI::ButtonWithSpinner {

wxIMPLEMENT_DYNAMIC_CLASS(UI::ButtonWithSpinner::ResourceHandler,
                          wxXmlResourceHandler);

ResourceHandler::ResourceHandler() {
  XRC_ADD_STYLE(wxTAB_TRAVERSAL);
  XRC_ADD_STYLE(wxALL);
  XRC_ADD_STYLE(wxEXPAND);
  XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
  AddWindowStyles();
}

wxObject *ResourceHandler::DoCreateResource() {
  if (GetInstance() != nullptr) {
    throw std::runtime_error(
        "ButtonWithSpinner::ResourceHandler: Instance already created");
  }

  ButtonWithSpinner *control = new ButtonWithSpinner(
      GetSize(), GetParamValue("label").ToStdString(), m_parentAsWindow);
  control->SetName(GetName());
  SetupWindow(control);

  return control;
}

bool ResourceHandler::CanHandle(wxXmlNode *node) {
  return IsOfClass(node, _("ButtonWithSpinner"));
}

}; // namespace UI::ButtonWithSpinner
