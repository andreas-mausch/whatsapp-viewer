#pragma once

#include <wx/wx.h>
#include <vector>

namespace UI {

template <class T, class Panel> class PanelList : public wxScrolledWindow {
public:
  PanelList(wxWindow *parent);
  virtual ~PanelList();

  void clear();
  void setList(const std::vector<T> &list);
};

} // namespace UI
