#ifndef CUSTOM_GAME_ENGINE2_H
#define CUSTOM_GAME_ENGINE2_H

#include <set>
#include <vector>

#include "engine.h"
#include "./common/smallToolBox.h"

using BoxCollider = stbox::Rectangle;
using Position    = stbox::Point;
using RGBA        = stbox::Color::RGBA;


// SDL Game
// Structure with minimum resources for building the Game.
class Engine2 : public Engine {

public:
    Engine2(int screenWidth, int screenHeight, const std::string &title = "");

private:
    struct Text {
        std::string id{""};
        std::string fontId{""};
        std::string text{""};
        Position position{0, 0};
        RGBA foreground{255, 255, 255};
        double angle{0};
        float opacity{255};
        float scale{0};
        int fontSize{12};
        int layer{0};
        State::Activity activity{State::Activity::activated};

        void move(Position distance);
        void setAngle(double newAngle);
        void setColor(RGBA color);
        void setOpacity(Uint8 opacityPercentage);
        void setScale(float newScale);
    };

    struct SpriteSheet {
        std::string id{""};
        std::string filename{""};
        int rows{0};
        int columns{0};
        int spriteWidth{0};
        int spriteHeight{0};
        int count{0};
    };

    struct Animation {
        std::string id{""};
        int frame{0};
        int speed{0};
        bool loop{false};
        std::vector<int> sequence{};

        void forward();
        void restart();
        void setFrame(int newFrame);
        void setLoop(bool running);
        void setSpeed(int newSpeed);

        auto str() -> std::string;
    };

    struct Sprite {
        std::string id{""};
        std::string spriteSheetId{""};
        Position position;
        RGBA rgba{255, 0, 0, 255};
        bool solid{false};
        double angle{0};
        float height;
        float opacity{255};
        float scale{0};
        float width;
        int layer{0};
        Animation animation;
        std::map<std::string, Animation> animations;
        State::Activity activity{State::Activity::disabled};

        auto boxCollider() -> BoxCollider;
        auto size() -> stbox::Math::Size<float>;
        auto center() -> Position;

        auto getAnimation() -> Animation;
        auto setAnimation(std::string animationId) -> bool;

        void move(Position distance);
        void move(Position distance, stbox::Rectangle limit);
        void move(double distance, double angle);
        void move4directions(double distance, stbox::Rectangle limit, State::Input state);
        void move8directions(double distance, stbox::Rectangle rectangle, State::Input state);
        void moveHorizontal(double distance);
        void moveTo(Position newPosition, double limit, bool isGreater, bool vertically);
        void moveVertical(double distance);
        void rotate(double rotationAngle);
        void setAngle(double newAngle);
        void setColor(RGBA color);
        void setOpacity(Uint8 opacityPercentage);
        void setScale(float newScale);
        void setSolid(bool isSolid);

        auto str() -> std::string;
    };

protected:
    // Window
    template<typename T>
    auto windowSize() -> stbox::Math::Size<T>
    {
        return {windowWidth<T>(), windowHeight<T>()};
    }

    // Text
    virtual auto newText(const std::string &id, const std::string &fontId, const std::string &text, Position position, RGBA foreground, int size = 8) -> bool final;
    virtual auto setText(std::string id, std::string text, bool quiet = true) -> bool final;
    virtual auto setTextActivity(std::string id, State::Activity activity, bool quiet = true) -> bool final;
    virtual auto setTextAngle(std::string id, int angle, bool quiet = true) -> bool final;
    virtual auto setTextColor(std::string id, RGBA foreground, bool quiet = true) -> bool final;
    virtual auto setTextFont(std::string id, std::string fontId, bool quiet = true) -> bool final;
    virtual auto setTextLayer(std::string id, int layer, bool quiet = true) -> bool final;
    virtual auto setTextOpacity(std::string id, Uint8 opacity, bool quiet = true) -> bool final;
    virtual auto setTextPosition(std::string id, Position position, bool quiet = true) -> bool final;
    virtual auto setTextScale(std::string id, float scale, bool quiet = true) -> bool final;
    virtual auto setTextSize(std::string id, int size, bool quiet = true) -> bool final;

