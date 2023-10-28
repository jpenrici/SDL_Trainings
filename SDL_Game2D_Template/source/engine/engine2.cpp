#include "engine2.h"
#include "common/smallToolBox/src/smalltoolbox_text.hpp"

Engine2::Engine2(int screenWidth, int screenHeight, const std::string &title)
    : Engine(screenWidth, screenHeight, title) {}

auto Engine2::loadSpriteSheet(const std::string &id, int rows, int  columns, const std::string &filename) -> bool
{
    if (id.empty() || filename.empty()) {
        inform("Empty Image ID or image file path!");
        return false;
    }

    if (m_spriteSheets.contains(id)) {
        inform("Image ID: " + id + ", already exists!");
        return false;
    }

    if (!loadTexture(id, rows, columns, filename)) {
        return false;
    }

    auto size = textureSize(id);
    auto height = size.back();
    auto width = size.front();
    int spriteHeight = rows > 0 ? height / rows : height;
    int spriteWidth = columns > 0 ? width / columns : width;

    SpriteSheet spriteSheet;
    spriteSheet.id = id;
    spriteSheet.filename = filename;
    spriteSheet.rows = rows;
    spriteSheet.columns = columns;
    spriteSheet.spriteWidth = spriteWidth;
    spriteSheet.spriteHeight = spriteHeight;
    spriteSheet.count = rows * columns;

    m_spriteSheets[id] = spriteSheet;
    inform(stbox::Text::toStr("Added Image: Sprite Sheet [ ", id, ", Rows [", rows, "], Columns [", columns, "], ",
                              spriteSheet.count, " Sprites [", spriteWidth, " x ", spriteHeight, "], Texture [",
                              width, " x ", height, "], ", filename, "]"));

    return true;
}

auto Engine2::loadSpriteSheet(std::string id, const std::string &filename) -> bool
{
    return loadSpriteSheet(id, 1, 1, filename);
}

auto Engine2::loadSprite(std::string id, const std::string &filename) -> bool
{
    if (loadSpriteSheet(id, 1, 1, filename)) {
        auto result = spriteSheet(id);
        if (std::get<bool>(result)) {
            auto spriteSheet = std::get<SpriteSheet>(result);
            auto height = static_cast<float>(spriteSheet.spriteHeight);
            auto width = static_cast<float>(spriteSheet.spriteWidth);
            return newSprite(id, spriteSheet.id, 0, 0, width, height);
        }
    }

    return false;
}

auto Engine2::newAnimation(std::string id, std::string spriteId, std::vector<int> spriteNumbers,
                           int timeInterval) -> bool
{
    if (!m_sprites.contains(spriteId)) {
        if (!m_quiet) {
            inform("Sprite ID: " + spriteId + ", not found! Cannot sequence animation with sprites!");
        }
        return false;
    }

    if (m_sprites[spriteId].animations.contains(id)) {
        if (!m_quiet) {
            inform("Animation ID: " + id + ", already exists!");
        }
        return false;
    }

    if (spriteNumbers.empty()) {
        if (!m_quiet) {
            inform("Animation ID: " + id + ", list of sprite numbers empty!");
        }
        return false;
    }

    Animation animation;
    animation.id = id;
    animation.speed = timeInterval;
    animation.loop = true;
    animation.sequence = spriteNumbers;

    m_sprites[spriteId].animations[id] = animation;
    if (!m_quiet) {
        inform("Added: Animation [" + id + "], Sprite [" + spriteId + "] : " + std::to_string(spriteNumbers.size()) +
               " selected sprites.");
    }

    return true;
}

auto Engine2::newSprite(Sprite sprite) -> bool
{
    return newSprite(sprite.id, sprite.spriteSheetId,
                     sprite.position.X.toFloat(), sprite.position.Y.toFloat(),
                     sprite.width, sprite.height);
}

