/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ReplayState
*/

#include "ReplayState.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>  // NOLINT(build/c++17)
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
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
    _isPlaying(false),
    _isPaused(false),
    _shouldSwitch(false),
    _spacePressCooldown(0.0f),
    _playbackSpeed(1.0f),
    _renderOffsetX(0.0f),
    _renderOffsetY(0.0f) {
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

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
    _backButton->setText(constants::BACK_BUTTON_TEXT);
    _backButton->setSize(math::Vector2f(500.f, 70.f));
    _backButton->setPosition(math::Vector2f(650.0f, 1000.0f));
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

    createReplaySelectionUI();
}

void ReplayState::enter() {
    _uiManager->setOnBack([this]() {
        if (!_isPlaying) {
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePop();
            }
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(6487.0f, 3584.0f));
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

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);

    if (_shouldSwitch) {
        if (!_isPlaying) {
            _uiManager->clearElements();
            _replayButtons.clear();
            _isPlaying = true;
        } else {
            auto window = _resourceManager->get<gfx::IWindow>();
            if (window) {
                window->setViewCenter(
                    constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);
            }

            if (_playbackUIManager) {
                _playbackUIManager->clearElements();
                _playbackUIManager.reset();
            }
            _playPauseButton.reset();
            _replayBackButton.reset();
            _increaseSpeedButton.reset();
            _decreaseSpeedButton.reset();
            _progressSlider.reset();
            _timeText.reset();
            _speedText.reset();
            _playbackLayout.reset();

            _isPlaying = false;
            createReplaySelectionUI();
        }
        _shouldSwitch = false;
    }

    if (_isPlaying) {
        if (_spacePressCooldown > 0.0f) {
            _spacePressCooldown -= deltaTime;
        }

        auto event = _resourceManager->get<gfx::IEvent>();

        bool spacePressed = event->isKeyPressed(gfx::EventType::SPACE);
        bool gamepadAPressed = event->isKeyPressed(gfx::EventType::GAMEPAD_A);
        if ((spacePressed || gamepadAPressed) && _spacePressCooldown <= 0.0f) {
            _isPaused = !_isPaused;
            _spacePressCooldown = 0.25f;
        }

        bool gamepadBPressed = event->isKeyPressed(gfx::EventType::GAMEPAD_B);
        if (gamepadBPressed && _spacePressCooldown <= 0.0f) {
            _shouldSwitch = true;
            _isPaused = false;
            _spacePressCooldown = 0.25f;
        }

        bool leftPressed = event->isKeyPressed(gfx::EventType::LEFT) ||
                          event->isKeyPressed(gfx::EventType::GAMEPAD_DPAD_LEFT) ||
                          event->isKeyPressed(gfx::EventType::GAMEPAD_LEFT_STICK_LEFT);
        bool rightPressed = event->isKeyPressed(gfx::EventType::RIGHT) ||
                           event->isKeyPressed(gfx::EventType::GAMEPAD_DPAD_RIGHT) ||
                           event->isKeyPressed(gfx::EventType::GAMEPAD_LEFT_STICK_RIGHT);

        bool upPressed = event->isKeyPressed(gfx::EventType::UP) ||
                        event->isKeyPressed(gfx::EventType::GAMEPAD_DPAD_UP) ||
                        event->isKeyPressed(gfx::EventType::GAMEPAD_LEFT_STICK_UP);
        bool downPressed = event->isKeyPressed(gfx::EventType::DOWN) ||
                          event->isKeyPressed(gfx::EventType::GAMEPAD_DPAD_DOWN) ||
                          event->isKeyPressed(gfx::EventType::GAMEPAD_LEFT_STICK_DOWN);

        if (leftPressed) {
            _replayTime = (std::max)(0.0f, _replayTime - deltaTime * 5.0f);
        }
        if (rightPressed) {
            _replayTime = (std::min)(_totalReplayTime, _replayTime + deltaTime * 5.0f);
        }

        if (upPressed && _spacePressCooldown <= 0.0f) {
            _playbackSpeed = (std::min)(2.0f, _playbackSpeed + 0.1f);
            _spacePressCooldown = 0.15f;
        }
        if (downPressed && _spacePressCooldown <= 0.0f) {
            _playbackSpeed = (std::max)(0.1f, _playbackSpeed - 0.1f);
            _spacePressCooldown = 0.15f;
        }

        if (!_isPaused) {
            playReplay(deltaTime);
        }

        updatePlaybackControls();
    }

    renderUI();
}

