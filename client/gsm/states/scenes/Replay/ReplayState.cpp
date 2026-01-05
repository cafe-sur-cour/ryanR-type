/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ReplayState
*/

#include "ReplayState.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cmath>
#include <algorithm>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/types/Vector2f.hpp"
#include "../../../../../common/types/FRect.hpp"

namespace gsm {

ReplayState::ReplayState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager),
    _replayTime(0.0f),
    _totalReplayTime(0.0f),
    _currentFrameIndex(0),
    _isPlaying(false) {
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

    _playButton = std::make_shared<ui::Button>(_resourceManager);
    _playButton->setText("Play Replay");
    _playButton->setSize(math::Vector2f(300.f, 50.f));
    _playButton->setOnRelease([this]() {
        loadReplay();
        _isPlaying = true;
    });

    _statusText = std::make_shared<ui::Text>(_resourceManager);
    _statusText->setText("Replay not loaded");
    _statusText->setSize(math::Vector2f(400.f, 50.f));

    ui::LayoutConfig layoutConfig;
    layoutConfig.direction = ui::LayoutDirection::Vertical;
    layoutConfig.alignment = ui::LayoutAlignment::Center;
    layoutConfig.spacing = 20.0f;
    layoutConfig.padding = math::Vector2f(0.0f, 0.0f);
    layoutConfig.anchorX = ui::AnchorX::Center;
    layoutConfig.anchorY = ui::AnchorY::Center;
    layoutConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto layout = std::make_shared<ui::UILayout>(_resourceManager, layoutConfig);
    layout->setSize(math::Vector2f(400.f, 200.f));
    layout->addElement(_playButton);
    layout->addElement(_statusText);
    layout->addElement(_backButton);

    _uiManager->addElement(layout);
}

void ReplayState::enter() {
    _uiManager->setOnBack([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
}

void ReplayState::update(float deltaTime) {
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

    if (_isPlaying) {
        playReplay(deltaTime);
    }

    renderUI();
}

void ReplayState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);

    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);
    _uiManager->clearElements();
    _backButton.reset();
    _playButton.reset();
    _statusText.reset();
    _background.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

void ReplayState::renderUI() {
    if (!_isPlaying) {
        _background->render();
    }
    _uiManager->render();

    if (_isPlaying && !_frames.empty()) {
        renderReplaySprites();
    }
}

void ReplayState::loadReplay() {
    std::ifstream file("replay.json");
    if (!file.is_open()) {
        _statusText->setText("Replay file not found");
        return;
    }

    std::string line;
    size_t frameCount = 0;
    _frames.clear();
    _totalReplayTime = 0.0f;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            nlohmann::json frame = nlohmann::json::parse(line);
            if (frame.contains("totalTime")) {
                _frames.push_back(frame);
                frameCount++;
            }
        } catch (const std::exception& e) {
            std::cout << "Error parsing line: " << e.what() << std::endl;
        }
    }

    file.close();

    if (frameCount > 0) {
        _totalReplayTime = _frames.back()["totalTime"].get<float>();
        _statusText->setText("Replay loaded: " + std::to_string(frameCount) + " frames (" + std::to_string(_totalReplayTime) + "s)");
        std::cout << "Replay loaded: " << frameCount << " frames (" << _totalReplayTime << "s)" << std::endl;
        _replayTime = 0.0f;
        _currentFrameIndex = 0;
    } else {
        _statusText->setText("No valid frames in replay file");
        std::cout << "No valid frames in replay file" << std::endl;
    }
}

void ReplayState::playReplay(float deltaTime) {
    if (_frames.empty()) return;

    if (_currentFrameIndex >= _frames.size()) {
        _isPlaying = false;
        _statusText->setText("Replay finished");
        return;
    }

    _replayTime += deltaTime;

    size_t left = 0;
    size_t right = _frames.size() - 1;
    size_t targetIndex = 0;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        float frameTime = _frames[mid]["totalTime"].get<float>();

        if (frameTime <= _replayTime) {
            targetIndex = mid;
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }

    _currentFrameIndex = targetIndex;

    if (_replayTime >= _totalReplayTime) {
        _isPlaying = false;
        _statusText->setText("Replay finished");
        _currentFrameIndex = _frames.size() - 1;

        auto window = _resourceManager->get<gfx::IWindow>();
        if (window) {
            window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);
        }
    } else {
        _statusText->setText("Playing replay: " + std::to_string(_replayTime) + "s / " + std::to_string(_totalReplayTime) + "s");
    }

    processAudioForFrame(_frames[_currentFrameIndex]);
}

