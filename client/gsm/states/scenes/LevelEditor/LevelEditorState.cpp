/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorState
*/

#include "LevelEditorState.hpp"
#include <filesystem>  // NOLINT(build/c++17)
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <utility>
#include <nlohmann/json.hpp>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

LevelEditorState::LevelEditorState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _shouldUpdateUI = false;
    _shouldHideDeletePopup = false;

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText("Back to Menu");
    _backButton->setSize(math::Vector2f(300.f, 50.f));
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    createLevelSelectionUI();
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

    if (mousePressed && !isHoveringUI) {
        auto navManager = _uiManager->getNavigationManager();
        if (navManager) {
            navManager->clearFocus();
        }
    }

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);

    if (_shouldUpdateUI) {
        _shouldUpdateUI = false;
        createLevelSelectionUI();
    }

    if (_shouldHideDeletePopup) {
        _shouldHideDeletePopup = false;
        hideDeleteConfirmationPopup();
    }

    renderUI();
}

void LevelEditorState::renderUI() {
    _uiManager->render();
}

void LevelEditorState::createLevelSelectionUI() {
    _uiManager->clearElements();
    _uiManager->addElement(_background);
    _levelButtons.clear();
    _indexLabels.clear();

    auto availableLevels = getAvailableLevels();

    ui::LayoutConfig levelsLayoutConfig;
    levelsLayoutConfig.direction = ui::LayoutDirection::Vertical;
    levelsLayoutConfig.alignment = ui::LayoutAlignment::Center;
    levelsLayoutConfig.spacing = 20.0f;
    levelsLayoutConfig.padding = math::Vector2f(20.0f, 20.0f);
    levelsLayoutConfig.anchorX = ui::AnchorX::Center;
    levelsLayoutConfig.anchorY = ui::AnchorY::Center;
    levelsLayoutConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto levelsLayout = std::make_shared<ui::UILayout>(_resourceManager, levelsLayoutConfig);
    levelsLayout->setSize(math::Vector2f(500.f, 400.f));

    auto titleText = std::make_shared<ui::Text>(_resourceManager);
    titleText->setText("    Level Editor");
    titleText->setSize(math::Vector2f(300.f, 40.f));
    levelsLayout->addElement(titleText);

    if (availableLevels.empty()) {
        auto noLevelsText = std::make_shared<ui::Text>(_resourceManager);
        noLevelsText->setText("No levels available");
        noLevelsText->setSize(math::Vector2f(300.f, 30.f));
        levelsLayout->addElement(noLevelsText);
    } else {
        for (size_t i = 0; i < availableLevels.size(); ++i) {
            const auto& [levelPath, index] = availableLevels[i];

            std::string levelName = "Unknown";
            try {
                std::ifstream file(levelPath);
                nlohmann::json levelData;
                file >> levelData;
                file.close();
                levelName = levelData.value("name", levelPath.stem().string());
            } catch (const std::exception&) {
                levelName = levelPath.stem().string();
            }

            ui::LayoutConfig levelLayoutConfig;
            levelLayoutConfig.direction = ui::LayoutDirection::Horizontal;
            levelLayoutConfig.alignment = ui::LayoutAlignment::Center;
            levelLayoutConfig.spacing = 20.0f;
            levelLayoutConfig.padding = math::Vector2f(0.0f, 0.0f);
            levelLayoutConfig.anchorX = ui::AnchorX::Center;
            levelLayoutConfig.anchorY = ui::AnchorY::Center;
            levelLayoutConfig.offset = math::Vector2f(0.0f, 0.0f);

            auto levelLayout = std::make_shared<ui::UILayout>(
                _resourceManager, levelLayoutConfig);
            levelLayout->setSize(math::Vector2f(750.f, 40.f));

            auto indexText = std::make_shared<ui::Text>(_resourceManager);
            indexText->setText("NB: " + std::to_string(index));
            indexText->setSize(math::Vector2f(80.f, 40.f));
            indexText->setTextColor(gfx::color_t{255, 255, 255, 255});
            indexText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
            indexText->setOutlineThickness(1.0f);
            levelLayout->addElement(indexText);
            _indexLabels.push_back(indexText);

            auto levelButton = std::make_shared<ui::Button>(_resourceManager);
            levelButton->setText(levelName);
            levelButton->setSize(math::Vector2f(250.f, 40.f));
            levelButton->setNormalColor(colors::BUTTON_PRIMARY);
            levelButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
            levelButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);

            levelButton->setOnRelease([this, levelPath]() {
                // TODO(anyone): Switch to level editing scene
            });
            levelButton->setOnActivated([this, levelPath]() {
                // TODO(anyone): Switch to level editing scene
            });

            _levelButtons.push_back(levelButton);
            levelLayout->addElement(levelButton);

            ui::LayoutConfig controlLayoutConfig;
            controlLayoutConfig.direction = ui::LayoutDirection::Horizontal;
            controlLayoutConfig.alignment = ui::LayoutAlignment::Center;
            controlLayoutConfig.spacing = 15.0f;
            controlLayoutConfig.padding = math::Vector2f(0.0f, 0.0f);
            controlLayoutConfig.anchorX = ui::AnchorX::Center;
            controlLayoutConfig.anchorY = ui::AnchorY::Center;
            controlLayoutConfig.offset = math::Vector2f(0.0f, 0.0f);

            auto controlLayout = std::make_shared<ui::UILayout>(
                _resourceManager, controlLayoutConfig);
            controlLayout->setSize(math::Vector2f(250.f, 40.f));

            auto upButton = std::make_shared<ui::Button>(_resourceManager);
            upButton->setText("Up");
            upButton->setSize(math::Vector2f(70.f, 40.f));
            upButton->setNormalColor(colors::BUTTON_SECONDARY);
            upButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
            upButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

            bool canGoUp = (i > 0);
            if (!canGoUp) {
                upButton->setState(ui::UIState::Disabled);
            } else {
                upButton->setOnRelease(
                    [this, levelPath, nextLevelPath = availableLevels[i - 1].first]() {
                    swapLevels(levelPath, nextLevelPath);
                });
                upButton->setOnActivated(
                    [this, levelPath, nextLevelPath = availableLevels[i - 1].first]() {
                    swapLevels(levelPath, nextLevelPath);
                });
            }

            controlLayout->addElement(upButton);

            auto downButton = std::make_shared<ui::Button>(_resourceManager);
            downButton->setText("Down");
            downButton->setSize(math::Vector2f(90.f, 40.f));
            downButton->setNormalColor(colors::BUTTON_SECONDARY);
            downButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
            downButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

            bool canGoDown = (i < availableLevels.size() - 1);
            if (!canGoDown) {
                downButton->setState(ui::UIState::Disabled);
            } else {
                downButton->setOnRelease(
                    [this, levelPath, nextLevelPath = availableLevels[i + 1].first]() {
                    swapLevels(levelPath, nextLevelPath);
                });
                downButton->setOnActivated(
                    [this, levelPath, nextLevelPath = availableLevels[i + 1].first]() {
                    swapLevels(levelPath, nextLevelPath);
                });
            }

            controlLayout->addElement(downButton);

            auto deleteButton = std::make_shared<ui::Button>(_resourceManager);
            deleteButton->setText("Delete");
            deleteButton->setSize(math::Vector2f(110.f, 40.f));
            deleteButton->setNormalColor(gfx::color_t{255, 100, 100, 255});
            deleteButton->setHoveredColor(gfx::color_t{255, 150, 150, 255});
            deleteButton->setPressedColor(gfx::color_t{255, 50, 50, 255});

            deleteButton->setOnRelease([this, levelPath, levelName]() {
                showDeleteConfirmation(levelPath, levelName);
            });
            deleteButton->setOnActivated([this, levelPath, levelName]() {
                showDeleteConfirmation(levelPath, levelName);
            });

            controlLayout->addElement(deleteButton);

            levelLayout->addElement(controlLayout);

            levelsLayout->addElement(levelLayout);
        }
    }

    _addLevelButton = std::make_shared<ui::Button>(_resourceManager);
    _addLevelButton->setText("Add Level");
    _addLevelButton->setSize(math::Vector2f(400.f, 40.f));
    _addLevelButton->setNormalColor(colors::BUTTON_SECONDARY);
    _addLevelButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _addLevelButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

    _addLevelButton->setOnRelease([]() {
        // TODO(anyone): Implement add level functionality
    });
    _addLevelButton->setOnActivated([]() {
        // TODO(anyone): Implement add level functionality
    });
    levelsLayout->addElement(_addLevelButton);

    _uiManager->addElement(levelsLayout);

    ui::LayoutConfig backLayoutConfig;
    backLayoutConfig.direction = ui::LayoutDirection::Vertical;
    backLayoutConfig.alignment = ui::LayoutAlignment::Center;
    backLayoutConfig.spacing = 15.0f;
    backLayoutConfig.padding = math::Vector2f(20.0f, 20.0f);
    backLayoutConfig.anchorX = ui::AnchorX::Center;
    backLayoutConfig.anchorY = ui::AnchorY::Bottom;
    backLayoutConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto backLayout = std::make_shared<ui::UILayout>(_resourceManager, backLayoutConfig);
    backLayout->setSize(math::Vector2f(300.f, 100.f));
    backLayout->addElement(_backButton);

    _uiManager->addElement(backLayout);
}