void ReplayState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);

    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);
    _uiManager->clearElements();
    _backButton.reset();
    _background.reset();
    _mouseHandler.reset();
    _uiManager.reset();

    if (_playbackUIManager) {
        _playbackUIManager->clearElements();
        _playbackUIManager.reset();
    }
    _playPauseButton.reset();
    _replayBackButton.reset();
    _increaseSpeedButton.reset();
    _decreaseSpeedButton.reset();
    _progressSlider.reset();
    _timeText.reset();
    _speedText.reset();
    _playbackLayout.reset();
}

void ReplayState::renderUI() {
    if (!_isPlaying) {
        _background->render();
    }
    _uiManager->render();

    if (_isPlaying && !_frames.empty()) {
        renderReplaySprites();
        if (_playbackUIManager) {
            _playbackUIManager->render();
        }
    }
}

void ReplayState::playReplay(float deltaTime) {
    if (_frames.empty()) return;

    if (_currentFrameIndex >= _frames.size()) {
        _isPlaying = false;
        return;
    }

    _replayTime += deltaTime * _playbackSpeed;

    size_t left = 0;
    size_t right = _frames.size() - 1;
    size_t targetIndex = 0;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        float frameTime = _frames[mid][constants::REPLAY_TOTAL_TIME].get<float>();

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
        _shouldSwitch = true;
        _isPaused = false;
        _currentFrameIndex = _frames.size() - 1;
    }

    processAudioForFrame(_frames[_currentFrameIndex]);
}

void ReplayState::updateViewForFrame(const nlohmann::json& frame) {
    if (!frame.contains(constants::REPLAY_GAMEZONE)) {
        _renderOffsetX = 0.0f;
        _renderOffsetY = 0.0f;
        return;
    }

    const auto& gamezone = frame[constants::REPLAY_GAMEZONE];
    if (!gamezone.contains(constants::REPLAY_X) || !gamezone.contains(constants::REPLAY_Y) ||
        !gamezone.contains(constants::REPLAY_WIDTH) ||
        !gamezone.contains(constants::REPLAY_HEIGHT)) {
        _renderOffsetX = 0.0f;
        _renderOffsetY = 0.0f;
        return;
    }

    float gamezoneX = gamezone[constants::REPLAY_X];
    float gamezoneY = gamezone[constants::REPLAY_Y];
    float gamezoneWidth = gamezone[constants::REPLAY_WIDTH];
    float gamezoneHeight = gamezone[constants::REPLAY_HEIGHT];

    float centerX = gamezoneX + gamezoneWidth / 2.0f;
    float centerY = gamezoneY + gamezoneHeight / 2.0f;

    _renderOffsetX = constants::MAX_WIDTH / 2.0f - centerX;
    _renderOffsetY = constants::MAX_HEIGHT / 2.0f - centerY;
}

