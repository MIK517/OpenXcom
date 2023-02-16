/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MiniBaseView.h"
#include <cmath>
#include "../Engine/SurfaceSet.h"
#include "../Engine/Action.h"
#include "../Savegame/Base.h"
#include "../Savegame/BaseFacility.h"
#include "../Mod/RuleBaseFacility.h"

namespace OpenXcom
{

/**
 * Sets up a mini base view with the specified size and position.
 * @param bases Pointer to base list to display.
 * @param width Width in pixels.
 * @param height Height in pixels.
 * @param x X position in pixels.
 * @param y Y position in pixels.
 */
MiniBaseView::MiniBaseView(std::vector<Base *> *bases, NewBaseSelectedHandler newBaseSelectedHandler, int width, int height, int x, int y) : InteractiveSurface(width, height, x, y), _bases(bases), _startIndex(0), _newBaseSelectedHandler(newBaseSelectedHandler), _texture(0), _base(0), _hoverBase(0), _red(0), _green(0), _blue(0)
{
}

/**
 *
 */
MiniBaseView::~MiniBaseView()
{
}

/**
 * Changes the texture to use for drawing
 * the various base elements.
 * @param texture Pointer to SurfaceSet to use.
 */
void MiniBaseView::setTexture(SurfaceSet *texture)
{
	_texture = texture;
}

/**
 * Returns the base the mouse cursor is currently over.
 * @return ID of the base.
 */
size_t MiniBaseView::getHoveredBase() const
{
	return _hoverBase;
}

/**
 * Changes the base that is currently selected on
 * the mini base view.
 * @param base the desired base.
 */
void MiniBaseView::setSelectedBase(Base *base)
{
	for (size_t i = 0; i < _bases->size(); ++i)
	{
		if (_bases->at(i) == base)
		{
			if (i >= MAX_BASES-1)
			{
				if (i < _bases->size()-1) _base = MAX_BASES-2; else _base = MAX_BASES-1;
				_startIndex = i - _base;
			}
			else
			{
				_base = i;
				_startIndex = 0;
			}
			break;
		}
	}
	_redraw = true;
}

/**
 * Draws the view of all the bases with facilities
 * in varying colors.
 */
void MiniBaseView::draw()
{
	Surface::draw();
	for (size_t i = 0; i < MAX_BASES; ++i)
	{
		// Draw base squares
		if (i == _base)
		{
			SDL_Rect r;
			r.x = i * (MINI_SIZE + 2);
			r.y = 0;
			r.w = MINI_SIZE + 2;
			r.h = MINI_SIZE + 2;
			drawRect(&r, 1);
		}
		_texture->getFrame(41)->blitNShade(this, i * (MINI_SIZE + 2), 0);

		// Draw facilities
		if (_startIndex+i < _bases->size())
		{
			SDL_Rect r;
			lock();
			for (const auto* fac : *_bases->at(_startIndex+i)->getFacilities())
			{
				int color;
				if (fac->getDisabled())
					color = _blue;
				else if (fac->getBuildTime() == 0)
					color = _green;
				else
					color = _red;

				r.x = i * (MINI_SIZE + 2) + 2 + fac->getX() * 2;
				r.y = 2 + fac->getY() * 2;
				r.w = fac->getRules()->getSizeX() * 2;
				r.h = fac->getRules()->getSizeY() * 2;
				drawRect(&r, color+3);
				r.x++;
				r.y++;
				r.w--;
				r.h--;
				drawRect(&r, color+5);
				r.x--;
				r.y--;
				drawRect(&r, color+2);
				r.x++;
				r.y++;
				r.w--;
				r.h--;
				drawRect(&r, color+3);
				r.x--;
				r.y--;
				setPixel(r.x, r.y, color+1);
			}
			unlock();
		}
	}
}

/**
 * Selects the base the mouse is over.
 * @param action Pointer to an action.
 * @param state State that the action handlers belong to.
 */
void MiniBaseView::mouseOver(Action *action, State *state)
{
	_hoverBase = _startIndex+(int)floor(action->getRelativeXMouse() / ((MINI_SIZE + 2) * action->getXScale()));
	InteractiveSurface::mouseOver(action, state);
}

/**
 * Handles a mouse click event.
 * @param action Pointer to an action.
 * @param state State that the action handlers belong to.
 */
void MiniBaseView::mouseClick(Action *action, State *state)
{
	if (SDL_BUTTON_LEFT == action->getDetails()->button.button)
	{
		size_t base = (int)floor(action->getRelativeXMouse() / ((MINI_SIZE + 2) * action->getXScale()));
		if (_startIndex+base < _bases->size())
		{
			if (MAX_BASES-1 == base && _startIndex+base+1 < _bases->size())
			{
				++_startIndex; --base;
			}
			else if (0 == base && _startIndex > 0)
			{
				--_startIndex; ++base;
			}
			_base = base;
			_redraw = true;
			(state->*_newBaseSelectedHandler)(_bases->at(_startIndex+base));
		}
	}
	InteractiveSurface::mouseClick(action, state);
}

void MiniBaseView::setColor(Uint8 color)
{
	_green = color;
}
void MiniBaseView::setSecondaryColor(Uint8 color)
{
	_red = color;
}
void MiniBaseView::setBorderColor(Uint8 color)
{
	_blue = color;
}

}
