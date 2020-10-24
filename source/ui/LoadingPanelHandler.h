#include <wx/xrc/xmlres.h>

namespace UI {

class LoadingPanelHandler : public wxXmlResourceHandler {
public:
  LoadingPanelHandler();
  virtual wxObject *DoCreateResource();
  virtual bool CanHandle(wxXmlNode *node);
  wxDECLARE_DYNAMIC_CLASS(LoadingPanelHandler);
};

}; // namespace UI
