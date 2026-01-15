/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorSelectorState
*/

#include "LevelEditorSelectorState.hpp"
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

LevelEditorSelectorState::LevelEditorSelectorState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);
    _shouldUpdateUI = false;
    _lastLevelCount = 0;
    _shouldHideDeletePopup = false;
    _shouldHideDuplicatePopup = false;
    _currentPage = 0;

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(6487.0f, 3584.0f));
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

void LevelEditorSelectorState::enter() {
    _uiManager->setOnBack([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(6487.0f, 3584.0f));

    _shouldUpdateUI = true;
}

void LevelEditorSelectorState::update(float deltaTime) {
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

    auto navManager = _uiManager->getNavigationManager();
    if (navManager) {
        navManager->clearFocus();
    }

    _uiManager->update(deltaTime);

    size_t currentLevelCount = getAvailableLevels().size();
    if (currentLevelCount != _lastLevelCount) {
        _lastLevelCount = currentLevelCount;
        _shouldUpdateUI = true;
    }

    if (_shouldUpdateUI) {
        _shouldUpdateUI = false;
        createLevelSelectionUI();
    }

    if (_shouldHideDeletePopup) {
        _shouldHideDeletePopup = false;
        hideDeleteConfirmationPopup();
    }

    if (_shouldHideDuplicatePopup) {
        _shouldHideDuplicatePopup = false;
        hideDuplicateConfirmationPopup();
    }

    renderUI();
}

void LevelEditorSelectorState::renderUI() {
    _uiManager->render();
}

void LevelEditorSelectorState::createLevelSelectionUI() {
    _uiManager->clearElements();
    _uiManager->addElement(_background);
    _levelButtons.clear();
    _indexLabels.clear();
    _upButtons.clear();
    _downButtons.clear();
    _duplicateButtons.clear();
    _deleteButtons.clear();

    auto availableLevels = getAvailableLevels();
    int totalLevels = static_cast<int>(availableLevels.size());
    size_t start =
        static_cast<size_t>(_currentPage) * static_cast<size_t>(_levelsPerPage);
    size_t end = (std::min)(start + static_cast<size_t>(_levelsPerPage),
        static_cast<size_t>(totalLevels));
    size_t startIdx = start;
    size_t endIdx = end;

    ui::LayoutConfig levelsLayoutConfig;
    levelsLayoutConfig.direction = ui::LayoutDirection::Vertical;
    levelsLayoutConfig.alignment = ui::LayoutAlignment::Center;
    levelsLayoutConfig.spacing = 20.0f;
    levelsLayoutConfig.padding = math::Vector2f(20.0f, 20.0f);
    levelsLayoutConfig.anchorX = ui::AnchorX::Center;
    levelsLayoutConfig.anchorY = ui::AnchorY::Center;
    levelsLayoutConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto levelsLayout = std::make_shared<ui::UILayout>(_resourceManager, levelsLayoutConfig);
    levelsLayout->setSize(math::Vector2f(800.f, 400.f));

    auto titleText = std::make_shared<ui::Text>(_resourceManager);
    titleText->setText(
        "Level Editor - Not compatible with controllers - Press BACK to return");
    titleText->setSize(math::Vector2f(900.f, 40.f));
    levelsLayout->addElement(titleText);

    if (availableLevels.empty()) {
        auto noLevelsText = std::make_shared<ui::Text>(_resourceManager);
        noLevelsText->setText("No levels available");
        noLevelsText->setSize(math::Vector2f(300.f, 30.f));
        levelsLayout->addElement(noLevelsText);
    } else {
        for (size_t i = startIdx; i < endIdx; ++i) {
            const auto& [levelPath, index] = availableLevels[i];

            std::string levelName = "Unknown";
            try {
                std::ifstream file(levelPath);
                nlohmann::json levelData;
                file >> levelData;
                file.close();
                levelName = levelData.value(
                    constants::NAME_FIELD, levelPath.stem().string());
            } catch (const std::exception&) {
                levelName = levelPath.stem().string();
            }

            const size_t maxLength = 15;
            if (levelName.length() > maxLength) {
                levelName = levelName.substr(0, maxLength - 3) + "...";
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
                if (auto stateMachine = this->_gsm.lock()) {
                    stateMachine->requestStatePush(
                        std::make_shared<LevelEditorState>(stateMachine,
                        this->_resourceManager, levelPath));
                }
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
            }

            controlLayout->addElement(upButton);
            _upButtons.push_back(upButton);

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
            }

            controlLayout->addElement(downButton);
            _downButtons.push_back(downButton);

            auto duplicateButton = std::make_shared<ui::Button>(_resourceManager);
            duplicateButton->setText("Duplicate");
            duplicateButton->setSize(math::Vector2f(150.f, 40.f));
            duplicateButton->setNormalColor(colors::BUTTON_SECONDARY);
            duplicateButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
            duplicateButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

            duplicateButton->setOnRelease([this, levelPath, levelName]() {
                showDuplicateConfirmation(levelPath, levelName);
            });

            controlLayout->addElement(duplicateButton);
            _duplicateButtons.push_back(duplicateButton);

            auto deleteButton = std::make_shared<ui::Button>(_resourceManager);
            deleteButton->setText("Delete");
            deleteButton->setSize(math::Vector2f(110.f, 40.f));
            deleteButton->setNormalColor(colors::BUTTON_DANGER);
            deleteButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
            deleteButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);

            deleteButton->setOnRelease([this, levelPath, levelName]() {
                showDeleteConfirmation(levelPath, levelName);
            });

            controlLayout->addElement(deleteButton);
            _deleteButtons.push_back(deleteButton);

            levelLayout->addElement(controlLayout);

            levelsLayout->addElement(levelLayout);
        }
    }

    ui::LayoutConfig paginationConfig;
    paginationConfig.direction = ui::LayoutDirection::Vertical;
    paginationConfig.alignment = ui::LayoutAlignment::Center;
    paginationConfig.spacing = 10.0f;
    paginationConfig.padding = math::Vector2f(0.0f, 0.0f);

    auto paginationLayout = std::make_shared<ui::UILayout>(_resourceManager, paginationConfig);
    paginationLayout->setSize(math::Vector2f(800.f, 100.f));

    int totalPages = (totalLevels + _levelsPerPage - 1) / _levelsPerPage;
    auto pageText = std::make_shared<ui::Text>(_resourceManager);
    pageText->setText(
        "Page " + std::to_string(_currentPage + 1) + " / " + std::to_string(totalPages));
    pageText->setSize(math::Vector2f(800.f, 40.f));
    pageText->setTextColor(gfx::color_t{255, 255, 255, 255});
    paginationLayout->addElement(pageText);

    ui::LayoutConfig buttonLayoutConfig;
    buttonLayoutConfig.direction = ui::LayoutDirection::Horizontal;
    buttonLayoutConfig.alignment = ui::LayoutAlignment::Center;
    buttonLayoutConfig.spacing = 25.0f;
    buttonLayoutConfig.padding = math::Vector2f(0.0f, 0.0f);

    auto buttonLayout = std::make_shared<ui::UILayout>(_resourceManager, buttonLayoutConfig);
    buttonLayout->setSize(math::Vector2f(800.f, 40.f));

    auto prevButton = std::make_shared<ui::Button>(_resourceManager);
    prevButton->setText("Previous");
    prevButton->setSize(math::Vector2f(180.f, 40.f));
    prevButton->setNormalColor(colors::BUTTON_SECONDARY);
    prevButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    prevButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

    if (_currentPage > 0) {
        prevButton->setOnRelease([this]() {
            _currentPage--;
            _shouldUpdateUI = true;
        });
    } else {
        prevButton->setState(ui::UIState::Disabled);
    }

    buttonLayout->addElement(prevButton);
    _prevButton = prevButton;

    auto nextButton = std::make_shared<ui::Button>(_resourceManager);
    nextButton->setText("Next");
    nextButton->setSize(math::Vector2f(180.f, 40.f));
    nextButton->setNormalColor(colors::BUTTON_SECONDARY);
    nextButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    nextButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

    if (_currentPage < totalPages - 1) {
        nextButton->setOnRelease([this]() {
            _currentPage++;
            _shouldUpdateUI = true;
        });
    } else {
        nextButton->setState(ui::UIState::Disabled);
    }

    buttonLayout->addElement(nextButton);
    _nextButton = nextButton;

    paginationLayout->addElement(buttonLayout);

    levelsLayout->addElement(paginationLayout);

    _uiManager->addElement(levelsLayout);

    _addLevelButton = std::make_shared<ui::Button>(_resourceManager);
    _addLevelButton->setText("Add Level");
    _addLevelButton->setSize(math::Vector2f(300.f, 50.f));
    _addLevelButton->setNormalColor(colors::BUTTON_SECONDARY);
    _addLevelButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _addLevelButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);

    _addLevelButton->setOnRelease([this]() {
        auto newLevelPath = createNewLevel();
        if (newLevelPath) {
            auto stateMachine = this->_gsm.lock();
            if (stateMachine) {
                stateMachine->requestStatePush(std::make_shared<LevelEditorState>(
                    stateMachine, this->_resourceManager, *newLevelPath));
            }
        }
    });

    ui::LayoutConfig backLayoutConfig;
    backLayoutConfig.direction = ui::LayoutDirection::Horizontal;
    backLayoutConfig.alignment = ui::LayoutAlignment::Center;
    backLayoutConfig.spacing = 20.0f;
    backLayoutConfig.padding = math::Vector2f(20.0f, 20.0f);
    backLayoutConfig.anchorX = ui::AnchorX::Center;
    backLayoutConfig.anchorY = ui::AnchorY::Bottom;
    backLayoutConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto backLayout = std::make_shared<ui::UILayout>(_resourceManager, backLayoutConfig);
    backLayout->setSize(math::Vector2f(650.f, 100.f));
    backLayout->addElement(_backButton);
    backLayout->addElement(_addLevelButton);

    _uiManager->addElement(backLayout);
}

