#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <queue>

using namespace geode::prelude;

// Adjust how many frames you want to delay mobile taps (e.g. 3 frames)
const int kDelayFrames = 3; 

struct DelayedTouch {
    int framesRemaining;
    bool isPush;
};

static std::queue<DelayedTouch> g_touchQueue;
static bool g_isProcessingDelayedTouch = false;

class $modify(DelayedPlayerObject, PlayerObject) {
    void pushButton(PlayerButton p0) {
        if (g_isProcessingDelayedTouch) {
            PlayerObject::pushButton(p0);
            return;
        }

        g_touchQueue.push({kDelayFrames, true});
    }

    void releaseButton(PlayerButton p0) {
        if (g_isProcessingDelayedTouch) {
            PlayerObject::releaseButton(p0);
            return;
        }

        g_touchQueue.push({kDelayFrames, false});
    }
};

class $modify(DelayedPlayLayer, PlayLayer) {
    void update(float dt) {
        std::queue<DelayedTouch> nextQueue;
        
        while (!g_touchQueue.empty()) {
            auto touch = g_touchQueue.front();
            g_touchQueue.pop();

            touch.framesRemaining--;

            if (touch.framesRemaining <= 0) {
                g_isProcessingDelayedTouch = true;
                if (m_player1) {
                    if (touch.isPush) {
                        static_cast<PlayerObject*>(m_player1)->pushButton(PlayerButton::Jump);
                    } else {
                        static_cast<PlayerObject*>(m_player1)->releaseButton(PlayerButton::Jump);
                    }
                }
                g_isProcessingDelayedTouch = false;
            } else {
                nextQueue.push(touch);
            }
        }
        g_touchQueue = nextQueue;

        PlayLayer::update(dt);
    }
};