void ReplayState::updateViewForFrame(const nlohmann::json& frame) {
    if (!frame.contains("gamezone")) {
        return;
    }

    const auto& gamezone = frame["gamezone"];
    if (!gamezone.contains("x") || !gamezone.contains("y") || !gamezone.contains("width") || !gamezone.contains("height")) {
        return;
    }

    float gamezoneX = gamezone["x"];
    float gamezoneY = gamezone["y"];
    float gamezoneWidth = gamezone["width"];
    float gamezoneHeight = gamezone["height"];

    float centerX = gamezoneX + gamezoneWidth / 2.0f;
    float centerY = gamezoneY + gamezoneHeight / 2.0f;

    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setViewCenter(centerX, centerY);
    }
}

void ReplayState::renderReplaySprites() {
    if (_frames.empty() || _currentFrameIndex >= _frames.size()) return;

    const auto& frame = _frames[_currentFrameIndex];

    updateViewForFrame(frame);

    if (!frame.contains("renderables")) return;

    auto window = _resourceManager->get<gfx::IWindow>();
    if (!window) return;

    for (const auto& renderable : frame["renderables"]) {
        if (renderable.contains("type") && renderable["type"] == "parallax" &&
            renderable.contains("transform") && renderable.contains("parallax")) {
            renderParallaxBackground(renderable, window);
        }
        else if (renderable.contains("type") && renderable["type"] == "healthbar" &&
                 renderable.contains("transform") && renderable.contains("health") && renderable.contains("collider")) {
            renderHealthBar(renderable, window);
        }
        else if (renderable.contains("type") && renderable["type"] == "text" &&
                 renderable.contains("transform") && renderable.contains("text")) {
            renderText(renderable, window);
        }
        else if (renderable.contains("type") && renderable["type"] == "rectangle" &&
                 renderable.contains("transform") && renderable.contains("rectangle")) {
            renderRectangle(renderable, window);
        }
        else if (renderable.contains("type") && renderable["type"] == "hitbox" &&
                 renderable.contains("transform") && renderable.contains("hitbox") && renderable.contains("collider")) {
            renderHitbox(renderable, window);
        }
    }

    for (const auto& renderable : frame["renderables"]) {
        if (renderable.contains("transform") && renderable.contains("sprite")) {
            const auto& transform = renderable["transform"];
            const auto& sprite = renderable["sprite"];

            float x = transform["x"];
            float y = transform["y"];
            float scaleX = transform["sx"];
            float scaleY = transform["sy"];

            std::string textureName = sprite["texture"];
            std::string texturePath = textureName;

            float offsetX = sprite["offsetX"];
            float offsetY = sprite["offsetY"];
            float width = sprite["width"];
            float height = sprite["height"];

            if (width > 0 && height > 0) {
                math::FRect frameRect(offsetX, offsetY, width, height);
                window->drawSprite(texturePath, x, y, frameRect, scaleX, scaleY);
            } else {
                window->drawSprite(texturePath, x, y, scaleX, scaleY);
            }
        }
    }
}



