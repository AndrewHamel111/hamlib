/* TODO
** - consider an "animation registry" similar to alarm registry. for now each drawanim requires passing the GetFrameTime() to advance animations
*/

#ifndef TMP_ANIMATION_H_
#define TMP_ANIMATION_H_

#include "sprite.h"

enum SpriteAnimationType
{
	SAT_ONCE, SAT_LOOPED, SAT_PING_PONG, SAT_PING_PONG_ONCE, SAT_LOOPED_N, SAT_PING_PONG_N
};

typedef struct _SpriteAnimation
{
	Sprite* sprites;
	int sprites_length;
	int frames_per_second;
	enum SpriteAnimationType animation_type;
	int loop_count;

	// PRIVATE
	float _seconds_on_frame;
	int _frame_current;
	float _seconds_per_frame;
	bool _playing;
	bool _ping_pong_reverse;
	int _loops_left;

} SpriteAnimation;

/**
 * \brief Every call to DrawSpriteAnimation__ must perform these actions first, to maintain the animation state.
 */
void DrawSpriteAnimationBase(float frametime, SpriteAnimation* animation);

/**
 * \brief SpriteAnimation constructor from minimum fields.
 * 
 * \param sprites_length no. of frames in animation
 * \param sprites array of sprites corresponding to the animation
 * \param frames_per_second frames per second of the animation
 * \return SpriteAnimation 
 */
SpriteAnimation NewAnimation(int sprites_length, Sprite sprites[], int frames_per_second);
/**
 * \brief Set the animations framerate. Sets invalid values to 1.
 * 
 * \return SpriteAnimation modified animation
 */
SpriteAnimation ChangeAnimationFramerate(SpriteAnimation* animation, int frames_per_second);

/**
 * \brief Set the value of N for N loop styles.
 */
SpriteAnimation SetSpriteAnimationLoops(SpriteAnimation* animation, int loop_count, bool usePingPong);

/**
 * \brief Draws the current sprite of the animation. If the animation is not playing, it will simply draw the "locked" frame of the animation until Reset/Start/Stop are called.
 */
void DrawSpriteAnimation(float frametime, SpriteAnimation* animation, int posX, int posY, Color tint);
/**
 * \brief Draws the current sprite of the animation. If the animation is not playing, it will simply draw the "locked" frame of the animation until Reset/Start/Stop are called.
 */
void DrawSpriteAnimationV(float frametime, SpriteAnimation* animation, Vector2 position, Color tint);
/**
 * \brief Draws the current sprite of the animation. If the animation is not playing, it will simply draw the "locked" frame of the animation until Reset/Start/Stop are called.
 */
void DrawSpriteAnimationPro(float frametime, SpriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint);

/**
 * \brief Reset all runtime values of SpriteAnimation except _playing.
 */
void SpriteAnimationReset(SpriteAnimation* animation);
/**
 * \brief Sets _playing = false.
 */
void SpriteAnimationStop(SpriteAnimation* animation);
/**
 * \brief Sets _playing = true.
 */
void SpriteAnimationResume(SpriteAnimation* animation);
/**
 * \brief Calls ResetAnimation, then ResumeAnimation.
 */
void SpriteAnimationStart(SpriteAnimation* animation);

#endif