auto Engine2::newSprite(std::string id, std::string spriteSheetID, float x, float y, float width,
                        float height) -> bool
{
    if (m_sprites.contains(id)) {
        inform("Sprite ID: " + id + ", already exists!");
        return false;
    }

    if (!m_spriteSheets.contains(spriteSheetID)) {
        inform("Sprite ID: " + id + ", sprite sheet " + spriteSheetID + ", not found!");
    }

    Animation animation;
    animation.id = id + "-all";
    for (int i = 0; i < m_spriteSheets[spriteSheetID].count; ++i) {
        animation.sequence.emplace_back(i);
    }

    Sprite sprite;
    sprite.id = id;
    sprite.spriteSheetId = spriteSheetID;
    sprite.angle = 0;
    sprite.height = height;
    sprite.layer = 0;
    sprite.opacity = 255;
    sprite.position = {x, y};
    sprite.scale = 1;
    sprite.solid = true;
    sprite.width = width;
    sprite.animation = animation;
    sprite.animations[animation.id] = animation;
    sprite.activity = State::Activity::activated;
    m_sprites[id] = sprite;
    if (!m_quiet) {
        inform("Added: Sprite [" + sprite.id + ", Sprite Sheet [" + spriteSheetID + "], Animation [" +
               sprite.animation.id + ", " + std::to_string(animation.sequence.size()) + " sprites]");
    }

    return true;
}

auto Engine2::newSprite(std::string id, std::string spriteSheetId, float x, float y) -> bool
{
    auto result1 = spriteSheet(spriteSheetId);
    if (std::get<bool>(result1)) {
        auto spriteSheet = std::get<SpriteSheet>(result1);
        auto height = static_cast<float>(spriteSheet.spriteHeight);
        auto width = static_cast<float>(spriteSheet.spriteWidth);
        auto result2 = newSprite(id, spriteSheetId, x, y, width, height);
        return result2;
    }

    return false;
}

auto Engine2::newSprite(std::string id, std::string spriteSheetId, BoxCollider box) -> bool
{
    return newSprite(id, spriteSheetId, box.origin.X.toFloat(), box.origin.Y.toFloat(),
                     box.width<float>(), box.height<float>());
}

auto Engine2::newSprite(std::string id, std::string spriteSheetId) -> bool
{
    return newSprite(id, spriteSheetId, 0, 0);
}

auto Engine2::newText(const std::string &id, const std::string &fontId, const std::string &text, Position position,
                      RGBA foreground, int size) -> bool
{
    if (m_texts.contains(id)) {
        inform("Text ID: " + id + ", already exists!");
        return false;
    }

    if (!textFontExists(fontId)) {
        inform("Text Font ID: " + fontId + ", not found!");
        return false;
    }

    if (size < 0 || size > windowHeight()) {
        inform("Text ID: " + id + ", size out of limit!");
        return false;
    }

    Text newText;
    newText.id = id;
    newText.fontId = fontId;
    newText.text = text;
    newText.position = position;
    newText.foreground = foreground;
    newText.fontSize = size;
    newText.activity =  State::Activity::activated;

    m_texts[id] = newText;
    inform("Added: Text [" + id + "], Font [" + fontId + "], '" + text + "'");

    return true;
}

auto Engine2::setAnimation(std::string id, std::string spriteId) -> bool
{
    if (!m_sprites.contains(spriteId)) {
        if (!m_quiet) {
            inform("Sprite ID: " + spriteId + ", not found! Failed to configure animation!");
        }
        return false;
    }

    if (!m_sprites[spriteId].animations.contains(id)) {
        if (!m_quiet) {
            inform("Animation ID: " + id + ", not found! Failed to configure animation!");
        }
        return false;
    }

    m_sprites[spriteId].animation = m_sprites[spriteId].animations[id];
    if (!m_quiet) {
        inform("Sprite [" + spriteId + "]: Animation [" + id + "], defined!");
    }

    return true;
}

