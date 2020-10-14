#pragma once

class MessagePanel: public wxPanel
{
public:
    MessagePanel(wxWindow* parent, const std::string &message);

private:
    wxDECLARE_EVENT_TABLE();
};
