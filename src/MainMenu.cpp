#include "MainMenu.hpp"
#include "AudioMenu.hpp"
#include "ServerTestBranch.hpp"
#include "HostGameBranch.hpp"

MainMenu::~MainMenu()
{
    sound_track.stop();
}

void MainMenu::start()
{
    sound_track.setLoop(true);
    sound_track.play();
    rn::Table button_table{ 5, 10, Button::getSize()*1.2f };
    size_t i = 0;
    for (auto &button : Button::getButtons())
    {
        button->setPosition(button_table.getCellGlobalPos(1, (3 + i++) % 10));
    }
    logo_sprite.setPosition(button_table.getCellGlobalPos(2, 1));
    background.start();
    for (auto &button : Button::getButtons())
    {
        button->start();
    }
}

void MainMenu::update()
{
    for (auto &button : Button::getButtons())
    {
        button->update();
    }
    background.update();
    window.clear();
    window.draw(background);
    for (auto &button : Button::getButtons())
    {
        window.draw(*button);
    }
    window.draw(logo_sprite);
    window.display();
}

void MainMenu::onEvent(sf::Event &event)
{
    for (auto &button : Button::getButtons())
    {
        button->onEvent(event);
    }
    background.onEvent(event);
    if (event.type == sf::Event::Closed)
    {
        window.close();
    }
    if (play_button.isClicked(sf::Mouse::Left))
    {
        next_branch<AudioMenu>(window);
    }
    else if (host_button.isClicked(sf::Mouse::Left))
    {
        next_branch<HostGameBranch>(window);
    }
    else if (connect_button.isClicked(sf::Mouse::Left))
    {
        next_branch<ServerTestBranch>(window);
    }
    else if (exit_button.isClicked(sf::Mouse::Left))
    {
        window.close();
    }
}
