#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "HelloWorldFrame.h"

wxBEGIN_EVENT_TABLE(HelloWorldFrame, wxFrame)
    EVT_MENU(XRCID("ID_Hello"),   HelloWorldFrame::OnHello)
    EVT_MENU(wxID_EXIT,  HelloWorldFrame::OnExit)
    EVT_MENU(wxID_ABOUT, HelloWorldFrame::OnAbout)
wxEND_EVENT_TABLE()

HelloWorldFrame::HelloWorldFrame(wxWindow* parent)
{
    wxXmlResource::Get()->LoadFrame(this, parent, wxT("HelloWorldFrame"));
    SetStatusText("Welcome to wxWidgets!");
}

void HelloWorldFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void HelloWorldFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
                 "About Hello World",
                 wxOK | wxICON_INFORMATION);
}

void HelloWorldFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
