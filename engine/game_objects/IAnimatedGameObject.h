#ifndef IANIMATED_GAME_OBJECT_H
#define IANIMATED_GAME_OBJECT_H

#include "Animation.h"

class IAnimatedGameObject : public IVisibleGameObject
{
  public:
    explicit IAnimatedGameObject (sf::Time frameTime = sf::seconds(0.2f),bool paused = false, bool looped = true);


    void draw (sf::RenderTarget& target, sf::RenderStates states);
    void update (sf::Time deltaTime);
    void setAnimation (const Animation& animation);
    void setFrameTime (sf::Time time);
    void play ();
    void play (const Animation& animation);
    void pause ();
    void stop ();
    void setLooped (bool looped);
    //void setColor is inherited
    const Animation* getAnimation () const;
    sf::Rect<float> getLocalBounds () const;
    sf::Rect<float> getGlobalBounds () const;
    bool isLooped () const;
    bool isPlaying () const;
    sf::Time getFrameTime () const;
    void setFrame (stf::size_t newFrame, bool resetTime = true);

  private:
    mAnimation* animation;
    sf::Time mFrameTime;
    sf::Time mCurrentTime;
    std::size_t mCurrentFrame;
    bool mIsPaused;
    bool mIsLooped;
    //dunno if i need this
    sf::Texture mTexture;

};

#endif
