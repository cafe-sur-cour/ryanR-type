/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Chat State
*/

#include "ChatState.hpp"
#include <memory>
#include <string>
#include <iostream>
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"

namespace gsm {

ChatState::ChatState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void ChatState::enter() {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig mainConfig;
    mainConfig.direction = ui::LayoutDirection::Vertical;
    mainConfig.alignment = ui::LayoutAlignment::Center;
    mainConfig.spacing = 35.0f;
    mainConfig.padding = math::Vector2f(50.0f, 45.0f);
    mainConfig.anchorX = ui::AnchorX::Center;
    mainConfig.anchorY = ui::AnchorY::Center;
    mainConfig.offset = math::Vector2f(0.0f, -30.0f);
    mainConfig.background.enabled = true;
    mainConfig.background.fillColor = colors::PANEL_BACKGROUND;
    mainConfig.background.outlineColor = colors::PANEL_BORDER;
    mainConfig.background.cornerRadius = 20.0f;

    _mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    _mainLayout->setSize(math::Vector2f(1200.f, 720.f));
    _mainLayout->setBackgroundEnabled(true);
    _mainLayout->setBackgroundFillColor(colors::PANEL_BACKGROUND);
    _mainLayout->setBackgroundOutlineColor(colors::PANEL_BORDER);
    _mainLayout->setBackgroundCornerRadius(20.0f);

    ui::LayoutConfig titleConfig;
    titleConfig.direction = ui::LayoutDirection::Vertical;
    titleConfig.alignment = ui::LayoutAlignment::Center;
    titleConfig.spacing = 0.0f;
    titleConfig.padding = math::Vector2f(20.0f, 20.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(1000.f, 70.f));

    auto titleText = std::make_shared<ui::Text>(_resourceManager);
    titleText->setText("Chat Room");
    titleText->setTextColor(colors::UI_ACCENT);
    titleText->setFontSize(32);
    titleText->setOutlineColor(colors::UI_OUTLINE);
    titleText->setOutlineThickness(2.5f);

    titleLayout->addElement(titleText);

    ui::LayoutConfig messagesConfig;
    messagesConfig.direction = ui::LayoutDirection::Vertical;
    messagesConfig.alignment = ui::LayoutAlignment::Center;
    messagesConfig.spacing = 18.0f;
    messagesConfig.padding = math::Vector2f(35.0f, 35.0f);
    messagesConfig.background.enabled = true;
    messagesConfig.background.fillColor = colors::UI_BACKGROUND;
    messagesConfig.background.outlineColor = colors::UI_OUTLINE;
    messagesConfig.background.cornerRadius = 12.0f;

    _messagesContainer = std::make_shared<ui::UILayout>(_resourceManager, messagesConfig);
    _messagesContainer->setSize(math::Vector2f(1100.f, 420.f));
    _messagesContainer->setBackgroundEnabled(true);
    _messagesContainer->setBackgroundFillColor(colors::UI_BACKGROUND);
    _messagesContainer->setBackgroundOutlineColor(colors::UI_OUTLINE);
    _messagesContainer->setBackgroundCornerRadius(12.0f);

    auto placeholderText = std::make_shared<ui::Text>(_resourceManager);
    placeholderText->setText("No messages yet. Start the conversation!");
    placeholderText->setTextColor(gfx::color_t{150, 150, 150, 255});
    placeholderText->setFontSize(16);
    _messagesContainer->addElement(placeholderText);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText("Back");
    _backButton->setSize(math::Vector2f{150, 55});
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnRelease([this]() { onBackButtonClicked(); });

    ui::LayoutConfig backButtonConfig;
    backButtonConfig.anchorX = ui::AnchorX::Center;
    backButtonConfig.anchorY = ui::AnchorY::Bottom;
    backButtonConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto backButtonLayout = std::make_shared<ui::UILayout>(_resourceManager, backButtonConfig);
    backButtonLayout->setSize(math::Vector2f(150.f, 55.f));
    backButtonLayout->addElement(_backButton);

    _messageInput = std::make_shared<ui::TextInput>(_resourceManager);
    _messageInput->setPlaceholder("Type your message...");
    _messageInput->setSize(math::Vector2f{800, 55});
    _messageInput->setMaxLength(50);
    _messageInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_messageInput);
    });

    _sendButton = std::make_shared<ui::Button>(_resourceManager);
    _sendButton->setText("Send");
    _sendButton->setSize(math::Vector2f{170, 55});
    _sendButton->setNormalColor(colors::BUTTON_PRIMARY);
    _sendButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _sendButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _sendButton->setOnRelease([this]() {
        auto network = _resourceManager->get<ClientNetwork>();
        if (network) {
            network->sendMessageToServer(_messageInput->getText());
            _messageInput->setText("");
        }
     });

    ui::LayoutConfig controlsConfig;
    controlsConfig.direction = ui::LayoutDirection::Horizontal;
    controlsConfig.alignment = ui::LayoutAlignment::Center;
    controlsConfig.spacing = 25.0f;
    controlsConfig.padding = math::Vector2f(0.0f, 10.0f);

    auto controlsLayout = std::make_shared<ui::UILayout>(_resourceManager, controlsConfig);
    controlsLayout->setSize(math::Vector2f(1000.f, 70.f));

    ui::LayoutConfig messagesWrapperConfig;
    messagesWrapperConfig.direction = ui::LayoutDirection::Vertical;
    messagesWrapperConfig.alignment = ui::LayoutAlignment::Center;
    messagesWrapperConfig.spacing = 0.0f;
    messagesWrapperConfig.padding = math::Vector2f(0.0f, 0.0f);
    messagesWrapperConfig.anchorX = ui::AnchorX::Center;
    messagesWrapperConfig.anchorY = ui::AnchorY::Center;

    auto messagesWrapper = std::make_shared<ui::UILayout>(_resourceManager,
        messagesWrapperConfig);
    messagesWrapper->setSize(math::Vector2f(1100.f, 450.f));
    messagesWrapper->addElement(_messagesContainer);

    _mainLayout->addElement(titleLayout);
    _mainLayout->addElement(messagesWrapper);

    controlsLayout->addElement(_messageInput);
    controlsLayout->addElement(_sendButton);

    _mainLayout->addElement(controlsLayout);

    _uiManager->addElement(_background);
    _uiManager->addElement(_mainLayout);
    _uiManager->addElement(backButtonLayout);
}