void ReplayState::renderParallaxBackground(const nlohmann::json& parallaxData, std::shared_ptr<gfx::IWindow> window) {
    if (!parallaxData.contains("transform") || !parallaxData.contains("parallax")) return;

    const auto& transform = parallaxData["transform"];
    const auto& parallax = parallaxData["parallax"];

    float baseX = transform["x"];
    float baseY = transform["y"];

    float screenWidth = constants::MAX_WIDTH;
    float screenHeight = constants::MAX_HEIGHT;

    math::Vector2f viewCenter = window->getViewCenter();
    float viewLeft = viewCenter.getX() - screenWidth / 2.0f;

    if (parallax.contains("layers")) {
        std::vector<std::pair<int, const nlohmann::json*>> sortedLayers;
        for (const auto& layerData : parallax["layers"]) {
            int zIndex = layerData["zIndex"];
            sortedLayers.emplace_back(zIndex, &layerData);
        }
        std::sort(sortedLayers.begin(), sortedLayers.end(),
            [](const std::pair<int, const nlohmann::json*>& a, const std::pair<int, const nlohmann::json*>& b) {
                return a.first < b.first;
            });

        for (const auto& [zIndex, layerDataPtr] : sortedLayers) {
            const auto& layerData = *layerDataPtr;
            std::string filePath = layerData["filePath"];
            math::Vector2f scale(layerData["scale"]["x"], layerData["scale"]["y"]);
            int scaleModeInt = layerData["scaleMode"];
            math::Vector2f sourceSize(layerData["sourceSize"]["x"], layerData["sourceSize"]["y"]);
            bool repeat = layerData["repeat"];
            math::Vector2f currentOffset(layerData["currentOffset"]["x"], layerData["currentOffset"]["y"]);

            math::Vector2f finalScale = scale;
            if (scaleModeInt == 0) {
                float scaleX = screenWidth / sourceSize.getX();
                float scaleY = screenHeight / sourceSize.getY();
                float uniformScale = std::max(scaleX, scaleY);
                finalScale = math::Vector2f(uniformScale, uniformScale);
            } else if (scaleModeInt == 1) {
                float scaleX = screenWidth / sourceSize.getX();
                float scaleY = screenHeight / sourceSize.getY();
                finalScale = math::Vector2f(scaleX, scaleY);
            }

            float textureWidth = sourceSize.getX() * finalScale.getX();
            float textureHeight = sourceSize.getY() * finalScale.getY();

            if (repeat) {
                float startX = currentOffset.getX() + baseX - viewLeft;
                float startY = currentOffset.getY() + baseY;

                startX = std::fmod(startX, textureWidth);
                if (startX > 0) startX -= textureWidth;

                startY = std::fmod(startY, textureHeight);
                if (startY > 0) startY -= textureHeight;

                int tilesX = static_cast<int>(std::ceil((screenWidth - startX) / textureWidth)) + 1;
                int tilesY = static_cast<int>(std::ceil((screenHeight - startY) / textureHeight)) + 1;

                for (int y = 0; y < tilesY; ++y) {
                    for (int x = 0; x < tilesX; ++x) {
                        float drawX = startX + (static_cast<float>(x) * textureWidth) + viewLeft;
                        float drawY = startY + (static_cast<float>(y) * textureHeight);

                        window->drawSprite(filePath, drawX, drawY, finalScale.getX(), finalScale.getY());
                    }
                }
            } else {
                float drawX = currentOffset.getX() + baseX;
                float drawY = currentOffset.getY() + baseY;

                window->drawSprite(filePath, drawX, drawY, finalScale.getX(), finalScale.getY());
            }
        }
    }
}

void ReplayState::renderHealthBar(const nlohmann::json& healthBarData, std::shared_ptr<gfx::IWindow> window) {
    if (!healthBarData.contains("transform") || !healthBarData.contains("health") || !healthBarData.contains("collider")) return;

    const auto& transform = healthBarData["transform"];
    const auto& health = healthBarData["health"];
    const auto& collider = healthBarData["collider"];

    float posX = transform["x"];
    float posY = transform["y"];
    float scaleX = transform["sx"];
    float scaleY = transform["sy"];

    float currentHealth = health["current"];
    float maxHealth = health["max"];

    float offsetX = collider["offsetX"];
    float offsetY = collider["offsetY"];
    float sizeX = collider["sizeX"];
    float sizeY = collider["sizeY"];

    math::FRect hitbox = math::FRect(
        posX + offsetX,
        posY + offsetY,
        sizeX * scaleX,
        sizeY * scaleY
    );

    float barX = hitbox.getLeft();
    float barY = hitbox.getTop() + constants::HEALTH_BAR_OFFSET_Y;
    float barWidth = hitbox.getWidth();
    float barHeight = constants::HEALTH_BAR_HEIGHT;

    window->drawFilledRectangle({0, 0, 0, 255},
        {static_cast<size_t>(barX - constants::HEALTH_BAR_OUTLINE_THICKNESS),
            static_cast<size_t>(barY - constants::HEALTH_BAR_OUTLINE_THICKNESS)},
        {static_cast<size_t>(barWidth + 2 * constants::HEALTH_BAR_OUTLINE_THICKNESS),
            static_cast<size_t>(barHeight + 2 * constants::HEALTH_BAR_OUTLINE_THICKNESS)});

    float healthRatio = currentHealth / maxHealth;
    if (healthRatio < 0.0f) healthRatio = 0.0f;
    if (healthRatio > 1.0f) healthRatio = 1.0f;

    uint8_t red = static_cast<uint8_t>((1.0f - healthRatio) * 255);
    uint8_t green = static_cast<uint8_t>(healthRatio * 255);
    uint8_t blue = 0;

    float fillWidth = barWidth * healthRatio;
    window->drawFilledRectangle({red, green, blue, 255},
        {static_cast<size_t>(barX), static_cast<size_t>(barY)},
        {static_cast<size_t>(fillWidth), static_cast<size_t>(barHeight)});
}