void ReplayState::renderReplaySprites() {
    if (_frames.empty() || _currentFrameIndex >= _frames.size()) return;

    const auto& frame = _frames[_currentFrameIndex];

    updateViewForFrame(frame);

    if (!frame.contains(constants::REPLAY_RENDERABLES)) return;

    auto window = _resourceManager->get<gfx::IWindow>();
    if (!window) return;

    for (const auto& renderable : frame[constants::REPLAY_RENDERABLES]) {
        if (renderable.contains(constants::REPLAY_TYPE) &&
            renderable[constants::REPLAY_TYPE] == constants::REPLAY_TYPE_PARALLAX &&
            renderable.contains(constants::REPLAY_TRANSFORM) &&
            renderable.contains(constants::REPLAY_PARALLAX)) {
            renderParallaxBackground(renderable, window);
        } else if (renderable.contains(constants::REPLAY_TYPE) &&
                renderable[constants::REPLAY_TYPE] == constants::REPLAY_TYPE_HEALTHBAR &&
                renderable.contains(constants::REPLAY_TRANSFORM) &&
                renderable.contains(constants::REPLAY_HEALTH) &&
                renderable.contains(constants::REPLAY_COLLIDER)) {
            renderHealthBar(renderable, window);
        } else if (renderable.contains(constants::REPLAY_TYPE) &&
                renderable[constants::REPLAY_TYPE] == constants::REPLAY_TYPE_TEXT &&
                renderable.contains(constants::REPLAY_TRANSFORM) &&
                renderable.contains(constants::REPLAY_TEXT)) {
            renderText(renderable, window);
        } else if (renderable.contains(constants::REPLAY_TYPE) &&
                renderable[constants::REPLAY_TYPE] == constants::REPLAY_TYPE_RECTANGLE &&
                renderable.contains(constants::REPLAY_TRANSFORM) &&
                renderable.contains(constants::REPLAY_RECTANGLE)) {
            renderRectangle(renderable, window);
        } else if (renderable.contains(constants::REPLAY_TYPE) &&
                renderable[constants::REPLAY_TYPE] == constants::REPLAY_TYPE_HITBOX &&
                renderable.contains(constants::REPLAY_TRANSFORM) &&
                renderable.contains(constants::REPLAY_HITBOX) &&
                renderable.contains(constants::REPLAY_COLLIDER)) {
            renderHitbox(renderable, window);
        }
    }

    for (const auto& renderable : frame[constants::REPLAY_RENDERABLES]) {
        if (renderable.contains(constants::REPLAY_TRANSFORM) &&
            renderable.contains(constants::REPLAY_SPRITE)) {
            const auto& transform = renderable[constants::REPLAY_TRANSFORM];
            const auto& sprite = renderable[constants::REPLAY_SPRITE];

            float x = transform[constants::REPLAY_X].get<float>() + _renderOffsetX;
            float y = transform[constants::REPLAY_Y].get<float>() + _renderOffsetY;
            float scaleX = transform[constants::REPLAY_SCALE_X];
            float scaleY = transform[constants::REPLAY_SCALE_Y];

            std::string textureName = sprite[constants::REPLAY_TEXTURE];
            std::string texturePath = textureName;

            float offsetX = sprite[constants::REPLAY_OFFSET_X];
            float offsetY = sprite[constants::REPLAY_OFFSET_Y];
            float width = sprite[constants::REPLAY_WIDTH];
            float height = sprite[constants::REPLAY_HEIGHT];

            if (width > 0 && height > 0) {
                math::FRect frameRect(offsetX, offsetY, width, height);
                window->drawSprite(texturePath, x, y, frameRect, scaleX, scaleY);
            } else {
                window->drawSprite(texturePath, x, y, scaleX, scaleY);
            }
        }
    }
}



