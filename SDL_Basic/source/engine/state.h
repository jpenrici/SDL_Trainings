#ifndef STATE_H
#define STATE_H


namespace State {

enum class Activity {
    none,
    activated,
    disabled
};

enum class Animation {
    none,
    idle,
    walking,
    running,
    jumping,
    falling
};

enum class Behavior {
    none,
    moveToUp,
    moveToDown,
    moveToLeft,
    moveToRight,
    moveDiagonally,
    moveHorizontally,
    moveVertically,
    sttoped
};

enum class Common {
    none,
    undefined
};

enum class Difficuly {
    none,
    beginner,
    easy,
    normal,
    hard,
    expert
};

enum class Game {
    none,
    playing,
    paused,
    stopped,
    restart,
    gameOver,
    gameWinner,
    saved,
    quit
};

enum class Input {
    none,
    keyboard_a,
    keyboard_d,
    keyboard_down,
    keyboard_down_left,
    keyboard_down_left_space,
    keyboard_down_right,
    keyboard_down_right_space,
    keyboard_down_space,
    keyboard_escape,
    keyboard_left,
    keyboard_left_space,
    keyboard_right,
    keyboard_right_space,
    keyboard_s,
    keyboard_space,
    keyboard_up,
    keyboard_up_left,
    keyboard_up_left_space,
    keyboard_up_rigth,
    keyboard_up_rigth_space,
    keyboard_up_space,
    keyboard_w,
    mouse_button_left,
    mouse_button_right
};

enum class Level {
    none,
    started,
    concluded
};

};  // namespace State

#endif