auto Engine2::setAnimationSpeed(std::string id, int speed) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Animation ID: " + id +
                   ", not found! It was not possible to change the time between one sprite and another!");
        }
        return false;
    }

    m_sprites[id].animation.speed = speed;

    return true;
}

auto Engine2::setSpriteActivity(std::string id, State::Activity activity) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Could not change the state (activity) of the Sprite!");
        }
        return false;
    }

    m_sprites[id].activity = activity;

    return true;
}

auto Engine2::setSpriteAngle(std::string id, double angle) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change angle!");
        }
        return false;
    }

    m_sprites[id].angle = angle;

    return true;
}

auto Engine2::setSpriteBoxCollider(std::string id, BoxCollider box) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite box!");
        }
        return false;
    }

    m_sprites[id].position = box.origin;
    m_sprites[id].width = box.width<float>();
    m_sprites[id].height = box.height<float>();

    return false;
}

auto Engine2::setSpriteBoxCollider(std::string id, float width, float height) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite box!");
        }
        return false;
    }

    m_sprites[id].width = width;
    m_sprites[id].height = height;

    return false;
}

auto Engine2::setSpriteEmptyBoxCollider(std::string id, Position position) -> bool
{
    return setSpriteBoxCollider(id, {position, 0, 0});
}

auto Engine2::setSpriteEmptyBoxCollider(std::string id) -> bool
{
    return setSpriteBoxCollider(id, 0, 0);
}

auto Engine2::setSpriteByIndex(std::string id, int index) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite!");
        }
        return false;
    }

    auto limit = m_sprites[id].animation.sequence.size() - 1;
    if (index < 0 || index > limit) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", index outside range [0 - " + std::to_string(limit) +
                   "]! Unable to change sprite!");
        }
        return false;
    }

    m_sprites[id].animation.frame = index;
    if (!m_quiet) {
        inform("Sprite [" + id + "]: Animation [" + m_sprites[id].animation.id + "] sprite [" +
               std::to_string(index) + ")]!");
    }

    return true;
}

auto Engine2::setSpriteLayer(std::string id, int layer) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change layer!");
        }
        return false;
    }

    m_sprites[id].layer = layer;
    m_layers.emplace(layer);

    return true;
}

auto Engine2::setSpriteScale(std::string id, float scale) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite scale!");
        }
        return false;
    }

    m_sprites[id].scale = scale;

    return true;
}

auto Engine2::setSpriteSolid(std::string id, bool isSolid) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite to solid!");
        }
        return false;
    }

    m_sprites[id].solid = isSolid;

    return true;
}

auto Engine2::setSpriteOpacity(std::string id, Uint8 opacityPercentage) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite opacity!");
        }
        return false;
    }

    m_sprites[id].opacity = opacityPercentage < 0 ? 0 : (opacityPercentage % 101) * 255.0 / 100.0;

    return true;
}

auto Engine2::setSpritePosition(std::string id, Position position) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to change sprite position!");
        }
        return false;
    }

    m_sprites[id].position = position;

    return true;
}

auto Engine2::setText(std::string id, std::string text) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change text!");
        }
        return false;
    }

    m_texts[id].text = text;

    return true;
}

auto Engine2::setTextActivity(std::string id, State::Activity activity) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Could not change the state (activity) of the text!");
        }
        return false;
    }

    m_texts[id].activity = activity;

    return true;
}

auto Engine2::setTextAngle(std::string id, int angle) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change angle!");
        }
        return false;
    }

    m_texts[id].angle = angle;

    return true;
}

auto Engine2::setTextColor(std::string id, RGBA foreground) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change text color!");
        }
        return false;
    }

    m_texts[id].foreground = foreground;

    return true;
}

auto Engine2::setTextFont(std::string id, std::string fontId) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change text!");
        }
        return false;
    }

    if (!textFontExists(fontId)) {
        if (!m_quiet) {
            inform("Text ID: " + id + " font ID " + fontId + ", not found!");
        }
        return false;
    }

    m_texts[id].fontId = fontId;

    return true;
}