void ReplayState::renderParallaxBackground(
    const nlohmann::json& parallaxData,
    std::shared_ptr<gfx::IWindow> window
) {
    if (!parallaxData.contains(constants::REPLAY_TRANSFORM) ||
        !parallaxData.contains(constants::REPLAY_PARALLAX)) return;

    const auto& transform = parallaxData[constants::REPLAY_TRANSFORM];
    const auto& parallax = parallaxData[constants::REPLAY_PARALLAX];

    float baseX = transform[constants::REPLAY_X].get<float>() + _renderOffsetX;
    float baseY = transform[constants::REPLAY_Y].get<float>() + _renderOffsetY;

    float screenWidth = constants::MAX_WIDTH;
    float screenHeight = constants::MAX_HEIGHT;

    float viewLeft = 0.0f;
    if (parallax.contains(constants::REPLAY_LAYERS)) {
        std::vector<std::pair<int, const nlohmann::json*>> sortedLayers;
        for (const auto& layerData : parallax[constants::REPLAY_LAYERS]) {
            int zIndex = layerData[constants::REPLAY_Z_INDEX];
            sortedLayers.emplace_back(zIndex, &layerData);
        }
        std::sort(sortedLayers.begin(), sortedLayers.end(),
            [](const std::pair<int, const nlohmann::json*>& a,
                const std::pair<int, const nlohmann::json*>& b) {
                return a.first < b.first;
            });

        for (const auto& [zIndex, layerDataPtr] : sortedLayers) {
            const auto& layerData = *layerDataPtr;
            std::string filePath = layerData[constants::REPLAY_FILE_PATH];
            math::Vector2f scale(layerData[constants::REPLAY_SCALE][constants::REPLAY_X],
                layerData[constants::REPLAY_SCALE][constants::REPLAY_Y]);
            int scaleModeInt = layerData[constants::REPLAY_SCALE_MODE];
            math::Vector2f sourceSize(layerData[constants::REPLAY_SOURCE_SIZE]
                [constants::REPLAY_X],
                layerData[constants::REPLAY_SOURCE_SIZE][constants::REPLAY_Y]);
            bool repeat = layerData[constants::REPLAY_REPEAT];
            math::Vector2f currentOffset(layerData[constants::REPLAY_CURRENT_OFFSET]
                [constants::REPLAY_X], layerData[constants::REPLAY_CURRENT_OFFSET]
                [constants::REPLAY_Y]);

            math::Vector2f finalScale = scale;
            if (scaleModeInt == 0) {
                float scaleX = screenWidth / sourceSize.getX();
                float scaleY = screenHeight / sourceSize.getY();
                float uniformScale = (std::max)(scaleX, scaleY);
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

                int tilesX = static_cast<int>(
                    std::ceil((screenWidth - startX) / textureWidth)) + 1;
                int tilesY = static_cast<int>(
                    std::ceil((screenHeight - startY) / textureHeight)) + 1;

                for (int y = 0; y < tilesY; ++y) {
                    for (int x = 0; x < tilesX; ++x) {
                        float drawX =
                            startX + (static_cast<float>(x) * textureWidth) + viewLeft;
                        float drawY =
                            startY + (static_cast<float>(y) * textureHeight);

                        window->drawSprite(
                            filePath, drawX, drawY, finalScale.getX(), finalScale.getY());
                    }
                }
            } else {
                float drawX = currentOffset.getX() + baseX;
                float drawY = currentOffset.getY() + baseY;

                window->drawSprite(
                    filePath, drawX, drawY, finalScale.getX(), finalScale.getY());
            }
        }
    }
}