void ReplayState::renderText(const nlohmann::json& textData, std::shared_ptr<gfx::IWindow> window) {
    if (!textData.contains("transform") || !textData.contains("text")) return;

    const auto& transform = textData["transform"];
    const auto& text = textData["text"];

    float x = transform["x"];
    float y = transform["y"];

    std::string content = text["content"];
    std::string fontPath = text["fontPath"];

    const auto& color = text["color"];
    gfx::color_t textColor = {
        static_cast<uint8_t>(color["r"]),
        static_cast<uint8_t>(color["g"]),
        static_cast<uint8_t>(color["b"]),
        static_cast<uint8_t>(color["a"])
    };

    window->drawText(content, textColor, {static_cast<size_t>(x), static_cast<size_t>(y)}, fontPath);
}

void ReplayState::renderRectangle(const nlohmann::json& rectangleData, std::shared_ptr<gfx::IWindow> window) {
    if (!rectangleData.contains("transform") || !rectangleData.contains("rectangle")) return;

    const auto& transform = rectangleData["transform"];
    const auto& rectangle = rectangleData["rectangle"];

    float x = transform["x"];
    float y = transform["y"];

    float width = rectangle["width"];
    float height = rectangle["height"];

    const auto& color = rectangle["color"];
    gfx::color_t rectColor = {
        static_cast<uint8_t>(color["r"]),
        static_cast<uint8_t>(color["g"]),
        static_cast<uint8_t>(color["b"]),
        static_cast<uint8_t>(color["a"])
    };

    window->drawFilledRectangle(rectColor,
        {static_cast<size_t>(x), static_cast<size_t>(y)},
        {static_cast<size_t>(width), static_cast<size_t>(height)});
}

void ReplayState::renderHitbox(const nlohmann::json& hitboxData, std::shared_ptr<gfx::IWindow> window) {
    if (!hitboxData.contains("transform") || !hitboxData.contains("hitbox") || !hitboxData.contains("collider")) return;

    const auto& transform = hitboxData["transform"];
    const auto& hitbox = hitboxData["hitbox"];
    const auto& collider = hitboxData["collider"];

    float posX = transform["x"];
    float posY = transform["y"];
    float scaleX = transform["sx"];
    float scaleY = transform["sy"];

    float offsetX = collider["offsetX"];
    float offsetY = collider["offsetY"];
    float sizeX = collider["sizeX"];
    float sizeY = collider["sizeY"];

    math::FRect hitboxRect = math::FRect(
        posX + offsetX,
        posY + offsetY,
        sizeX * scaleX,
        sizeY * scaleY
    );

    const auto& color = hitbox["color"];
    gfx::color_t hitboxColor = {
        static_cast<uint8_t>(color["r"]),
        static_cast<uint8_t>(color["g"]),
        static_cast<uint8_t>(color["b"]),
        static_cast<uint8_t>(color["a"])
    };

    window->drawRectangleOutline(hitboxColor,
        {static_cast<size_t>(hitboxRect.getLeft()), static_cast<size_t>(hitboxRect.getTop())},
        {static_cast<size_t>(hitboxRect.getWidth()), static_cast<size_t>(hitboxRect.getHeight())});
}

void ReplayState::processAudioForFrame(const nlohmann::json& frame) {
    if (!frame.contains("audio")) return;

    auto audio = _resourceManager->get<gfx::IAudio>();
    if (!audio) return;

    for (const auto& audioData : frame["audio"]) {
        if (!audioData.contains("type")) continue;

        std::string type = audioData["type"];

        if (type == "sound") {
            if (audioData.contains("soundPath") && audioData.contains("volume")) {
                std::string soundPath = audioData["soundPath"];
                float volume = audioData["volume"];

                if (_resourceManager->has<SettingsConfig>()) {
                    auto settings = _resourceManager->get<SettingsConfig>();
                    volume = (volume / 100.0f) * settings->getSoundVolume();
                }

                audio->playSound(soundPath, volume);
            }
        }
    }
}

}  // namespace gsm