std::vector<
    std::pair<std::filesystem::path, int>
> LevelEditorSelectorState::getAvailableLevels() {
    std::vector<std::pair<std::filesystem::path, int>> levels;
    std::filesystem::path levelDir = constants::LEVEL_DIRECTORY;

    if (std::filesystem::exists(levelDir) && std::filesystem::is_directory(levelDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(levelDir)) {
            if (entry.is_regular_file() && entry.path().extension() ==
                constants::LEVEL_FILE_EXTENSION &&
                entry.path().stem().string().find(constants::LEVEL_FILE_PREFIX) == 0) {
                try {
                    std::ifstream file(entry.path());
                    nlohmann::json levelData;
                    file >> levelData;
                    file.close();

                    int index = levelData.value(constants::INDEX_FIELD, -1);
                    std::string name = levelData.value(
                        constants::NAME_FIELD, entry.path().stem().string());

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

void LevelEditorSelectorState::swapLevels(
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
        index1 = data1.value(constants::INDEX_FIELD, -1);
    } catch (const std::exception&) {
        return;
    }

    try {
        std::ifstream file2(path2);
        nlohmann::json data2;
        file2 >> data2;
        file2.close();
        index2 = data2.value(constants::INDEX_FIELD, -1);
    } catch (const std::exception&) {
        return;
    }

    try {
        std::ifstream file1(path1);
        nlohmann::json data1;
        file1 >> data1;
        file1.close();

        data1[constants::INDEX_FIELD] = index2;

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

        data2[constants::INDEX_FIELD] = index1;

        std::ofstream outFile2(path2);
        outFile2 << data2.dump(4);
        outFile2.close();
    } catch (const std::exception&) {
        return;
    }

    _shouldUpdateUI = true;
}

void LevelEditorSelectorState::showDeleteConfirmation(
    const std::filesystem::path& levelPath,
    const std::string& levelName
) {
    showDeleteConfirmationPopup(levelPath, levelName);
}

void LevelEditorSelectorState::showDuplicateConfirmation(
    const std::filesystem::path& levelPath,
    const std::string& levelName
) {
    showDuplicateConfirmationPopup(levelPath, levelName);
}

void LevelEditorSelectorState::showDeleteConfirmationPopup(
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
    _deletePopupText->setText("Are you sure you want to delete\n-- " +
        levelName + " -- ?\n\nThis action cannot be undone.");
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

void LevelEditorSelectorState::showDuplicateConfirmationPopup(
    const std::filesystem::path& levelPath,
    const std::string& levelName
) {
    _pendingDuplicatePath = levelPath;
    _pendingDuplicateName = levelName;

    setMainButtonsEnabled(false);

    ui::LayoutConfig overlayConfig;
    overlayConfig.direction = ui::LayoutDirection::Vertical;
    overlayConfig.alignment = ui::LayoutAlignment::Center;
    overlayConfig.spacing = 0.0f;
    overlayConfig.padding = math::Vector2f(0.0f, 0.0f);
    overlayConfig.anchorX = ui::AnchorX::Center;
    overlayConfig.anchorY = ui::AnchorY::Center;
    overlayConfig.offset = math::Vector2f(0.0f, 0.0f);

    _duplicatePopupOverlay = std::make_shared<ui::UILayout>(_resourceManager, overlayConfig);
    _duplicatePopupOverlay->setSize(math::Vector2f(1920.f, 1080.f));

    _uiManager->addElement(_duplicatePopupOverlay);

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

    _duplicatePopupLayout = std::make_shared<ui::UILayout>(_resourceManager, popupConfig);
    _duplicatePopupLayout->setSize(math::Vector2f(600.f, 250.f));

    _duplicatePopupText = std::make_shared<ui::Text>(_resourceManager);
    _duplicatePopupText->setText(
        "The level will be duplicated\nand added to the\nend of the other levels.");
    _duplicatePopupText->setSize(math::Vector2f(500.f, 100.f));
    _duplicatePopupText->setTextColor(gfx::color_t{255, 255, 255, 255});
    _duplicatePopupLayout->addElement(_duplicatePopupText);

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

    _duplicateCancelButton = std::make_shared<ui::Button>(_resourceManager);
    _duplicateCancelButton->setText("Cancel");
    _duplicateCancelButton->setSize(math::Vector2f(150.f, 45.f));
    _duplicateCancelButton->setNormalColor(colors::BUTTON_SECONDARY);
    _duplicateCancelButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _duplicateCancelButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _duplicateCancelButton->setOnRelease([this]() {
        _shouldHideDuplicatePopup = true;
    });
    buttonsLayout->addElement(_duplicateCancelButton);

    _duplicateConfirmButton = std::make_shared<ui::Button>(_resourceManager);
    _duplicateConfirmButton->setText("Duplicate");
    _duplicateConfirmButton->setSize(math::Vector2f(150.f, 45.f));
    _duplicateConfirmButton->setNormalColor(colors::BUTTON_SECONDARY);
    _duplicateConfirmButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _duplicateConfirmButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _duplicateConfirmButton->setOnRelease([this]() {
        confirmDuplicate();
    });
    buttonsLayout->addElement(_duplicateConfirmButton);

    _duplicatePopupLayout->addElement(buttonsLayout);

    _uiManager->addElement(_duplicatePopupLayout);
}

void LevelEditorSelectorState::confirmDelete() {
    if (!_pendingDeletePath.empty()) {
        try {
            std::filesystem::remove(_pendingDeletePath);
            _shouldUpdateUI = true;
        } catch (const std::exception&) {
        }
    }
    _shouldHideDeletePopup = true;
}

void LevelEditorSelectorState::confirmDuplicate() {
    if (!_pendingDuplicatePath.empty()) {
        try {
            std::ifstream originalFile(_pendingDuplicatePath);
            nlohmann::json levelData;
            originalFile >> levelData;
            originalFile.close();

            auto availableLevels = getAvailableLevels();
            int nextIndex = 0;
            for (const auto& [path, index] : availableLevels) {
                if (index >= nextIndex) {
                    nextIndex = index + 1;
                }
            }

            levelData[constants::INDEX_FIELD] = nextIndex;
            std::string newName = _pendingDuplicateName + " -- copy";
            levelData[constants::NAME_FIELD] = newName;

            std::string newFileName = constants::LEVEL_FILE_PREFIX +
                std::to_string(nextIndex) + constants::LEVEL_FILE_EXTENSION;
            std::filesystem::path newPath = constants::LEVEL_DIRECTORY + "/" + newFileName;

            std::ofstream newFile(newPath);
            newFile << levelData.dump(4);
            newFile.close();

            _shouldUpdateUI = true;
        } catch (const std::exception&) {
        }
    }
    _shouldHideDuplicatePopup = true;
}

void LevelEditorSelectorState::setMainButtonsEnabled(bool enabled) {
    ui::UIState state = enabled ? ui::UIState::Normal : ui::UIState::Disabled;

    for (auto& button : _levelButtons) {
        if (button) {
            button->setState(state);
        }
    }

    for (auto& button : _upButtons) {
        if (button) {
            button->setState(state);
        }
    }

    for (auto& button : _downButtons) {
        if (button) {
            button->setState(state);
        }
    }

    for (auto& button : _duplicateButtons) {
        if (button) {
            button->setState(state);
        }
    }

    for (auto& button : _deleteButtons) {
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

    if (_prevButton) {
        if (enabled && _currentPage > 0) {
            _prevButton->setState(ui::UIState::Normal);
        } else {
            _prevButton->setState(ui::UIState::Disabled);
        }
    }

    if (_nextButton) {
        int totalLevels = static_cast<int>(getAvailableLevels().size());
        int totalPages = (totalLevels + _levelsPerPage - 1) / _levelsPerPage;
        if (enabled && _currentPage < totalPages - 1) {
            _nextButton->setState(ui::UIState::Normal);
        } else {
            _nextButton->setState(ui::UIState::Disabled);
        }
    }
}

void LevelEditorSelectorState::hideDeleteConfirmationPopup() {
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

void LevelEditorSelectorState::hideDuplicateConfirmationPopup() {
    if (_duplicatePopupOverlay) {
        _uiManager->removeElement(_duplicatePopupOverlay);
        _duplicatePopupOverlay.reset();
    }
    if (_duplicatePopupLayout) {
        _uiManager->removeElement(_duplicatePopupLayout);
        _duplicatePopupLayout.reset();
        _duplicatePopupText.reset();
        _duplicateCancelButton.reset();
        _duplicateConfirmButton.reset();
    }
    _pendingDuplicatePath.clear();
    _pendingDuplicateName.clear();

    setMainButtonsEnabled(true);
}

void LevelEditorSelectorState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _levelButtons.clear();
    _indexLabels.clear();
    _upButtons.clear();
    _downButtons.clear();
    _duplicateButtons.clear();
    _deleteButtons.clear();
    _background.reset();
    _backButton.reset();
    _addLevelButton.reset();
    _prevButton.reset();
    _nextButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
    _deletePopupOverlay.reset();
    _deletePopupLayout.reset();
    _deletePopupText.reset();
    _deleteCancelButton.reset();
    _deleteConfirmButton.reset();
    _duplicatePopupOverlay.reset();
    _duplicatePopupLayout.reset();
    _duplicatePopupText.reset();
    _duplicateCancelButton.reset();
    _duplicateConfirmButton.reset();
}

std::optional<std::filesystem::path> LevelEditorSelectorState::createNewLevel() {
    auto availableLevels = getAvailableLevels();
    int nextIndex = 0;
    for (const auto& [path, index] : availableLevels) {
        if (index >= nextIndex) {
            nextIndex = index + 1;
        }
    }

    nlohmann::json newLevelData;
    newLevelData[constants::INDEX_FIELD] = nextIndex;
    newLevelData[constants::NAME_FIELD] = "New Level";
    newLevelData[constants::BACKGROUND_FIELD] = "";
    newLevelData[constants::BACKGROUND_SCROLL_SPEED_FIELD] = 100.0;
    newLevelData[constants::MUSIC_FIELD] = "";
    newLevelData[constants::POWERUPS_FIELD] = nlohmann::json::array();
    newLevelData[constants::MAP_LENGTH_FIELD] = 0.0;
    newLevelData[constants::OBSTACLES_FIELD] = nlohmann::json::array();
    newLevelData[constants::WAVES_FIELD] = nlohmann::json::array();

    std::string newFileName = constants::LEVEL_FILE_PREFIX +
        std::to_string(nextIndex) + constants::LEVEL_FILE_EXTENSION;
    std::filesystem::path newPath = constants::LEVEL_DIRECTORY + "/" + newFileName;

    try {
        std::ofstream newFile(newPath);
        newFile << newLevelData.dump(4);
        newFile.close();
        return newPath;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

}  // namespace gsm