void ReplayState::renderHealthBar(
    const nlohmann::json& healthBarData,
    std::shared_ptr<gfx::IWindow> window
) {
    if (!healthBarData.contains(constants::REPLAY_TRANSFORM) ||
        !healthBarData.contains(constants::REPLAY_HEALTH) ||
        !healthBarData.contains(constants::REPLAY_COLLIDER)) return;

    const auto& transform = healthBarData[constants::REPLAY_TRANSFORM];
    const auto& health = healthBarData[constants::REPLAY_HEALTH];
    const auto& collider = healthBarData[constants::REPLAY_COLLIDER];

    float posX = transform[constants::REPLAY_X].get<float>() + _renderOffsetX;
    float posY = transform[constants::REPLAY_Y].get<float>() + _renderOffsetY;
    float scaleX = transform[constants::REPLAY_SCALE_X];
    float scaleY = transform[constants::REPLAY_SCALE_Y];

    float currentHealth = health[constants::REPLAY_CURRENT];
    float maxHealth = health[constants::REPLAY_MAX];

    float offsetX = collider[constants::REPLAY_OFFSET_X];
    float offsetY = collider[constants::REPLAY_OFFSET_Y];
    float sizeX = collider[constants::REPLAY_SIZE_X];
    float sizeY = collider[constants::REPLAY_SIZE_Y];

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

void ReplayState::renderText(
    const nlohmann::json& textData,
    std::shared_ptr<gfx::IWindow> window
) {
    if (!textData.contains(constants::REPLAY_TRANSFORM) ||
        !textData.contains(constants::REPLAY_TEXT)) return;

    const auto& transform = textData[constants::REPLAY_TRANSFORM];
    const auto& text = textData[constants::REPLAY_TEXT];

    float x = transform[constants::REPLAY_X].get<float>() + _renderOffsetX;
    float y = transform[constants::REPLAY_Y].get<float>() + _renderOffsetY;

    std::string content = text[constants::REPLAY_CONTENT];
    std::string fontPath = text[constants::REPLAY_FONT_PATH];

    const auto& color = text[constants::REPLAY_COLOR];
    gfx::color_t textColor = {
        static_cast<uint8_t>(color[constants::REPLAY_RED]),
        static_cast<uint8_t>(color[constants::REPLAY_GREEN]),
        static_cast<uint8_t>(color[constants::REPLAY_BLUE]),
        static_cast<uint8_t>(color[constants::REPLAY_ALPHA])
    };

    window->drawText(
        content, textColor, {static_cast<size_t>(x), static_cast<size_t>(y)}, fontPath);
}

void ReplayState::renderRectangle(
    const nlohmann::json& rectangleData,
    std::shared_ptr<gfx::IWindow> window
) {
    if (!rectangleData.contains(constants::REPLAY_TRANSFORM) ||
        !rectangleData.contains(constants::REPLAY_RECTANGLE)) return;

    const auto& transform = rectangleData[constants::REPLAY_TRANSFORM];
    const auto& rectangle = rectangleData[constants::REPLAY_RECTANGLE];

    float x = transform[constants::REPLAY_X].get<float>() + _renderOffsetX;
    float y = transform[constants::REPLAY_Y].get<float>() + _renderOffsetY;

    float width = rectangle[constants::REPLAY_WIDTH];
    float height = rectangle[constants::REPLAY_HEIGHT];

    const auto& color = rectangle[constants::REPLAY_COLOR];
    gfx::color_t rectColor = {
        static_cast<uint8_t>(color[constants::REPLAY_RED]),
        static_cast<uint8_t>(color[constants::REPLAY_GREEN]),
        static_cast<uint8_t>(color[constants::REPLAY_BLUE]),
        static_cast<uint8_t>(color[constants::REPLAY_ALPHA])
    };

    window->drawFilledRectangle(rectColor,
        {static_cast<size_t>(x), static_cast<size_t>(y)},
        {static_cast<size_t>(width), static_cast<size_t>(height)});
}

void ReplayState::renderHitbox(
    const nlohmann::json& hitboxData,
    std::shared_ptr<gfx::IWindow> window
) {
    if (!hitboxData.contains(constants::REPLAY_TRANSFORM) ||
    !hitboxData.contains(constants::REPLAY_HITBOX) ||
    !hitboxData.contains(constants::REPLAY_COLLIDER)) return;

    const auto& transform = hitboxData[constants::REPLAY_TRANSFORM];
    const auto& hitbox = hitboxData[constants::REPLAY_HITBOX];
    const auto& collider = hitboxData[constants::REPLAY_COLLIDER];

    float posX = transform[constants::REPLAY_X].get<float>() + _renderOffsetX;
    float posY = transform[constants::REPLAY_Y].get<float>() + _renderOffsetY;
    float scaleX = transform[constants::REPLAY_SCALE_X];
    float scaleY = transform[constants::REPLAY_SCALE_Y];

    float offsetX = collider[constants::REPLAY_OFFSET_X];
    float offsetY = collider[constants::REPLAY_OFFSET_Y];
    float sizeX = collider[constants::REPLAY_SIZE_X];
    float sizeY = collider[constants::REPLAY_SIZE_Y];

    math::FRect hitboxRect = math::FRect(
        posX + offsetX,
        posY + offsetY,
        sizeX * scaleX,
        sizeY * scaleY
    );

    const auto& color = hitbox[constants::REPLAY_COLOR];
    gfx::color_t hitboxColor = {
        static_cast<uint8_t>(color[constants::REPLAY_RED]),
        static_cast<uint8_t>(color[constants::REPLAY_GREEN]),
        static_cast<uint8_t>(color[constants::REPLAY_BLUE]),
        static_cast<uint8_t>(color[constants::REPLAY_ALPHA])
    };

    window->drawRectangleOutline(hitboxColor,
        {static_cast<size_t>(
            hitboxRect.getLeft()), static_cast<size_t>(hitboxRect.getTop())},
        {static_cast<size_t>(
            hitboxRect.getWidth()), static_cast<size_t>(hitboxRect.getHeight())});
}

void ReplayState::processAudioForFrame(const nlohmann::json& frame) {
    if (!frame.contains(constants::REPLAY_AUDIO)) return;

    auto audio = _resourceManager->get<gfx::IAudio>();
    if (!audio) return;

    for (const auto& audioData : frame[constants::REPLAY_AUDIO]) {
        if (!audioData.contains(constants::REPLAY_TYPE)) continue;

        std::string type = audioData[constants::REPLAY_TYPE];

        if (type == constants::REPLAY_TYPE_SOUND) {
            if (audioData.contains(constants::REPLAY_SOUND_PATH) &&
                audioData.contains(constants::REPLAY_VOLUME)) {
                std::string soundPath = audioData[constants::REPLAY_SOUND_PATH];
                float volume = audioData[constants::REPLAY_VOLUME];

                if (_resourceManager->has<SettingsConfig>()) {
                    auto settings = _resourceManager->get<SettingsConfig>();
                    volume = (volume / 100.0f) * settings->getSoundVolume();
                }

                audio->playSound(soundPath, volume, _playbackSpeed);
            }
        }
    }
}

std::vector<std::filesystem::path> ReplayState::getAvailableReplays() {
    std::vector<std::filesystem::path> replays;
    std::filesystem::path replayDir = constants::REPLAY_DIRECTORY;

    if (std::filesystem::exists(replayDir) && std::filesystem::is_directory(replayDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(replayDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json" &&
                entry.path().stem().string().find(constants::REPLAY_FILE_PREFIX) == 0) {
                replays.push_back(entry.path());
            }
        }

        std::sort(replays.begin(), replays.end(),
            [](const std::filesystem::path& a, const std::filesystem::path& b) {
                return std::filesystem::last_write_time(a) >
                    std::filesystem::last_write_time(b);
            });
    }

    return replays;
}

void ReplayState::createReplaySelectionUI() {
    _uiManager->clearElements();
    _uiManager->addElement(_background);
    _replayButtons.clear();

    auto availableReplays = getAvailableReplays();

    ui::LayoutConfig layoutConfig;
    layoutConfig.direction = ui::LayoutDirection::Vertical;
    layoutConfig.alignment = ui::LayoutAlignment::Center;
    layoutConfig.spacing = 15.0f;
    layoutConfig.padding = math::Vector2f(20.0f, 20.0f);
    layoutConfig.anchorX = ui::AnchorX::Center;
    layoutConfig.anchorY = ui::AnchorY::Center;
    layoutConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto layout = std::make_shared<ui::UILayout>(_resourceManager, layoutConfig);
    layout->setSize(math::Vector2f(500.f, 400.f));

    auto titleText = std::make_shared<ui::Text>(_resourceManager);
    titleText->setText("Select Replay");
    titleText->setSize(math::Vector2f(300.f, 40.f));
    layout->addElement(titleText);

    if (availableReplays.empty()) {
        auto noReplaysText = std::make_shared<ui::Text>(_resourceManager);
        noReplaysText->setText("No replays available");
        noReplaysText->setSize(math::Vector2f(300.f, 30.f));
        layout->addElement(noReplaysText);
    } else {
        for (size_t i = 0; i < availableReplays.size() && i < 5; ++i) {
            const auto& replayPath = availableReplays[i];

            auto writeTime = std::filesystem::last_write_time(replayPath);
            auto timeT = std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::from_time_t(0) +
                std::chrono::duration_cast<std::chrono::system_clock::duration>(
                    writeTime - std::filesystem::file_time_type::clock::now() +
                    std::chrono::system_clock::now().time_since_epoch()
                )
            );

            std::stringstream ss;
            #ifdef _WIN32
                std::tm timeInfo;
                localtime_s(&timeInfo, &timeT);
                ss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M");
            #else
                ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M");
            #endif

            auto replayButton = std::make_shared<ui::Button>(_resourceManager);
            replayButton->setText(ss.str());
            replayButton->setSize(math::Vector2f(400.f, 40.f));
            replayButton->setNormalColor(colors::BUTTON_PRIMARY);
            replayButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
            replayButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);

            replayButton->setOnRelease([this, replayPath]() {
                _shouldSwitch = true;
                loadReplay(replayPath);
            });
            replayButton->setOnActivated([this, replayPath]() {
                _shouldSwitch = true;
                loadReplay(replayPath);
            });

            _replayButtons.push_back(replayButton);
            layout->addElement(replayButton);
        }
    }

    layout->addElement(_backButton);

    _uiManager->addElement(layout);
}