std::vector<std::pair<std::filesystem::path, int>> LevelEditorState::getAvailableLevels() {
    std::vector<std::pair<std::filesystem::path, int>> levels;
    std::filesystem::path levelDir = "configs/map";

    if (std::filesystem::exists(levelDir) && std::filesystem::is_directory(levelDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(levelDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json" &&
                entry.path().stem().string().find("level") == 0) {
                try {
                    std::ifstream file(entry.path());
                    nlohmann::json levelData;
                    file >> levelData;
                    file.close();

                    int index = levelData.value("index", -1);
                    std::string name = levelData.value("name", entry.path().stem().string());

                    levels.emplace_back(entry.path(), index);
                } catch (const std::exception&) {
                    continue;
                }
            }
        }

        std::sort(levels.begin(), levels.end(),
            [](const std::pair<std::filesystem::path, int>& a, const std::pair<
                std::filesystem::path, int>& b) {
                return a.second < b.second;
            });
    }

    return levels;
}

void LevelEditorState::swapLevels(
    const std::filesystem::path& path1,
    const std::filesystem::path& path2
) {
    int index1 = -1;
    int index2 = -1;

    try {
        std::ifstream file1(path1);
        nlohmann::json data1;
        file1 >> data1;
        file1.close();
        index1 = data1.value("index", -1);
    } catch (const std::exception&) {
        return;
    }

    try {
        std::ifstream file2(path2);
        nlohmann::json data2;
        file2 >> data2;
        file2.close();
        index2 = data2.value("index", -1);
    } catch (const std::exception&) {
        return;
    }

    try {
        std::ifstream file1(path1);
        nlohmann::json data1;
        file1 >> data1;
        file1.close();

        data1["index"] = index2;

        std::ofstream outFile1(path1);
        outFile1 << data1.dump(4);
        outFile1.close();
    } catch (const std::exception&) {
        return;
    }

    try {
        std::ifstream file2(path2);
        nlohmann::json data2;
        file2 >> data2;
        file2.close();

        data2["index"] = index1;

        std::ofstream outFile2(path2);
        outFile2 << data2.dump(4);
        outFile2.close();
    } catch (const std::exception&) {
        return;
    }

    _shouldUpdateUI = true;
}