void ChatState::update(float deltaTime) {
    auto network = _resourceManager->get<ClientNetwork>();
    if (network) {
        const auto& messages = network->getLastMessages();
        if (!messages.empty()) {
            _messagesContainer->clearElements();

            for (size_t i = 0; i < messages.size(); ++i) {
                const auto& [username, message] = messages[i];
                auto messageText = std::make_shared<ui::Text>(_resourceManager);
                std::string displayText = username + ": " + message;
                messageText->setText(displayText);
                messageText->setTextColor(gfx::color_t{255, 255, 255, 255});
                messageText->setFontSize(22);
                messageText->setOutlineColor(gfx::color_t{0, 0, 0, 180});
                messageText->setOutlineThickness(1.0f);
                _messagesContainer->addElement(messageText);

                if (i < messages.size() - 1) {
                    auto spacer = std::make_shared<ui::Text>(_resourceManager);
                    spacer->setText("");
                    spacer->setFontSize(10);
                    _messagesContainer->addElement(spacer);
                }
            }
        }
    }

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _uiManager->handleKeyboardInput(eventResult);

    if (eventResult == gfx::EventType::TEXT_INPUT) {
        std::string textInput = _resourceManager->get<gfx::IEvent>()->getLastTextInput();
        if (!textInput.empty()) {
            _uiManager->handleTextInput(textInput);
        }
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    _uiManager->handleMouseInput(mousePos, mousePressed);

    bool isHoveringUI = _uiManager->isMouseHoveringAnyElement(mousePos);

    auto navManager = _uiManager->getNavigationManager();
    if (mousePressed && !isHoveringUI && navManager) {
        navManager->clearFocus();
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void ChatState::exit() {
    _uiManager.reset();
    _mouseHandler.reset();
}

void ChatState::renderUI() {
    _uiManager->render();
}

void ChatState::onBackButtonClicked() {
    if (auto gsm = _gsm.lock()) {
        gsm->requestStatePop();
    }
}

void ChatState::onSendMessage() {
}

}  // namespace gsm
