/*
 */

#include "IAnimatedGameObject.h"
#include "stdafx.h"

IAnimatedGameObject::IAnimatedGameObject (sf::Time frameTime, bool paused, bool looped) : 
  mAnimation {nullptr}, mFrametime {frameTime}, mCurrentFrame {0}, mIsPaused {paused}, mIsLooped {looped}, mTexture {nullptr}
{ }

void IAnimatedGameObject::setAnimation (const Animation& animation)
{
  mAnimation = &animation;
  mTexture = mAnimation->getSpritesheet();
  mCurrentFrame = 0;
  setFrame(mCurrentFrame);
}

void IAnimatedGameObject::setFrameTime (sf::Time time)
{
  mFrameTime = time;
}

void IAnimatedGameObject::play ()
{
  mIsPaused = false;
}

void IAnimatedGameObject::play (const Animation& animation)
{
  if (getAnimation() != &animation)
    setAnimation(animation);
  play();
}

void IAnimatedGameObject::pause ()
{
  mIsPaused = true;
}

void IAnimatedGameObject::stop ()
{
  mIsPaused = true;
  mCurrentFrame = 0;
  setFrame(mCurrentFrame);
}

void IAnimatedGameObject::setLooped (bool looped)
{
  mIsLooped = looped;
}

const Animation* IAnimatedGameObject::getAnimation () const
{
  return mAnimation;
}

sf::Rect<float> IAnimatedGameObject::getLocalBounds () const
{
  sf::Rect<int> rect = mAnimation->getFrame(mCurrentFrame);

  float width = static_cast<float>(std::abs(rect.width));
  float height = static_cast<float>(std::abs(rect.height));

  return sf::Rect<float> {0.0f, 0.0f, width, height};
}

sf::Rect<float> IAnimatedGameObject::getGlobalBounds () const
{
  return getTransform().transformRect(getLocalBounds());
}

bool IAnimatedGameObject::isLooped () const
{
  return mIsLooped;
}

bool IAnimatedGameObject::isPlaying () const
{
  return !mIsPaused;
} 

sf::Time IAnimatedGameObject::getFrameTime () const
{
  return mFrameTime;
}

void IAnimatedGameObject::setFrame (std::size_t newFrame, bool resetTime)
{
  if (mAnimation) 
  {
    sf::Rect<int> rect = mAnimation->getFrame(newFrame);

    mVertices[0].position = sf::Vector2f(0.0f,0.0f);
    mVertices[1].position = sf::Vector2f(0.0f,static_cast<float>(rect.height));
    mVertices[2].position = sf::Vector2f(static_cast<float>(rect.width),static_cast<float>(rect.height));
    mVertices[3].position = sf::Vector2f(static_cast<float>(rect.width),0.0f);

    float left = static_cast<float>(rect.left) + 0.0001f;
    float right = left + static_cast<float>(rect.width);
    float top = static_cast<float>(rect.top);
    float bottom = top + static_cast<float>(rect.height);

    mVertice[0].texCoords = sf::Vector2f(left,top);
    mVertice[1].texCoords = sf::Vector2f(left,bottom);
    mVertice[2].texCoords = sf::Vector2f(right,bottom);
    mVertice[3].texCoords = sf::Vector2f(right,top);
  } 

  if (resetTime) 
    mCurrentTime = sf::Time::Zero;
}

void IAnimatedGameObject::update (sf::Time deltaTime)
{
  if ((!mIsPaused) && mAnimation) 
  {
    mCurrentTime += deltaTime;

    //if the current time is bigger than the frame time, advance one frame
    if (mCurrentTime >= mFrameTime) 
    {
      mCurrentTime = sf::microseconds(mCurrentTime.asMicroseconds() % mFrameTime.asMicroseconds());
      
      //get next frame index
      if (mCurrentFrame + 1 < mAnimation->getSize()) 
        mCurrentFrame++;
      else {
        //animation has ended
        mCurrentFrame = 0; //reset to start
        if (!mIsLooped)
          mIsPaused = true;
      }
    
      //set the current frame, not restting the time
      setFrame(mCurrentFrame,false);
    }
  }
}

void IAnimatedGameObject::draw (sf::RenderTarget& target, sf::RenderStates states)
{
  if (mAnimation && mTexture)
  {
    states.transform *= getTransform();
    states.texture = mTexture;
    target.draw(mVertices, 4, sf::Quad, states);
  }
}