auto Engine2::setTextLayer(std::string id, int layer) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change layer!");
        }
        return false;
    }

    m_texts[id].layer = layer;
    m_layers.emplace(layer);

    return true;
}

auto Engine2::setTextScale(std::string id, float scale) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change text scale!");
        }
        return false;
    }

    m_texts[id].scale = scale;

    return true;
}

auto Engine2::setTextOpacity(std::string id, Uint8 opacityPercentage) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change text opacity!");
        }
        return false;
    }

    m_texts[id].opacity = opacityPercentage < 0 ? 0 : (opacityPercentage % 101) * 255 / 100;

    return true;
}

auto Engine2::setTextPosition(std::string id, Position position) -> bool
{
    if (!m_texts.contains(id)) {
        if (!m_quiet) {
            inform("Text ID: " + id + ", not found! Unable to change text position!");
        }
        return false;
    }

    m_texts[id].position = position;

    return true;
}

auto Engine2::setTextSize(std::string textId, int size) -> bool
{
    if (!m_texts.contains(textId)) {
        if (!m_quiet) {
            inform("Text ID: " + textId + ", not found! Unable to change text font size!");
        }
        return false;
    }

    if (size < 0 || size > windowHeight()) {
        if (!m_quiet) {
            inform("Text ID: " + textId + ", size out of limit!");
        }
        return false;
    }

    m_texts[textId].fontSize = size;

    return true;
}

auto Engine2::sprite() -> Sprite
{
    return {};
}

auto Engine2::sprite(std::string id) -> std::tuple<Sprite, bool>
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found!");
        }
        return {Sprite(), false};
    }

    return {m_sprites[id], true};
}

auto Engine2::sprites() -> std::map<std::string, Sprite>
{
    return m_sprites;
}

auto Engine2::totalSprites() -> size_t
{
    return m_sprites.size();
}

auto Engine2::eraseSprite(std::string id) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to delete!");
        }
        return false;
    }

    m_sprites.erase(id);
    if (!m_quiet) {
        inform("Sprite ID: " + id + " erased! Sprites [" + std::to_string(m_sprites.size()) + "].");
    }

    return true;
}

auto Engine2::swapSprite(std::string id, Sprite sprite) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to swap!");
        }
        return false;
    }

    m_sprites[id] = sprite;
    if (!m_quiet) {
        inform("Sprite ID: " + id + " changed!");
    }

    return true;
}

auto Engine2::Sprite::boxCollider() -> BoxCollider
{
    return {position, width, height};
}

auto Engine2::Sprite::center() -> Position
{
    return {position.X.value +  width / 2, position.Y.value + height / 2};
}

auto Engine2::Sprite::getAnimation() -> Animation
{
    return animation;
}

void Engine2::Sprite::move(Position distance, stbox::Math::Rectangle limit)
{
    auto x = position.X.value + distance.X.value;
    auto y = position.Y.value + distance.Y.value;
    position.X.value += (x < limit.origin.X.value || x > (limit.width() - width)) ? 0 : distance.X.value;
    position.Y.value += (y < limit.origin.Y.value || y > (limit.height() - height)) ? 0 : distance.Y.value;
}

void Engine2::Sprite::move(Position distance)
{
    position += distance;
}

void Engine2::Sprite::moveHorizontal(double distance)
{
    position.X.value += distance;
}

void Engine2::Sprite::moveVertical(double distance)
{
    position.Y.value += distance;
}

void Engine2::Sprite::moveTo(Position newPosition, double limit, bool isGreater, bool vertically)
{
    if (vertically && isGreater && position.Y.value >= limit) {
        position = newPosition;
    }
    else if (vertically && !isGreater && position.Y.value <= limit) {
        position = newPosition;
    }
    else if (!vertically && isGreater && position.X.value >= limit) {
        position = newPosition;
    }
    else if (!vertically && !isGreater && position.X.value <= limit) {
        position = newPosition;
    }
}