void LevelEditorState::showDeleteConfirmation(
    const std::filesystem::path& levelPath,
    const std::string& levelName
) {
    showDeleteConfirmationPopup(levelPath, levelName);
}

void LevelEditorState::showDeleteConfirmationPopup(
    const std::filesystem::path& levelPath,
    const std::string& levelName
) {
    _pendingDeletePath = levelPath;

    setMainButtonsEnabled(false);

    ui::LayoutConfig overlayConfig;
    overlayConfig.direction = ui::LayoutDirection::Vertical;
    overlayConfig.alignment = ui::LayoutAlignment::Center;
    overlayConfig.spacing = 0.0f;
    overlayConfig.padding = math::Vector2f(0.0f, 0.0f);
    overlayConfig.anchorX = ui::AnchorX::Center;
    overlayConfig.anchorY = ui::AnchorY::Center;
    overlayConfig.offset = math::Vector2f(0.0f, 0.0f);

    _deletePopupOverlay = std::make_shared<ui::UILayout>(_resourceManager, overlayConfig);
    _deletePopupOverlay->setSize(math::Vector2f(1920.f, 1080.f));

    _uiManager->addElement(_deletePopupOverlay);

    ui::LayoutConfig popupConfig;
    popupConfig.direction = ui::LayoutDirection::Vertical;
    popupConfig.alignment = ui::LayoutAlignment::Start;
    popupConfig.spacing = 25.0f;
    popupConfig.padding = math::Vector2f(60.0f, 40.0f);
    popupConfig.anchorX = ui::AnchorX::Center;
    popupConfig.anchorY = ui::AnchorY::Center;
    popupConfig.offset = math::Vector2f(20.0f, 0.0f);
    popupConfig.background.enabled = true;
    popupConfig.background.fillColor = colors::BLACK;
    popupConfig.background.outlineColor = colors::WHITE;
    popupConfig.background.cornerRadius = 15.0f;

    _deletePopupLayout = std::make_shared<ui::UILayout>(_resourceManager, popupConfig);
    _deletePopupLayout->setSize(math::Vector2f(600.f, 250.f));

    _deletePopupText = std::make_shared<ui::Text>(_resourceManager);
    _deletePopupText->setText("Are you sure you want to delete\n\"" +
        levelName + "\"?\n\nThis action cannot be undone.");
    _deletePopupText->setSize(math::Vector2f(500.f, 100.f));
    _deletePopupText->setTextColor(gfx::color_t{255, 255, 255, 255});
    _deletePopupLayout->addElement(_deletePopupText);

    ui::LayoutConfig buttonsConfig;
    buttonsConfig.direction = ui::LayoutDirection::Horizontal;
    buttonsConfig.alignment = ui::LayoutAlignment::Center;
    buttonsConfig.spacing = 20.0f;
    buttonsConfig.padding = math::Vector2f(0.0f, 0.0f);
    buttonsConfig.anchorX = ui::AnchorX::Center;
    buttonsConfig.anchorY = ui::AnchorY::Center;
    buttonsConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto buttonsLayout = std::make_shared<ui::UILayout>(_resourceManager, buttonsConfig);
    buttonsLayout->setSize(math::Vector2f(300.f, 50.f));

    _deleteCancelButton = std::make_shared<ui::Button>(_resourceManager);
    _deleteCancelButton->setText("Cancel");
    _deleteCancelButton->setSize(math::Vector2f(140.f, 45.f));
    _deleteCancelButton->setNormalColor(colors::BUTTON_SECONDARY);
    _deleteCancelButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _deleteCancelButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _deleteCancelButton->setOnRelease([this]() {
        _shouldHideDeletePopup = true;
    });
    buttonsLayout->addElement(_deleteCancelButton);

    _deleteConfirmButton = std::make_shared<ui::Button>(_resourceManager);
    _deleteConfirmButton->setText("Delete");
    _deleteConfirmButton->setSize(math::Vector2f(140.f, 45.f));
    _deleteConfirmButton->setNormalColor(gfx::color_t{255, 100, 100, 255});
    _deleteConfirmButton->setHoveredColor(gfx::color_t{255, 150, 150, 255});
    _deleteConfirmButton->setPressedColor(gfx::color_t{255, 50, 50, 255});
    _deleteConfirmButton->setOnRelease([this]() {
        confirmDelete();
    });
    buttonsLayout->addElement(_deleteConfirmButton);

    _deletePopupLayout->addElement(buttonsLayout);

    _uiManager->addElement(_deletePopupLayout);
}