void ReplayState::loadReplay(const std::filesystem::path& replayFile) {
    std::ifstream file(replayFile);
    if (!file.is_open()) {
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
            if (frame.contains(constants::REPLAY_TOTAL_TIME)) {
                _frames.push_back(frame);
                frameCount++;
            }
        } catch (const std::exception& e) {
            std::cout << "Error parsing line: " << e.what() << std::endl;
        }
    }

    file.close();

    if (frameCount > 0) {
        _totalReplayTime = _frames.back()[constants::REPLAY_TOTAL_TIME].get<float>();
        _replayTime = 0.0f;
        _currentFrameIndex = 0;
        _isPaused = false;

        createPlaybackControlsUI();
    }
}

void ReplayState::createPlaybackControlsUI() {
    if (!_playbackUIManager) {
        _playbackUIManager = std::make_unique<ui::UIManager>();
        auto config = _resourceManager->get<SettingsConfig>();
        _playbackUIManager->setGlobalScale(config->getUIScale());
    }

    _playbackUIManager->clearElements();

    ui::LayoutConfig mainLayoutConfig;
    mainLayoutConfig.direction = ui::LayoutDirection::Vertical;
    mainLayoutConfig.alignment = ui::LayoutAlignment::Start;
    mainLayoutConfig.spacing = 70.0f;
    mainLayoutConfig.padding = math::Vector2f(20.0f, 20.0f);
    mainLayoutConfig.anchorX = ui::AnchorX::Left;
    mainLayoutConfig.anchorY = ui::AnchorY::Bottom;
    mainLayoutConfig.offset = math::Vector2f(20.0f, -50.0f);

    _playbackLayout = std::make_shared<ui::UILayout>(_resourceManager, mainLayoutConfig);
    _playbackLayout->setSize(math::Vector2f(900.f, 170.f));

    auto buttonsLayout = std::make_shared<ui::UILayout>(_resourceManager);
    buttonsLayout->setDirection(ui::LayoutDirection::Horizontal);
    buttonsLayout->setAlignment(ui::LayoutAlignment::Start);
    buttonsLayout->setSpacing(25.0f);
    buttonsLayout->setPadding(math::Vector2f(0.0f, 0.0f));

    _replayBackButton = std::make_shared<ui::Button>(_resourceManager);
    _replayBackButton->setText("Back");
    _replayBackButton->setSize(math::Vector2f(100.f, 50.f));
    _replayBackButton->setNormalColor(colors::BUTTON_SECONDARY);
    _replayBackButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _replayBackButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _replayBackButton->setOnRelease([this]() {
        _shouldSwitch = true;
        _isPaused = false;
    });
    _replayBackButton->setOnActivated([this]() {
        _shouldSwitch = true;
        _isPaused = false;
    });

    _playPauseButton = std::make_shared<ui::Button>(_resourceManager);
    _playPauseButton->setText("Pause");
    _playPauseButton->setSize(math::Vector2f(120.f, 50.f));
    _playPauseButton->setNormalColor(colors::BUTTON_PRIMARY);
    _playPauseButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _playPauseButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _playPauseButton->setOnRelease([this]() {
        _isPaused = !_isPaused;
    });
    _playPauseButton->setOnActivated([this]() {
        _isPaused = !_isPaused;
    });

    _decreaseSpeedButton = std::make_shared<ui::Button>(_resourceManager);
    _decreaseSpeedButton->setText("-");
    _decreaseSpeedButton->setSize(math::Vector2f(50.f, 50.f));
    _decreaseSpeedButton->setNormalColor(colors::BUTTON_PRIMARY);
    _decreaseSpeedButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _decreaseSpeedButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _decreaseSpeedButton->setOnRelease([this]() {
        _playbackSpeed = (std::max)(0.1f, _playbackSpeed - 0.1f);
    });
    _decreaseSpeedButton->setOnActivated([this]() {
        _playbackSpeed = (std::max)(0.1f, _playbackSpeed - 0.1f);
    });

    _increaseSpeedButton = std::make_shared<ui::Button>(_resourceManager);
    _increaseSpeedButton->setText("+");
    _increaseSpeedButton->setSize(math::Vector2f(50.f, 50.f));
    _increaseSpeedButton->setNormalColor(colors::BUTTON_PRIMARY);
    _increaseSpeedButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _increaseSpeedButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _increaseSpeedButton->setOnRelease([this]() {
        _playbackSpeed = (std::min)(2.0f, _playbackSpeed + 0.1f);
    });
    _increaseSpeedButton->setOnActivated([this]() {
        _playbackSpeed = (std::min)(2.0f, _playbackSpeed + 0.1f);
    });

    buttonsLayout->addElement(_replayBackButton);
    buttonsLayout->addElement(_playPauseButton);
    buttonsLayout->addElement(_decreaseSpeedButton);
    buttonsLayout->addElement(_increaseSpeedButton);

    auto progressLayout = std::make_shared<ui::UILayout>(_resourceManager);
    progressLayout->setDirection(ui::LayoutDirection::Vertical);
    progressLayout->setAlignment(ui::LayoutAlignment::Start);
    progressLayout->setSpacing(0.0f);
    progressLayout->setPadding(math::Vector2f(0.0f, 0.0f));

    _timeText = std::make_shared<ui::Text>(_resourceManager);
    _timeText->setText(" 0:00 / 0:00               Speed: 100%");
    _timeText->setSize(math::Vector2f(850.f, 15.f));

    _progressSlider = std::make_shared<ui::Slider>(_resourceManager);
    _progressSlider->setMinValue(0.0f);
    _progressSlider->setMaxValue(_totalReplayTime);
    _progressSlider->setValue(_replayTime);
    _progressSlider->setShowPercentage(false);
    _progressSlider->setSize(math::Vector2f(850.0f, 40.0f));
    _progressSlider->setStep(0.016f);
    _progressSlider->setOnValueChanged([this](float value) {
        _replayTime = value;

        size_t targetIndex = 0;
        for (size_t i = 0; i < _frames.size(); ++i) {
            float frameTime = _frames[i][constants::REPLAY_TOTAL_TIME].get<float>();
            if (frameTime <= _replayTime) {
                targetIndex = i;
            } else {
                break;
            }
        }
        _currentFrameIndex = targetIndex;
    });

    progressLayout->addElement(_timeText);
    progressLayout->addElement(_progressSlider);

    _playbackLayout->addElement(progressLayout);
    _playbackLayout->addElement(buttonsLayout);

    _playbackUIManager->addElement(_playbackLayout);
}