void Engine2::Sprite::move(double distance, double angle)
{
    position = position.position(angle, distance);
}

void Engine2::Sprite::move4directions(double distance, stbox::Math::Rectangle limit, State::Input state)
{
    switch (state) {
    case State::Input::keyboard_up:
        move({0, -distance}, limit);
        break;
    case State::Input::keyboard_down:
        move({0, distance}, limit);
        break;
    case State::Input::keyboard_right:
        move({distance, 0}, limit);
        break;
    case State::Input::keyboard_left:
        move({-distance, 0}, limit);
        break;
    default:
        break;
    }
}

void Engine2::Sprite::move8directions(double distance, stbox::Math::Rectangle limit, State::Input state)
{
    move4directions(distance, limit, state);

    switch (state) {
    case State::Input::keyboard_up_rigth:
        move({distance, -distance}, limit);
        break;
    case State::Input::keyboard_up_left:
        move({-distance, -distance}, limit);
        break;
    case State::Input::keyboard_down_right:
        move({distance, distance}, limit);
        break;
    case State::Input::keyboard_down_left:
        move({-distance, distance}, limit);
        break;
    default:
        break;
    }
}

void Engine2::Sprite::rotate(double rotationAngle)
{
    setAngle(angle + rotationAngle);
}

auto Engine2::Sprite::size() -> stbox::Math::Size<float>
{
    return {width, height};
}

void Engine2::Sprite::setAngle(double newAngle)
{
    newAngle = newAngle > 360 ? 0 : newAngle;
    angle = newAngle;
}

void Engine2::Sprite::setColor(RGBA color)
{
    rgba = color;
    opacity = static_cast<float>(color.A % 256);
}

void Engine2::Sprite::setOpacity(Uint8 opacityPercentage)
{
    opacity = opacityPercentage < 0 ? 0 : (opacityPercentage % 101) * 255 / 100;
}

void Engine2::Sprite::setScale(float newScale)
{
    scale = newScale;
}

void Engine2::Sprite::setSolid(bool isSolid)
{
    solid = isSolid;
}

auto Engine2::Sprite::setAnimation(std::string animationId) -> bool
{
    if (animations.contains(animationId)) {
        animation = animations[animationId];
        return true;
    }

    return false;
}

auto Engine2::Sprite::str() -> std::string
{
    return "Sprite [" + id + "] Sprite sheet [" + spriteSheetId + "] " + "Box [ Position:" + position.toStr() +
           " Width: " + std::to_string(width) + " Height: " + std::to_string(height) + "] RGBA [" + rgba.toStr() +
           "] Angle [" +  std::to_string(angle) + "] Scale [" +  std::to_string(scale) + "] Layer [" +
           std::to_string(layer) + "] " + animation.str();
}

void Engine2::Animation::forward()
{
    frame++;
    frame = frame % sequence.size();
}

void Engine2::Animation::restart()
{
    frame = 0;
}

void Engine2::Animation::setFrame(int newFrame)
{
    if (newFrame > 0 && newFrame < sequence.size()) {
        frame = newFrame;
    }
}

void Engine2::Animation::setLoop(bool running)
{
    loop = running;
}

void Engine2::Animation::setSpeed(int newSpeed)
{
    speed = newSpeed < 0 ? 0 : newSpeed;
}

auto Engine2::Animation::str() -> std::string
{
    return "Animation [" + id + ", Frame [" + std::to_string(frame) + "] Speed [" +
           std::to_string(speed) + "] Loop [ " + (loop ? "true" : "false") + "] ]";
}

auto Engine2::stopAnimation(std::string id) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to stop animation!");
        }
        return false;
    }

    m_sprites[id].animation.setLoop(false);

    return true;
}

