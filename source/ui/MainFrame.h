#pragma once

#include <memory>

namespace WhatsApp { class Database; }

class MainFrame: public wxFrame
{
public:
    MainFrame(wxWindow* parent);
private:
    std::unique_ptr<WhatsApp::Database> database;

    void OnOpenDatabase(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
