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

protected:
    // Game Structs
    struct Text {
        std::string id;
        std::string fontId;
        std::string text;
        Position position{0, 0};
        RGBA foreground{255, 255, 255};
        double angle{0};
        float opacity{255};
        float scale{0};
        int fontSize{12};
        int layer{0};
        State::Activity activity{State::Activity::activated};

        virtual void move(Position distance) final;
        virtual void setAngle(double newAngle) final;
        virtual void setColor(RGBA color) final;
        virtual void setOpacity(Uint8 opacityPercentage) final;
        virtual void setScale(float newScale) final;
    };

    struct SpriteSheet {
        std::string id;
        std::string filename;
        int rows{0};
        int columns{0};
        int spriteWidth{0};
        int spriteHeight{0};
        int count{0};
    };

    struct Animation {
        std::string id;
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
        std::string id;
        std::string spriteSheetId;
        Position position;
        RGBA rgba{255, 0, 0, 255};
        Uint8 opacity{255};
        bool solid{false};
        double angle{0};
        float height;
        float scale{0};
        float width;
        int layer{0};
        Animation animation;
        std::map<std::string, Animation> animations;
        State::Activity activity{State::Activity::disabled};

        virtual auto boxCollider() -> BoxCollider final;
        virtual auto size() -> stbox::Math::Size<float> final;
        virtual auto center() -> Position final;

        virtual auto getAnimation() -> Animation final;
        virtual auto setAnimation(std::string animationId) -> bool final;

        virtual void move(Position distance) final;
        virtual void move(Position distance, stbox::Rectangle limit) final;
        virtual void move(double distance, double angle) final;
        virtual void move4directions(double distance, stbox::Rectangle limit, State::Input state) final;
        virtual void move8directions(double distance, stbox::Rectangle rectangle, State::Input state) final;
        virtual void moveHorizontal(double distance) final;
        virtual void moveTo(Position newPosition, double limit, bool isGreater, bool vertically) final;
        virtual void moveVertical(double distance) final;
        virtual void rotate(double rotationAngle) final;
        virtual void setAngle(double newAngle) final;
        virtual void setColor(RGBA color) final;
        virtual void setOpacity(Uint8 opacityPercentage) final;
        virtual void setScale(float newScale) final;
        virtual void setSolid(bool isSolid) final;

        virtual auto str() -> std::string final;
    };

    // Window
    template<typename T>
    auto windowSize() -> stbox::Math::Size<T>
    {
        return {windowWidth<T>(), windowHeight<T>()};
    }

    // Text
    virtual auto newText(const std::string &id, const std::string &fontId, const std::string &text, Position position, RGBA foreground, int size) -> bool final;
    virtual auto setText(std::string id, std::string text) -> bool final;
    virtual auto setTextActivity(std::string id, State::Activity activity) -> bool final;
    virtual auto setTextAngle(std::string id, int angle) -> bool final;
    virtual auto setTextColor(std::string id, RGBA foreground) -> bool final;
    virtual auto setTextFont(std::string id, std::string fontId) -> bool final;
    virtual auto setTextLayer(std::string id, int layer) -> bool final;
    virtual auto setTextOpacity(std::string id, Uint8 opacity) -> bool final;
    virtual auto setTextPosition(std::string id, Position position) -> bool final;
    virtual auto setTextScale(std::string id, float scale) -> bool final;
    virtual auto setTextSize(std::string id, int size) -> bool final;

    virtual auto text(std::string id) -> Text final;
    virtual auto texts() -> std::map<std::string, Text> final;

    // Texture, Image, Sprite, Animation
    virtual auto newSprite(Sprite sprite) -> bool final;
    virtual auto newSprite(std::string id, std::string spriteSheetId, BoxCollider box) -> bool final;
    virtual auto newSprite(std::string id, std::string spriteSheetId) -> bool final;
    virtual auto newSprite(std::string id, std::string spriteSheetId, float x, float y) -> bool final;
    virtual auto setSpriteActivity(std::string id, State::Activity activity) -> bool final;
    virtual auto setSpriteAngle(std::string id, double angle) -> bool final;
    virtual auto setSpriteBoxCollider(std::string id, BoxCollider box) -> bool final;
    virtual auto setSpriteBoxCollider(std::string id, float width, float height) -> bool final;
    virtual auto setSpriteByIndex(std::string id, int index) -> bool final;
    virtual auto setSpriteEmptyBoxCollider(std::string id, Position position) -> bool final;
    virtual auto setSpriteEmptyBoxCollider(std::string id) -> bool final;
    virtual auto setSpriteLayer(std::string id, int layer) -> bool final;
    virtual auto setSpriteOpacity(std::string id, Uint8 opacity) -> bool final;
    virtual auto setSpritePosition(std::string id, Position position) -> bool final;
    virtual auto setSpriteScale(std::string id, float scale) -> bool final;
    virtual auto setSpriteSolid(std::string id, bool isSolid) -> bool final;
    virtual auto spriteSheetSize(std::string id) -> stbox::Math::Size<float> final;

    virtual auto eraseSprite(std::string id) -> bool final;
    virtual auto swapSprite(std::string id, Sprite sprite) -> bool final;

    virtual auto sprite() -> Sprite final;  // Returns empty Sprite.
    virtual auto sprite(std::string id) -> std::tuple<Sprite, bool> final;
    virtual auto sprites() -> std::map<std::string, Sprite> final;
    virtual auto totalSprites() -> size_t final;

    virtual auto checkCollision(std::string spriteId1, std::string spriteId2) -> bool final;

    virtual auto newAnimation(std::string id, std::string spriteId, std::vector<int> spriteNumbers, int timeInterval) -> bool final;
    virtual auto restartAnimation(std::string id) -> bool final;
    virtual auto setAnimation(std::string id, std::string spriteId) -> bool final;
    virtual auto setAnimationSpeed(std::string id, int speed) -> bool final;
    virtual auto stopAnimation(std::string id) -> bool final;

    virtual auto loadSprite(std::string id, const std::string &filename) -> bool final;
    virtual auto loadSpriteSheet(std::string id, const std::string &filename) -> bool final;
    virtual auto loadSpriteSheet(const std::string &id, int rows, int columns, const std::string &filename) -> bool final;

    // Render
    virtual auto renderSprite(Sprite sprite) -> bool final;
    virtual auto renderText(std::string textId) -> bool final;
    virtual void viewBoxCollider(bool enabled) final;

    // Control function.
    void render() override;

private:
    bool m_viewBoxCollider{false};
    bool m_quiet{true}; // view log

    std::map<std::string, Text> m_texts;
    std::map<std::string, Sprite> m_sprites;
    std::map<std::string, SpriteSheet> m_spriteSheets;
    std::set<int> m_layers{0};

    virtual auto newSprite(std::string id, std::string spriteSheetID, float x, float y, float width, float height) -> bool final;
    virtual auto spriteSheet(std::string id) -> std::tuple<SpriteSheet, bool> final;
};

#endif
