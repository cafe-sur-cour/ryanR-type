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
#include "../../../../../common/InputMapping/IInputProvider.hpp"

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
    _uiManager->setNavigationEnabled(true);
    _uiManager->setOnBack([this]() { onBackButtonClicked(); });

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig titleConfig;
    titleConfig.direction = ui::LayoutDirection::Vertical;
    titleConfig.alignment = ui::LayoutAlignment::Center;
    titleConfig.spacing = 0.0f;
    titleConfig.padding = math::Vector2f(0.0f, 0.0f);
    titleConfig.anchorX = ui::AnchorX::Left;
    titleConfig.anchorY = ui::AnchorY::Top;
    titleConfig.offset = math::Vector2f(735.0f, 30.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(200.f, 250.f));

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText(constants::CHAT_TITLE_TEXT);
    _titleText->setSize(math::Vector2f(2000.f, 250.f));
    _titleText->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    _titleText->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    _titleText->setFontSize(56);
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    ui::LayoutConfig messagesConfig;
    messagesConfig.direction = ui::LayoutDirection::Vertical;
    messagesConfig.alignment = ui::LayoutAlignment::Start;
    messagesConfig.spacing = 22.0f;
    messagesConfig.padding = math::Vector2f(30.0f, 25.0f);
    messagesConfig.background.enabled = true;
    messagesConfig.background.fillColor = colors::UI_BACKGROUND;
    messagesConfig.background.outlineColor = colors::UI_OUTLINE;
    messagesConfig.background.cornerRadius = 15.0f;
    messagesConfig.anchorX = ui::AnchorX::Left;
    messagesConfig.anchorY = ui::AnchorY::Top;
    messagesConfig.offset = math::Vector2f(505.0f, 235.0f);

    _messagesContainer = std::make_shared<ui::UILayout>(_resourceManager, messagesConfig);
    _messagesContainer->setSize(math::Vector2f(900.f, 475.f));
    _messagesContainer->setBackgroundEnabled(true);
    _messagesContainer->setBackgroundFillColor(colors::UI_BACKGROUND);
    _messagesContainer->setBackgroundOutlineColor(colors::UI_OUTLINE);
    _messagesContainer->setBackgroundCornerRadius(15.0f);

    auto placeholderText = std::make_shared<ui::Text>(_resourceManager);
    placeholderText->setText(constants::CHAT_NO_MESSAGES_TEXT);
    placeholderText->setTextColor(gfx::color_t{150, 150, 150, 255});
    placeholderText->setFontSize(18);
    _messagesContainer->addElement(placeholderText);

    _messageInput = std::make_shared<ui::TextInput>(_resourceManager);
    _messageInput->setPlaceholder(constants::CHAT_PLACEHOLDER_TEXT);
    _messageInput->setSize(math::Vector2f{800, 50});
    _messageInput->setMaxLength(45);
    _messageInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_messageInput);
    });

    _sendButton = std::make_shared<ui::Button>(_resourceManager);
    _sendButton->setText(constants::SEND_BUTTON_TEXT);
    _sendButton->setSize(math::Vector2f{120, 50});
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
    controlsConfig.spacing = 15.0f;
    controlsConfig.padding = math::Vector2f(0.0f, 0.0f);
    controlsConfig.anchorX = ui::AnchorX::Center;
    controlsConfig.anchorY = ui::AnchorY::Bottom;
    controlsConfig.offset = math::Vector2f(0.0f, -280.0f);

    auto controlsLayout = std::make_shared<ui::UILayout>(_resourceManager, controlsConfig);
    controlsLayout->setSize(math::Vector2f(900.f, 60.f));

    controlsLayout->addElement(_messageInput);
    controlsLayout->addElement(_sendButton);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText(constants::BACK_BUTTON_TEXT_UPPER);
    _backButton->setSize(math::Vector2f(400.f, 70.f));
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });
    _backButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    ui::LayoutConfig backButtonConfig;
    backButtonConfig.anchorX = ui::AnchorX::Center;
    backButtonConfig.anchorY = ui::AnchorY::Bottom;
    backButtonConfig.offset = math::Vector2f(0.0f, -40.0f);

    auto backButtonLayout = std::make_shared<ui::UILayout>(_resourceManager, backButtonConfig);
    backButtonLayout->setSize(math::Vector2f(140.f, 50.f));
    backButtonLayout->addElement(_backButton);

    _uiManager->addElement(titleLayout);
    _uiManager->addElement(_messagesContainer);
    _uiManager->addElement(controlsLayout);
    _uiManager->addElement(backButtonLayout);

    _uiManager->focusFirstElement();
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

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

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
