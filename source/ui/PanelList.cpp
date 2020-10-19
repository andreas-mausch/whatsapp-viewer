#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "PanelList.h"
#include "MessagePanel.h"

namespace UI {

template <class T, class Panel>
PanelList<T, Panel>::PanelList(wxWindow *parent) {
  wxXmlResource::Get()->LoadObject(this, parent, _("PanelList"), "wxScrolledWindow");
}

template <class T, class Panel>
PanelList<T, Panel>::~PanelList() {
  clear();
}

template <class T, class Panel>
void PanelList<T, Panel>::clear() {
  GetSizer()->Clear(true);
}

template <class T, class Panel>
void PanelList<T, Panel>::setList(const std::vector<T> &list) {
  for (auto &item : list) {
    GetSizer()->Add(new Panel(this, item), wxSizerFlags().Expand().Border(wxALL, 0));
  }

  GetSizer()->Layout();
  SetVirtualSize(GetBestVirtualSize());
  FitInside();
}

} // namespace UI
