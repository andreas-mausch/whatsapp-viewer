#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <wx/wx.h>

class wxListEvent;

namespace WhatsApp {
class Chat;
class Database;
} // namespace WhatsApp

namespace UI {

namespace LoadingPanel {
class LoadingPanel;
}

class DatabasePanel : public wxPanel {
public:
  DatabasePanel(wxWindow *parent, std::unique_ptr<WhatsApp::Database> database);
  ~DatabasePanel();
  void loadChats();

private:
  std::unique_ptr<WhatsApp::Database> database;
  std::vector<std::unique_ptr<WhatsApp::Chat>> chats;
  std::optional<WhatsApp::Chat *> selectedChat;

  void updateChats(wxCommandEvent &event);
  void updateMessages(wxCommandEvent &event);
  void openChat(WhatsApp::Chat &chat);

  void OnDisplayChat(wxListEvent &event);

  LoadingPanel::LoadingPanel &getDatabaseLoadingPanel();
  LoadingPanel::LoadingPanel &getMessagesLoadingPanel();
};

} // namespace UI
