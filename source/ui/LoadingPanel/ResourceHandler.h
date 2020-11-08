#include <wx/xrc/xmlres.h>

namespace UI::LoadingPanel {

class ResourceHandler : public wxXmlResourceHandler {
public:
  ResourceHandler();
  virtual wxObject *DoCreateResource();
  virtual bool CanHandle(wxXmlNode *node);
  wxDECLARE_DYNAMIC_CLASS(ResourceHandler);
};

}; // namespace UI
