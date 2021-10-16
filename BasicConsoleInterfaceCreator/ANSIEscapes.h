#pragma once

#include "pch.h"

#define ESC "\033"
#define CSI "["

#define CUU ESC CSI "%d" "A"
#define CUD ESC CSI "%d" "B"
#define CUF ESC CSI "%d" "C"
#define CUB ESC CSI "%d" "D"

#define CNL ESC CSI "%d" "E"
#define CPL ESC CSI "%d" "F"

#define CHA ESC CSI "%d" "G"

#define CUP ESC CSI "%d;%d" "H"

#define ED ESC CSI "%d" "J"
enum EDValuesEnum
{
	CURSOR_TO_END_SCREEN = 0,
	CURSOR_TO_BEGINNING_SCREEN = 1,
	ENTIRE_SCREEN = 2,
	ENTIRE_SCREEN_DELETE_LINES = 3
};

#define EL ESC CSI "%d" "K"
enum ELValuesEnum
{
	CURSOR_TO_END_LINE = 0,
	CURSOR_TO_BEGINNING_LINE = 1,
	ENTIRE_LINE = 2
};

#define SU ESC CSI "%d" "S"
#define SD ESC CSI "%d" "T"

#define HVP ESC CSI "%d;%d" "f"

#define SGR ESC CSI "%s" "m"

#define DSR ESC CSI "6" "n"

void cursorUp(int n = 1);
void cursorDown(int n = 1);
void cursorForward(int n = 1);
void cursorBack(int n = 1);

void cursorNextLine(int n = 1);
void cursorPreviousLine(int n = 1);

void cursorHorizontalAbsolute(int n = 1);

void cursorPosition(int n = 1, int m = 1);

void eraseDisplay(EDValuesEnum val = ENTIRE_SCREEN);
void eraseLine(ELValuesEnum val = ENTIRE_LINE);

void scroolUp(int n = 1);
void scroolDown(int n = 1);

void horizontalVerticalPosition(int n = 1, int m = 1);

void selectGraphicRendition(const char* n);

void deviceStatusReport(int* n = NULL, int* m = NULL);