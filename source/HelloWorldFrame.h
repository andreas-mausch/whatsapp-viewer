#pragma once

class HelloWorldFrame: public wxFrame
{
public:
    HelloWorldFrame(wxWindow* parent);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