    virtual auto text(std::string id) -> Text final;
    virtual auto texts() -> std::map<std::string, Text> final;

    // Texture, Image, Sprite, Animation
    virtual auto newSprite(std::string id, std::string spriteSheetId, BoxCollider box, bool quiet = true) -> bool final;
    virtual auto newSprite(std::string id, std::string spriteSheetId, bool quiet = true) -> bool final;
    virtual auto newSprite(std::string id, std::string spriteSheetId, float x, float y, bool quiet = true) -> bool final;
    virtual auto setSpriteActivity(std::string id, State::Activity activity, bool quiet = true) -> bool final;
    virtual auto setSpriteAngle(std::string id, double angle, bool quiet = true) -> bool final;
    virtual auto setSpriteBoxCollider(std::string id, BoxCollider box, bool quiet = true) -> bool final;
    virtual auto setSpriteBoxCollider(std::string id, float width, float height, bool quiet = true) -> bool final;
    virtual auto setSpriteByIndex(std::string id, int index, bool quiet = true) -> bool final;
    virtual auto setSpriteEmptyBoxCollider(std::string id, Position position, bool quiet = true) -> bool final;
    virtual auto setSpriteEmptyBoxCollider(std::string id, bool quiet = true) -> bool final;
    virtual auto setSpriteLayer(std::string id, int layer, bool quiet = true) -> bool final;
    virtual auto setSpriteOpacity(std::string id, Uint8 opacity, bool quiet = true) -> bool final;
    virtual auto setSpritePosition(std::string id, Position position, bool quiet = true) -> bool final;
    virtual auto setSpriteScale(std::string id, float scale, bool quiet = true) -> bool final;
    virtual auto setSpriteSolid(std::string id, bool isSolid, bool quiet = true) -> bool final;
    virtual auto spriteSheetSize(std::string id, bool quiet = true) -> stbox::Math::Size<float> final;

    virtual auto eraseSprite(std::string id, bool quiet = true) -> bool final;
    virtual auto swapSprite(std::string id, Sprite sprite, bool quiet = true) -> bool final;

    virtual auto sprite(std::string id, bool quiet = true) -> std::tuple<Sprite, bool> final;
    virtual auto sprites() -> std::map<std::string, Sprite> final;
    virtual auto totalSprites() -> int final;

    virtual auto checkCollision(std::string spriteId1, std::string spriteId2, bool quiet = true) -> bool final;

    virtual auto newAnimation(std::string id, std::string spriteId, std::vector<int> spriteNumbers, int timeInterval, bool quiet = true) -> bool final;
    virtual auto restartAnimation(std::string id, bool quiet = true) -> bool final;
    virtual auto setAnimation(std::string id, std::string spriteId, bool quiet = true) -> bool final;
    virtual auto setAnimationSpeed(std::string id, int speed, bool quiet = true) -> bool final;
    virtual auto stopAnimation(std::string id, bool quiet = true) -> bool final;

    virtual auto loadSprite(std::string id, const std::string &filename, bool quiet = true) -> bool final;
    virtual auto loadSpriteSheet(std::string id, const std::string &filename) -> bool final;
    virtual auto loadSpriteSheet(const std::string &id, int rows, int columns, const std::string &filename) -> bool final;

    // Render
    virtual auto renderSprite(Sprite sprite) -> bool final;
    virtual auto renderText(std::string textId) -> bool final;
    virtual void viewBoxCollider(bool enabled = true) final;

    // Control function.
    void render() override;

private:
    bool m_viewBoxCollider{false};

    std::map<std::string, Text> m_texts;
    std::map<std::string, Sprite> m_sprites;
    std::map<std::string, SpriteSheet> m_spriteSheets;
    std::set<int> m_layers{0};

    virtual auto newSprite(std::string id, std::string spriteSheetID, float x, float y, float width, float height, bool quiet = true) -> bool final;
    virtual auto spriteSheet(std::string id) -> std::tuple<SpriteSheet, bool> final;
};

#endif
