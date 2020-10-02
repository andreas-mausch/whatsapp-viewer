#pragma once

class MainFrame: public wxFrame
{
public:
    MainFrame(wxWindow* parent);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
