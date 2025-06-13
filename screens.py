# The screens dictionary contains the objects of the models and controllers
# of the screens of the application.
from Model.login_screen import LoginScreenModel
from Controller.login_screen import LoginScreenController

from Model.main_screen import MainScreenModel
from Controller.main_screen import MainScreenController

from Model.chat_screen import ChatScreenModel
from Controller.chat_screen import ChatScreenController

screens = {
    "login screen": {
        "model": LoginScreenModel,
        "controller": LoginScreenController,
    },
    "main screen": {
        "model": MainScreenModel,
        "controller": MainScreenController,
    },
    "chat screen": {
        "model": ChatScreenModel,
        "controller": ChatScreenController,
    },
}
