#pragma once

#include <optional>
#include <string>

class wxWindow;

std::optional<std::string> fileOpenDialog(wxWindow *parent);
