#pragma once

#include <cinttypes>

namespace Input
{
	typedef uint32_t KeyCode;

	namespace Key
	{
		enum : KeyCode
		{
			// from SDL_keycodes.h
			Unknown = 0,

			Return = '\r',
			Escape = '\033',
			Backspace = '\b',
			Tab = '\t',
			Space = ' ',
			Exclimation = '!',
			DoubleQuote = '"',
			Hash = '#',
			Percent = '%',
			Dollar = '$',
			Ampersand = '&',
			Quote = '\'',
			LeftParenthesis = '(',
			RightParenthesis = ')',
			Asterisk = '*',
			Plus = '+',
			Comma = ',',
			Minus = '-',
			Period = '.',
			ForwardSlash = '/',
			Digit0 = '0',
			Digit1 = '1',
			Digit2 = '2',
			Digit3 = '3',
			Digit4 = '4',
			Digit5 = '5',
			Digit6 = '6',
			Digit7 = '7',
			Digit8 = '8',
			Digit9 = '9',
			Colon = ':',
			SemiColon = ';',
			Less = '<',
			Equals = '=',
			Greater = '>',
			QuestionMark = '?',
			At = '@',

			/*
				Skip upper-case letters
			*/

			LeftBracket = '[',
			BackSlash = '\\',
			RightBracket = ']',
			Caret = '^',
			Underscore = '_',
			Backquote = '`',
			a = 'a',
			b = 'b',
			c = 'c',
			d = 'd',
			e = 'e',
			f = 'f',
			g = 'g',
			h = 'h',
			i = 'i',
			j = 'j',
			k = 'k',
			l = 'l',
			m = 'm',
			n = 'n',
			o = 'o',
			p = 'p',
			q = 'q',
			r = 'r',
			s = 's',
			t = 't',
			u = 'u',
			v = 'v',
			w = 'w',
			x = 'x',
			y = 'y',
			z = 'z',

			CapsLock = 57 | (1 << 30),

			F1 = 58 | (1 << 30),
			F2 = 59 | (1 << 30),
			F3 = 60 | (1 << 30),
			F4 = 61 | (1 << 30),
			F5 = 62 | (1 << 30),
			F6 = 63 | (1 << 30),
			F7 = 64 | (1 << 30),
			F8 = 65 | (1 << 30),
			F9 = 66 | (1 << 30),
			F10 = 67 | (1 << 30),
			F11 = 68 | (1 << 30),
			F12 = 69 | (1 << 30),

			PrintScreen = 70 | (1 << 30),
			ScrollLock = 71 | (1 << 30),
			Pause = 72 | (1 << 30),
			Insert = 73 | (1 << 30),
			Home = 74 | (1 << 30),
			PageUp = 75 | (1 << 30),
			Delete = '\177',
			End = 77 | (1 << 30),
			PageDown = 78 | (1 << 30),
			Right = 79 | (1 << 30),
			Left = 80 | (1 << 30),
			Down = 81 | (1 << 30),
			Up = 82 | (1 << 30),

			NumLockClear = 83 | (1 << 30),
			NumPad_Divide = 84 | (1 << 30),
			NumPad_Multiply = 85 | (1 << 30),
			NumPad_Minus = 86 | (1 << 30),
			NumPad_Plus = 87 | (1 << 30),
			NumPad_Enter = 88 | (1 << 30),
			NumPad_1 = 89 | (1 << 30),
			NumPad_2 = 90 | (1 << 30),
			NumPad_3 = 91 | (1 << 30),
			NumPad_4 = 92 | (1 << 30),
			NumPad_5 = 93 | (1 << 30),
			NumPad_6 = 94 | (1 << 30),
			NumPad_7 = 95 | (1 << 30),
			NumPad_8 = 96 | (1 << 30),
			NumPad_9 = 97 | (1 << 30),
			NumPad_0 = 98 | (1 << 30),
			NumPad_Period = 99 | (1 << 30),

			AltErase = 153 | (1 << 30),

			LeftCtrl = 224 | (1 << 30),
			LeftShift = 225 | (1 << 30),
			LeftAlt = 226 | (1 << 30),
			LeftSuper = 227 | (1 << 30),
			RightCtrl = 228 | (1 << 30),
			RightShift = 229 | (1 << 30),
			RightAlt = 230 | (1 << 30),
			RightSuper = 231 | (1 << 30),
		};
	}
}
