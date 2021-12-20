#include "hamlib.h"
#include "hamlib/animation.h"

animation createspriteanimation(Texture2D spritesheet, unsigned short frames, float animation_period)
{
	animation anim;
	anim.info = ANIM_SPRITE;
	anim.play_type = APT_LOOP;
	anim.animation_period = animation_period;
	anim.playing = false;
	anim.time = 0;
	anim.spritesheet = spritesheet;
	anim.frame = 0;
	anim.frames = frames;
	return anim;
}

void playanimation(animation* anim)
{
	if (anim->playing) stopanimation(anim);

	anim->playing = true;
}

void stopanimation(animation* anim)
{
	anim->playing = false;
	anim->frame = anim->time = 0;
}

void drawanimation(animation* anim, Vector2 pos, float frametime)
{
	anim->time += frametime;

	Rectangle r = {pos.x - (anim->spritesheet.width / anim->frames)/2, pos.y - anim->spritesheet.height/2, (anim->spritesheet.width / anim->frames), anim->spritesheet.height};
	drawanimationpro(anim, r, GetCenterRelative(r), 0.0f, WHITE);
}

void drawanimationpro(animation* anim, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	DrawTexturePro(anim->spritesheet, animationrect(*anim), dest, origin, rotation, tint);
}

Rectangle animationrect(animation anim)
{
	int cell_width = anim.spritesheet.width / anim.frames;
	return (Rectangle){(int)((anim.time / anim.animation_period) * anim.frames) * cell_width, 0, cell_width, anim.spritesheet.height - 1};
}