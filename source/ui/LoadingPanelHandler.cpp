#include <wx/xml/xml.h>

#include "LoadingPanel.h"
#include "LoadingPanelHandler.h"

namespace UI {

wxIMPLEMENT_DYNAMIC_CLASS(LoadingPanelHandler, wxXmlResourceHandler);

LoadingPanelHandler::LoadingPanelHandler() {
  XRC_ADD_STYLE(wxTAB_TRAVERSAL);
  XRC_ADD_STYLE(wxALL);
  XRC_ADD_STYLE(wxEXPAND);
  XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
  AddWindowStyles();
}

wxObject *LoadingPanelHandler::DoCreateResource() {
  if (GetInstance() != nullptr) {
    throw std::runtime_error("LoadingPanelHandler: Instance already created");
  }
  LoadingPanel *control = new LoadingPanel(m_parentAsWindow);
  control->SetName(GetName());
  SetupWindow(control);

  wxXmlNode *childNode = GetNode()->GetChildren();
  if (childNode != nullptr) {
    if (childNode->GetNext() != nullptr) {
      throw std::runtime_error(
          "LoadingPanelHandler: Only one child is allowed");
    }

    if (IsObjectNode(childNode)) {
      wxWindow *child =
          static_cast<wxWindow *>(CreateResFromNode(childNode, control));
      control->setChild(child);
    }
  }

  return control;
}

bool LoadingPanelHandler::CanHandle(wxXmlNode *node) {
  return IsOfClass(node, _("LoadingPanel"));
}

}; // namespace UI
