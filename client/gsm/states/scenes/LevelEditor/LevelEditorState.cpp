/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorState
*/

#include "LevelEditorState.hpp"
#include <optional>
#include <filesystem>  // NOLINT(build/c++17)
#include <fstream>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../ui/elements/base/UIElement.hpp"

namespace gsm {

LevelEditorState::LevelEditorState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    std::optional<std::filesystem::path> levelPath
) : AGameState(gsm, resourceManager), _levelPath(levelPath) {
    if (_levelPath) {
        try {
            std::ifstream file(*_levelPath);
            file >> _levelData;
            file.close();
        } catch (const std::exception& e) {
            if (auto stateMachine = gsm) {
                stateMachine->requestStatePop();
            }
            return;
        }
    } else {
        if (auto stateMachine = gsm) {
            stateMachine->requestStatePop();
        }
        return;
    }

    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    createUI();
}

void LevelEditorState::enter() {
    _uiManager->setOnBack([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
}

void LevelEditorState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (_uiManager->getGlobalScale() != config->getUIScale()) {
        _uiManager->setGlobalScale(config->getUIScale());
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
    _resourceManager->get<gfx::IWindow>()->setCursor(isHoveringUI);

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void LevelEditorState::renderUI() {
    _uiManager->render();
}

void LevelEditorState::createUI() {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasWidth = constants::MAX_WIDTH - sidePanelWidth;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    _sidePanel = std::make_shared<ui::Panel>(_resourceManager);
    _sidePanel->setPosition(math::Vector2f(0.0f, 0.0f));
    _sidePanel->setSize(math::Vector2f(sidePanelWidth, constants::MAX_HEIGHT));
    _sidePanel->setBackgroundColor(colors::LIGHT_GRAY);
    _sidePanel->setBorderColor(colors::GRAY);
    _uiManager->addElement(_sidePanel);

    _saveButton = std::make_shared<ui::Button>(_resourceManager);
    _saveButton->setPosition(math::Vector2f(10.0f, 15.0f));
    _saveButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _saveButton->setText("Save Level");
    _saveButton->setOnClick([this]() {
        if (!_levelPath || !validateFields()) {
            return;
        }

        std::string levelName = _levelNameInput->getText();
        std::string mapLengthStr = _mapLengthInput->getText();
        std::string scrollSpeedStr = _scrollSpeedInput->getText();

        _levelData[constants::LEVEL_NAME_FIELD] = levelName;

        try {
            float mapLength = std::stof(mapLengthStr);
            _levelData[constants::LEVEL_MAP_LENGTH_FIELD] = mapLength;
        } catch (const std::exception&) {
        }

        try {
            int scrollSpeed = std::stoi(scrollSpeedStr);
            _levelData[constants::LEVEL_SCROLL_SPEED_FIELD] = scrollSpeed;
        } catch (const std::exception&) {
        }

        std::filesystem::path savePath = *_levelPath;

        std::ofstream file(savePath);
        if (file.is_open()) {
            file << _levelData.dump(4);
            file.close();
            _hasUnsavedChanges = false;
            updateSaveButtonText();
        }
    });
    _saveButton->setScalingEnabled(false);
    _saveButton->setFocusEnabled(false);
    _sidePanel->addChild(_saveButton);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setPosition(math::Vector2f(10.0f, 70.0f));
    _backButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _backButton->setText("Back");
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnClick([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });
    _backButton->setScalingEnabled(false);
    _backButton->setFocusEnabled(false);
    _sidePanel->addChild(_backButton);

    float currentY = 150.0f;
    const float elementSpacing = 20.0f;
    const float labelToFieldSpacing = 40.0f;

    _nameLabel = std::make_shared<ui::Text>(_resourceManager);
    _nameLabel->setPosition(math::Vector2f(10.0f, currentY));
    _nameLabel->setText("Name");
    _nameLabel->setFontSize(24);
    _nameLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_nameLabel);

    currentY += labelToFieldSpacing;
    _levelNameInput = std::make_shared<ui::TextInput>(_resourceManager);
    _levelNameInput->setPosition(math::Vector2f(10.0f, currentY));
    _levelNameInput->setSize(math::Vector2f(sidePanelWidth - 25.0f, 30.0f));
    _levelNameInput->setText(_levelData.value(constants::LEVEL_NAME_FIELD, "New Level"));
    _levelNameInput->setPlaceholder("Enter level name...");
    _levelNameInput->setOnTextChanged([this](const std::string& /*text*/) {
        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
    });
    _levelNameInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_levelNameInput);
    });
    _levelNameInput->setScalingEnabled(false);
    _levelNameInput->setFocusEnabled(true);
    _sidePanel->addChild(_levelNameInput);

    currentY += 40.0f + elementSpacing;
    _mapLengthLabel = std::make_shared<ui::Text>(_resourceManager);
    _mapLengthLabel->setPosition(math::Vector2f(10.0f, currentY));
    _mapLengthLabel->setText("Map Length");
    _mapLengthLabel->setFontSize(24);
    _mapLengthLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_mapLengthLabel);

    currentY += labelToFieldSpacing;
    _mapLengthInput = std::make_shared<ui::TextInput>(_resourceManager);
    _mapLengthInput->setPosition(math::Vector2f(10.0f, currentY));
    _mapLengthInput->setSize(math::Vector2f(sidePanelWidth - 25.0f, 30.0f));
    _mapLengthInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0))));
    _mapLengthInput->setPlaceholder("Enter map length...");
    _mapLengthInput->setOnTextChanged([this](const std::string& text) {
        std::string filteredText;
        for (char c : text) {
            if (c >= '0' && c <= '9') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _mapLengthInput->setText(filteredText);
            return;
        }

        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
    });
    _mapLengthInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_mapLengthInput);
    });
    _mapLengthInput->setScalingEnabled(false);
    _mapLengthInput->setFocusEnabled(true);
    _sidePanel->addChild(_mapLengthInput);

    currentY += 40.0f + elementSpacing;
    _scrollSpeedLabel = std::make_shared<ui::Text>(_resourceManager);
    _scrollSpeedLabel->setPosition(math::Vector2f(10.0f, currentY));
    _scrollSpeedLabel->setText("Scroll Speed");
    _scrollSpeedLabel->setFontSize(24);
    _scrollSpeedLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_scrollSpeedLabel);

    currentY += labelToFieldSpacing;
    _scrollSpeedInput = std::make_shared<ui::TextInput>(_resourceManager);
    _scrollSpeedInput->setPosition(math::Vector2f(10.0f, currentY));
    _scrollSpeedInput->setSize(math::Vector2f(sidePanelWidth - 25.0f, 30.0f));
    _scrollSpeedInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::LEVEL_SCROLL_SPEED_FIELD, 1))));
    _scrollSpeedInput->setPlaceholder("Enter scroll speed...");
    _scrollSpeedInput->setOnTextChanged([this](const std::string& text) {
        std::string filteredText;
        for (char c : text) {
            if (c >= '0' && c <= '9') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _scrollSpeedInput->setText(filteredText);
            return;
        }

        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
    });
    _scrollSpeedInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_scrollSpeedInput);
    });
    _scrollSpeedInput->setScalingEnabled(false);
    _scrollSpeedInput->setFocusEnabled(true);
    _sidePanel->addChild(_scrollSpeedInput);

    _bottomPanel = std::make_shared<ui::Panel>(_resourceManager);
    _bottomPanel->setPosition(math::Vector2f(sidePanelWidth, canvasHeight));
    _bottomPanel->setSize(math::Vector2f(canvasWidth, bottomPanelHeight));
    _bottomPanel->setBackgroundColor(colors::LIGHT_GRAY);
    _bottomPanel->setBorderColor(colors::GRAY);
    _uiManager->addElement(_bottomPanel);

    _canvasPanel = std::make_shared<ui::Panel>(_resourceManager);
    _canvasPanel->setPosition(math::Vector2f(sidePanelWidth, 0.0f));
    _canvasPanel->setSize(math::Vector2f(canvasWidth, canvasHeight));
    _canvasPanel->setBackgroundColor(colors::BLACK);
    _canvasPanel->setBorderColor(colors::GRAY);
    _uiManager->addElement(_canvasPanel);

    _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
}

void LevelEditorState::updateSaveButtonText() {
    if (_saveButton) {
        std::string baseText = "Save Level";
        if (_hasUnsavedChanges) {
            baseText += "*";
        }
        _saveButton->setText(baseText);
    }
}

bool LevelEditorState::validateFields() {
    std::string levelName = _levelNameInput->getText();
    if (levelName.empty()) {
        return false;
    }

    std::string mapLengthStr = _mapLengthInput->getText();
    try {
        float mapLength = std::stof(mapLengthStr);
        if (mapLength <= 0.0f) {
            return false;
        }
    } catch (const std::exception&) {
        return false;
    }

    std::string scrollSpeedStr = _scrollSpeedInput->getText();
    try {
        int scrollSpeed = std::stoi(scrollSpeedStr);
        if (scrollSpeed <= 0) {
            return false;
        }
    } catch (const std::exception&) {
        return false;
    }

    return true;
}

void LevelEditorState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _background.reset();
    _sidePanel.reset();
    _bottomPanel.reset();
    _canvasPanel.reset();
    _saveButton.reset();
    _backButton.reset();
    _nameLabel.reset();
    _levelNameInput.reset();
    _mapLengthLabel.reset();
    _mapLengthInput.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