auto Engine2::restartAnimation(std::string id) -> bool
{
    if (!m_sprites.contains(id)) {
        if (!m_quiet) {
            inform("Sprite ID: " + id + ", not found! Unable to restart animation!");
        }
        return false;
    }

    m_sprites[id].animation.setLoop(true);

    return true;
}

auto Engine2::checkCollision(std::string spriteId1, std::string spriteId2) -> bool
{
    if (!m_sprites.contains(spriteId1)) {
        if (!m_quiet) {
            inform("Sprite ID: " + spriteId1 + ", not found! Cannot check collision!");
        }
        return false;
    }

    if (!m_sprites.contains(spriteId2)) {
        if (!m_quiet) {
            inform("Sprite ID: " + spriteId2 + ", not found! Cannot check collision!");
        }
        return false;
    }

    if (!m_sprites[spriteId1].solid || !m_sprites[spriteId2].solid) {
        return false;
    }

    if (m_sprites[spriteId1].width <= 0 || m_sprites[spriteId1].height <= 0) {
        return false;
    }

    if (m_sprites[spriteId2].width <= 0 || m_sprites[spriteId2].height <= 0) {
        return false;
    }

    // Option 1 - SDL_IntersectRect
    //SDL_Rect rect;
    //SDL_Rect box1{static_cast<int>(m_sprites[spriteId1].position.X.toInt()),
    //              static_cast<int>(m_sprites[spriteId1].position.Y.toInt()),
    //              static_cast<int>(m_sprites[spriteId1].width),
    //              static_cast<int>(m_sprites[spriteId1].height)};
    //SDL_Rect box2{static_cast<int>(m_sprites[spriteId2].position.X.toInt()),
    //              static_cast<int>(m_sprites[spriteId2].position.Y.toInt()),
    //              static_cast<int>(m_sprites[spriteId2].width),
    //              static_cast<int>(m_sprites[spriteId2].height)};
    //return SDL_IntersectRect(&box1, &box2, &rect);

    // Option 2 - Small Tool Box - Base::intersect(const Points &polygonPoints) -> Points
    BoxCollider box = m_sprites[spriteId1].boxCollider();
    return !box.intersect(m_sprites[spriteId2].boxCollider().points()).empty();
}

auto Engine2::spriteSheet(std::string id) -> std::tuple<SpriteSheet, bool>
{
    if (!m_spriteSheets.contains(id)) {
        inform("Image ID: " + id + ", not found!");
        return {SpriteSheet(), false};
    }

    return {m_spriteSheets[id], true};
}

auto Engine2::spriteSheetSize(std::string id) -> stbox::Math::Size<float>
{
    if (!m_spriteSheets.contains(id)) {
        inform("Image ID: " + id + ", not found! Return negative size! ");
        return {-1, -1};
    }

    return {static_cast<float>(m_spriteSheets[id].spriteWidth),
            static_cast<float>(m_spriteSheets[id].spriteHeight)};
}

void Engine2::Text::move(Position distance)
{
    position += distance;
}

void Engine2::Text::setAngle(double newAngle)
{
    angle = newAngle;
}

void Engine2::Text::setColor(RGBA color)
{
    foreground = color;
}

void Engine2::Text::setOpacity(Uint8 opacityPercentage)
{
    opacity = opacityPercentage < 0 ? 0 : static_cast<float>((opacityPercentage % 101) * 255) / 100;;
}

void Engine2::Text::setScale(float newScale)
{
    scale = newScale;
}

auto Engine2::text(std::string id) -> Text
{
    if (!m_texts.contains(id)) {
        inform("Text ID: " + id + ", not found!");
        return {};
    }

    return m_texts[id];
}

auto Engine2::texts() -> std::map<std::string, Text>
{
    return m_texts;
}

