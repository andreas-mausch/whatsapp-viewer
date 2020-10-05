#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "MainFrame.h"
#include "FileOpenDialog.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(XRCID("ID_OpenDatabase"),   MainFrame::OnOpenDatabase)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent)
{
    wxXmlResource::Get()->LoadFrame(this, parent, wxT("MainFrame"));
    SetStatusText("Welcome to wxWidgets!");
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
                 "About Hello World",
                 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenDatabase(wxCommandEvent& event)
{
    auto filename = fileOpenDialog(this);

    if (filename)
    {
        wxLogMessage((*filename).c_str());
    }
    else
    {
        wxLogMessage("No file selected");
    }
}
