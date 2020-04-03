#ifndef ANT_WORLD_PLAYER_H
#define ANT_WORLD_PLAYER_H

#include "insect.h"

namespace aw {

    class Player : public Insect {
    public:
        Player();
        ~Player();

        virtual void initialize();
        virtual void process();
        virtual void render();

        void updateEnergy(float dt);

    protected:
        virtual void grab();
        virtual void drop();
        virtual void eat();

        virtual void enterHole();
        virtual void exitHole();

        void updateMotion();
        void updateAnimation();

    protected:
        ysAnimationActionBinding
            m_animBlink,
            m_animIdle,
            m_animWalk,
            m_faceUp,
            m_faceDown,
            m_faceLeft,
            m_faceRight;

        dbasic::RenderSkeleton *m_renderSkeleton;

        ysAnimationChannel *m_legsChannel;
        ysAnimationChannel *m_antennaChannel;
        ysAnimationChannel *m_eyelidChannel;
        ysAnimationChannel *m_bodyRotationChannel;

    protected:
        float m_energy;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static ysAnimationAction
            *AnimBlink,
            *AnimIdle,
            *AnimWalk,
            *PoseFaceUp,
            *PoseFaceDown,
            *PoseFaceRight,
            *PoseFaceLeft;

        static dbasic::SceneObjectAsset *CharacterRoot;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PLAYER_H */