auto Engine2::renderSprite(Sprite sprite) -> bool
{
    if (!m_spriteSheets.contains(sprite.spriteSheetId)) {
        inform("Sprite ID: " + sprite.id + ", sprite sheet " + sprite.spriteSheetId + " not found!");
        return false;
    }

    if (sprite.animation.frame < 0 || sprite.animation.frame > sprite.animation.sequence.size()) {
        inform("Sprite ID: " + sprite.id + ", sprite index out of range!");
        return false;
    }

    auto spriteSheet = m_spriteSheets[sprite.spriteSheetId];

    if (sprite.animation.sequence.empty() || spriteSheet.count < 1) {
        inform("Sprite ID: " + sprite.id + ", Failed to load sprites!");
        return false;
    }

    if (sprite.animation.loop && sprite.animation.speed > 0) {
        if (performanceReport().ticks % sprite.animation.speed == 0) {
            sprite.animation.forward();
        }
    }

    auto currentSprite = sprite.animation.sequence[sprite.animation.frame];
    int row = currentSprite / spriteSheet.columns;
    int column = currentSprite - (row * spriteSheet.columns);
    int x = spriteSheet.spriteWidth * column;
    int y = spriteSheet.spriteHeight * row;

    SDL_Rect clip = {x, y, spriteSheet.spriteWidth, spriteSheet.spriteHeight};
    SDL_FRect box = {sprite.position.X.toFloat(), sprite.position.Y.toFloat(), sprite.width, sprite.height};
    return renderTexture(sprite.spriteSheetId, clip, box, sprite.angle, sprite.scale, sprite.opacity, SDL_FLIP_NONE);
}

auto Engine2::renderText(std::string textId) -> bool
{
    if (m_texts.contains(textId)) {
        inform("Text ID: " + textId + ", already exists!");
        return false;
    }

    if (m_texts[textId].activity == State::Activity::activated) {
        auto text = m_texts[textId];
        SDL_Color color = {text.foreground.R, text.foreground.G, text.foreground.B, text.foreground.A};
        renderTextFont(text.fontId, text.text, {text.position.X.toFloat(), text.position.Y.toFloat()},
                       color, text.fontSize, 0, 1, text.opacity, SDL_FLIP_NONE);
    }

    return true;
}

void Engine2::viewBoxCollider(bool enabled)
{
    m_viewBoxCollider = enabled;
}

void Engine2::render()
{
    // Use the Layer attribute on the Sprite and Text to control the rendering order.
    for (auto layer : m_layers) {
        for (auto item : m_sprites) {
            auto sprite = item.second;
            if (sprite.layer == layer) {
                SDL_FRect box = {sprite.position.X.toFloat(), sprite.position.Y.toFloat(),
                                 sprite.width, sprite.height
                                };
                if (sprite.activity == State::Activity::activated) {
                    if (!renderSprite(sprite)) {
                        SDL_SetRenderDrawColor(currentRenderer(), 255, 0, 0, 255);
                        SDL_RenderFillRectF(currentRenderer(), &box);
                    }
                    if (m_viewBoxCollider) {   // For need to see texture limits.
                        if (sprite.width > 0 && sprite.height > 0) {
                            SDL_SetRenderDrawColor(currentRenderer(), 255, 0, 0, 255);
                            SDL_RenderDrawRectF(currentRenderer(), &box);
                        }
                    }
                }
            }
        }
        // Text above textures.
        for (auto item : m_texts) {
            auto text = item.second;
            if (text.activity == State::Activity::activated) {
                if (text.layer == layer) {
                    SDL_Color color = {text.foreground.R,
                                       text.foreground.G,
                                       text.foreground.B,
                                       text.foreground.A
                                      };
                    Engine::renderTextFont(text.fontId, text.text,
                    {text.position.X.toFloat(), text.position.Y.toFloat()}, color,
                    text.fontSize, text.angle, text.scale, text.opacity, SDL_FLIP_NONE);
                }
            }
        }
    }
}