void ReplayState::updatePlaybackControls() {
    if (!_playbackUIManager || !_progressSlider || !_timeText) {
        return;
    }

    auto config = _resourceManager->get<SettingsConfig>();
    if (_playbackUIManager->getGlobalScale() != config->getUIScale()) {
        _playbackUIManager->setGlobalScale(config->getUIScale());
    }

    _progressSlider->setValue(_replayTime);

    int currentMinutes = static_cast<int>(_replayTime) / 60;
    int currentSeconds = static_cast<int>(_replayTime) % 60;
    int totalMinutes = static_cast<int>(_totalReplayTime) / 60;
    int totalSeconds = static_cast<int>(_totalReplayTime) % 60;

    std::string timeString = std::to_string(currentMinutes) + ":";
    if (currentSeconds < 10) timeString += "0";
    timeString += std::to_string(currentSeconds);
    timeString += " / " + std::to_string(totalMinutes) + ":";
    if (totalSeconds < 10) timeString += "0";
    timeString += std::to_string(totalSeconds);

    int speedPercent = static_cast<int>(std::round(_playbackSpeed * 100.0f));
    std::string speedString = "Speed: " + std::to_string(speedPercent) + "%";

    std::string paddedTimeString = " " + timeString;
    while (paddedTimeString.length() < 15) {
        paddedTimeString += " ";
    }
    _timeText->setText(paddedTimeString + speedString);

    if (_playPauseButton) {
        _playPauseButton->setText(_isPaused ? "Play" : "Pause");
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    _playbackUIManager->handleMouseInput(mousePos, mousePressed);
    _playbackUIManager->update(0.016f);
}

}  // namespace gsm
