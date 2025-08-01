#include "pch.h"
#include "PowerUp.h"

const char* PowerUp::PowerTypeString(Type type)
{
	switch (type) {
	case Chaos:                 return "Chaos";
	case DecreasePaddleSize:    return "DecreasePaddleSize";
	case IncreaseBallVelocity:  return "IncreaseBallVelocity";
	case Confuse:               return "Confuse";
	case DecreaseBallVelocity:  return "DecreaseBallVelocity";
	case IncreasePaddleSize:    return "IncreasePaddleSize";
	case StickyPaddle:          return "StickyPaddle";
	case PassThrough:           return "PassThrough";
	case BonusLife:             return "BonusLife";
		// case FireBall:           return "FireBall"; // If re-added
	case End:                   return "End";
	default:                    return "Unknown";
	}
}