void LevelEditorState::confirmDelete() {
    if (!_pendingDeletePath.empty()) {
        try {
            std::filesystem::remove(_pendingDeletePath);
            _shouldUpdateUI = true;
        } catch (const std::exception&) {
        }
    }
    _shouldHideDeletePopup = true;
}

void LevelEditorState::setMainButtonsEnabled(bool enabled) {
    ui::UIState state = enabled ? ui::UIState::Normal : ui::UIState::Disabled;

    for (auto& button : _levelButtons) {
        if (button) {
            button->setState(state);
        }
    }

    if (_addLevelButton) {
        _addLevelButton->setState(state);
    }

    if (_backButton) {
        _backButton->setState(state);
    }
}

void LevelEditorState::hideDeleteConfirmationPopup() {
    if (_deletePopupOverlay) {
        _uiManager->removeElement(_deletePopupOverlay);
        _deletePopupOverlay.reset();
    }
    if (_deletePopupLayout) {
        _uiManager->removeElement(_deletePopupLayout);
        _deletePopupLayout.reset();
        _deletePopupText.reset();
        _deleteCancelButton.reset();
        _deleteConfirmButton.reset();
    }
    _pendingDeletePath.clear();

    setMainButtonsEnabled(true);
}

void LevelEditorState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _levelButtons.clear();
    _indexLabels.clear();
    _background.reset();
    _backButton.reset();
    _addLevelButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
