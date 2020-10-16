#pragma once

#include <optional>
#include <string>

class wxWindow;

namespace UI {

std::optional<std::string> fileOpenDialog(wxWindow *parent);

}
