#pragma once

#include <memory>
#include <optional>
#include <vector>

class wxCommandEvent;
class wxListEvent;

namespace WhatsApp {
class Chat;
class Database;
} // namespace WhatsApp

namespace UI {

class MainFrame : public wxFrame {
public:
  MainFrame(wxWindow *parent);

private:
  std::unique_ptr<WhatsApp::Database> database;
  std::vector<WhatsApp::Chat> chats;
  std::optional<WhatsApp::Chat *> selectedChat;

  void updateChats();
  void openChat(WhatsApp::Chat &chat);

  void OnOpenDatabase(wxCommandEvent &event);
  void OnDisplayChat(wxListEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  wxDECLARE_EVENT_TABLE();
};

} // namespace UI
