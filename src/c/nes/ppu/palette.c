/***************************************************************************
 *   Copyright (C) 2006-2009 by Dead_Body   *
 *   jamesholodnak@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//palette generator borrowed from nintendulator

#include <math.h>

#include "defines.h"
#include "palette.h"
#include "nes/nes.h"
#include "system/video.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif


/* measurement by Chris Covell */
/*
const float emphasis_factor[8][3]={
   {1.0,  1.0,  1.0},
   {1.239,0.915,0.743},
   {0.794,1.086,0.882},
   {1.019,0.98, 0.653},
   {0.905,1.026,1.277},
   {1.023,0.908,0.979},
   {0.741,0.987,1.001},
   {0.75, 0.75, 0.75}
};
*/

/* measurement by Quietust */
const double emphasis_factor[8][3]={
   {1.00, 1.00, 1.00},
   {1.00, 0.80, 0.81},
   {0.78, 0.94, 0.66},
   {0.79, 0.77, 0.63},
   {0.82, 0.83, 1.12},
   {0.81, 0.71, 0.87},
   {0.68, 0.79, 0.79},
   {0.70, 0.70, 0.70}
};

palentry_t ronipalette[64] = //palette by roni, the default
	{
	{0x75,0x75,0x75},{0x27,0x1b,0x8f},{0x00,0x00,0xab},{0x47,0x00,0x9f}, //0x00 - 0x03
	{0x8f,0x00,0x77},{0xab,0x00,0x13},{0xa7,0x00,0x00},{0x7f,0x0b,0x00}, //0x04 - 0x07
	{0x43,0x2f,0x00},{0x00,0x47,0x00},{0x00,0x51,0x00},{0x00,0x3f,0x17}, //0x08 - 0x0b
	{0x1b,0x3f,0x5f},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00}, //0x0c - 0x0f
	{0xbc,0xbc,0xbc},{0x00,0x73,0xef},{0x23,0x3b,0xef},{0x83,0x00,0xf3}, //0x10 - 0x13
	{0xbf,0x00,0xbf},{0xe7,0x00,0x5b},{0xdb,0x2b,0x00},{0xcb,0x4f,0x0f}, //0x14 - 0x17
	{0x8b,0x73,0x00},{0x00,0x97,0x00},{0x00,0xab,0x00},{0x00,0x93,0x3b}, //0x18 - 0x1b
	{0x00,0x83,0x8b},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00}, //0x1c - 0x1f
	{0xff,0xff,0xff},{0x3f,0xbf,0xff},{0x5f,0x97,0xff},{0xa7,0x8b,0xfd}, //0x20 - 0x23
	{0xf7,0x7b,0xff},{0xff,0x77,0xb7},{0xff,0x77,0x63},{0xff,0x9b,0x3b}, //0x24 - 0x27
	{0xf3,0xbf,0x3f},{0x83,0xd3,0x13},{0x4f,0xdf,0x4b},{0x58,0xf8,0x98}, //0x28 - 0x2b
	{0x00,0xeb,0xdb},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00}, //0x2c - 0x2f
	{0xff,0xff,0xff},{0xab,0xe7,0xff},{0xc7,0xd7,0xff},{0xd7,0xcb,0xff}, //0x30 - 0x33
	{0xff,0xc7,0xff},{0xff,0xc7,0xdb},{0xff,0xbf,0xb3},{0xff,0xdb,0xab}, //0x34 - 0x37
	{0xe1,0xe7,0xa3},{0xe3,0xff,0xa3},{0xab,0xf3,0xbf},{0xb3,0xff,0xcf}, //0x38 - 0x3b
	{0x9f,0xff,0xf3},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00}  //0x3c - 0x3f
	};

palentry_t warmpalette[256] = //warm.pal, ive had it for 10 years, dont remember where it came from
	{
	{0x7f, 0x7f, 0x7f}, {0x00, 0x00, 0xff}, {0x00, 0x00, 0xbf}, {0x47, 0x2b, 0xbf},
	{0x97, 0x00, 0x87}, {0xab, 0x00, 0x23}, {0xab, 0x13, 0x00}, {0x8b, 0x17, 0x00},
	{0x53, 0x30, 0x00}, {0x00, 0x78, 0x00}, {0x00, 0x6b, 0x00}, {0x00, 0x5b, 0x00},
	{0x00, 0x43, 0x58}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0xbf, 0xbf, 0xbf}, {0x00, 0x78, 0xf8}, {0x00, 0x58, 0xf8}, {0x6b, 0x47, 0xff},
	{0xdb, 0x00, 0xcd}, {0xe7, 0x00, 0x5b}, {0xf8, 0x38, 0x00}, {0xe7, 0x5f, 0x13},
	{0xaf, 0x7f, 0x00}, {0x00, 0xb8, 0x00}, {0x00, 0xab, 0x00}, {0x00, 0xab, 0x47},
	{0x00, 0x8b, 0x8b}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0xf8, 0xf8, 0xf8}, {0x3f, 0xbf, 0xff}, {0x6b, 0x88, 0xff}, {0x98, 0x78, 0xf8},
	{0xf8, 0x78, 0xf8}, {0xf8, 0x58, 0x98}, {0xf8, 0x78, 0x58}, {0xff, 0xa3, 0x47},
	{0xf8, 0xb8, 0x00}, {0xb8, 0xf8, 0x18}, {0x5b, 0xdb, 0x57}, {0x58, 0xf8, 0x98},
	{0x00, 0xeb, 0xdb}, {0x78, 0x78, 0x78}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0xff, 0xff, 0xff}, {0xa7, 0xe7, 0xff}, {0xb8, 0xb8, 0xf8}, {0xd8, 0xb8, 0xf8},
	{0xf8, 0xb8, 0xf8}, {0xfb, 0xa7, 0xc3}, {0xf0, 0xd0, 0xb0}, {0xff, 0xe3, 0xab},
	{0xfb, 0xdb, 0x7b}, {0xd8, 0xf8, 0x78}, {0xb8, 0xf8, 0xb8}, {0xb8, 0xf8, 0xd8},
	{0x00, 0xff, 0xff}, {0xf8, 0xd8, 0xf8}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x68, 0x68, 0x68}, {0x00, 0x40, 0x80}, {0x00, 0x00, 0x80}, {0x40, 0x00, 0x80},
	{0x80, 0x00, 0x80}, {0x80, 0x00, 0x40}, {0x80, 0x00, 0x00}, {0x55, 0x00, 0x00},
	{0x40, 0x40, 0x00}, {0x33, 0x50, 0x00}, {0x00, 0x50, 0x00}, {0x00, 0x50, 0x00},
	{0x00, 0x40, 0x40}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x98, 0x98, 0x98}, {0x00, 0x80, 0xc0}, {0x40, 0x40, 0xc0}, {0x80, 0x00, 0xc0},
	{0xc0, 0x00, 0xc0}, {0xc0, 0x00, 0x80}, {0xc0, 0x20, 0x20}, {0xc0, 0x40, 0x00},
	{0x80, 0x80, 0x00}, {0x55, 0x80, 0x00}, {0x00, 0x80, 0x00}, {0x33, 0x80, 0x33},
	{0x00, 0x80, 0x80}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0xd0, 0xd0, 0xd0}, {0x40, 0xc0, 0xff}, {0x80, 0x80, 0xff}, {0xc0, 0x40, 0xff},
	{0xff, 0x00, 0xff}, {0xff, 0x40, 0xc0}, {0xff, 0x50, 0x50}, {0xff, 0x80, 0x40},
	{0xc0, 0xc0, 0x00}, {0x80, 0xc0, 0x00}, {0x00, 0xc0, 0x00}, {0x55, 0xc0, 0x55},
	{0x00, 0xc0, 0xc0}, {0x33, 0x33, 0x33}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0xff, 0xff, 0xff}, {0x80, 0xff, 0xff}, {0xc0, 0xc0, 0xff}, {0xff, 0x80, 0xff},
	{0xff, 0x40, 0xff}, {0xff, 0x80, 0xff}, {0xff, 0x80, 0x80}, {0xff, 0xc0, 0x80},
	{0xff, 0xff, 0x40}, {0xc0, 0xff, 0x40}, {0x40, 0xff, 0x40}, {0xaa, 0xff, 0xaa},
	{0x40, 0xff, 0xff}, {0x99, 0x99, 0x99}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
	{0x98, 0x98, 0x98}, {0x18, 0x58, 0xf8}, {0x18, 0x38, 0xf8}, {0x58, 0x38, 0xf8},
	{0xf8, 0x18, 0xf8}, {0xf8, 0x18, 0x58}, {0xf8, 0x18, 0x00}, {0xd8, 0x18, 0x00},
	{0xb8, 0x58, 0x00}, {0x00, 0x78, 0x00}, {0x00, 0x98, 0x00}, {0x00, 0x78, 0x18},
	{0x00, 0x78, 0xb8}, {0x00, 0x00, 0x00}, {0x18, 0x18, 0x18}, {0x18, 0x18, 0x18},
	{0xd8, 0xd8, 0xd8}, {0x00, 0x78, 0xf8}, {0x00, 0x58, 0xf8}, {0x58, 0x58, 0xf8},
	{0xf8, 0x38, 0xf8}, {0xf8, 0x38, 0x58}, {0xf8, 0x38, 0x00}, {0xf8, 0x58, 0x00},
	{0x60, 0xb0, 0x00}, {0x00, 0xb8, 0x00}, {0x00, 0xd8, 0x00}, {0x18, 0xd8, 0x58},
	{0x00, 0xb8, 0xf8}, {0x18, 0x18, 0x18}, {0x18, 0x18, 0x18}, {0x18, 0x18, 0x18},
	{0xf8, 0xf8, 0xf8}, {0x00, 0x98, 0xf8}, {0x78, 0x78, 0xf8}, {0x98, 0x78, 0xf8},
	{0xf8, 0x78, 0xf8}, {0xf8, 0x58, 0x98}, {0xf8, 0x78, 0x58}, {0xf8, 0xb8, 0x00},
	{0xf8, 0xb8, 0x00}, {0xb8, 0xf8, 0x18}, {0x78, 0xf8, 0x38}, {0x58, 0xf8, 0x98},
	{0x38, 0xd8, 0xf8}, {0x78, 0x78, 0x78}, {0x18, 0x18, 0x18}, {0x18, 0x18, 0x18},
	{0xf8, 0xf8, 0xf8}, {0x78, 0xb8, 0xf8}, {0xb8, 0xb8, 0xf8}, {0xd8, 0xb8, 0xf8},
	{0xf8, 0xb8, 0xf8}, {0xf8, 0x98, 0xb8}, {0xf8, 0xb8, 0x78}, {0xf8, 0xb8, 0x38},
	{0xf8, 0xb8, 0x58}, {0xd8, 0xf8, 0x78}, {0xb8, 0xf8, 0xb8}, {0xb8, 0xf8, 0xd8},
	{0x98, 0xd8, 0xf8}, {0xf8, 0xd8, 0xf8}, {0x18, 0x18, 0x18}, {0x00, 0x00, 0x00}
	};

static palentry_t palettes[8][64];
palentry_t *palette;   //palette used by emulator

void palette_set(palentry_t *p)  //set palette from array
{
	int i,j;

	for(j=0;j<8;j++) {
		for(i=0;i<64;i++) {
			palettes[j][i].r = (u8)((double)p[i].r * emphasis_factor[j][0]);
			palettes[j][i].g = (u8)((double)p[i].g * emphasis_factor[j][1]);
			palettes[j][i].b = (u8)((double)p[i].b * emphasis_factor[j][2]);
		}
	}
	palette = palettes[0];
}

#define CLIP(x,min,max) (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))

static	int	getPhase (double *wave)
{
	double max = -999, min = 999;
	double amp, offset;
	double angle = 0, base;
	int i, j, k;
	for (i = 0; i < 12; i++)
	{
		if (wave[i] > max)
			max = wave[i];
		if (wave[i] < min)
			min = wave[i];
	}
	amp = (max - min) / 2;
	offset = (max + min) / 2;

	for (k = 0; k < 3; k++)
	{
		double error[12], curerror = 0;
		double segsize = 360;
		for (i = 0; i <= k; i++)
			segsize /= 12.0;

		for (j = 0; j < 12; j++)
		{
			error[j] = 0;
			for (i = 0; i < 12; i++)
				error[j] += fabs((amp * sin((i * 30 + j * segsize + angle) * M_PI / 180.0) + offset) - wave[i]);
			curerror += error[j];
		}
		base = 0;
		for (j = 0; j < 12; j++)
		{
			if (error[j] < curerror)
			{
				base = j * segsize;
				curerror = error[j];
			}
		}
		angle += base;
	}

	return (int)angle;
}

void GenerateNTSC(int hue,int sat)
{
	const double black = 0.519;
	const double white = 1.443;
	const double voltage[2][4] = {{1.094,1.506,1.962,1.962},{0.350,0.519,0.962,1.550}};

	const char phases[12][12] = {
		{1,1,1,1,1,1,0,0,0,0,0,0},
		{1,1,1,1,1,0,0,0,0,0,0,1},	// blue
		{1,1,1,1,0,0,0,0,0,0,1,1},
		{1,1,1,0,0,0,0,0,0,1,1,1},	// magenta
		{1,1,0,0,0,0,0,0,1,1,1,1},
		{1,0,0,0,0,0,0,1,1,1,1,1},	// red
		{0,0,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,1,1,1,1,1,1,0},	// yellow
		{0,0,0,0,1,1,1,1,1,1,0,0},
		{0,0,0,1,1,1,1,1,1,0,0,0},	// green
		{0,0,1,1,1,1,1,1,0,0,0,0},
		{0,1,1,1,1,1,1,0,0,0,0,0},	// cyan
	};
	const char emphasis[8][12] = {
		{0,0,0,0,0,0,0,0,0,0,0,0},	// none
		{0,1,1,1,1,1,1,0,0,0,0,0},	// red
		{1,1,1,0,0,0,0,0,0,1,1,1},	// green
		{1,1,1,1,1,1,1,0,0,1,1,1},	// yellow
		{0,0,0,0,0,1,1,1,1,1,1,0},	// blue
		{0,1,1,1,1,1,1,1,1,1,1,0},	// magenta
		{1,1,1,0,0,1,1,1,1,1,1,1},	// cyan
		{1,1,1,1,1,1,1,1,1,1,1,1}	// all
	};

	int i, x, y, z;
	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 4; y++)
		{
			for (z = 0; z < 16; z++)
			{
				double wave[12];
				double Y, I, Q;
				double R, G, B;

				double H = 0, S = 0;

				for (i = 0; i < 12; i++)
				{
					if (z == 0)
						wave[i] = voltage[0][y];
					else if (z < 13)
						wave[i] = phases[z-1][i] ? voltage[0][y] : voltage[1][y];
					else if (z == 13)
						wave[i] = voltage[1][y];
					else	wave[i] = black;
					if ((emphasis[x][i]) && (z < 14))
						wave[i] = wave[i] * 0.75;
				}

				Y = 0.0; S = 0;
				for (i = 0; i < 12; i++)
					Y += wave[i] / 12.0;
				for (i = 0; i < 12; i++)
					S += (wave[i] - Y) * (wave[i] - Y);
				Y = (Y - black) / white;
				S = S / white;	// don't remove black offset, since this is already relative
				S = sqrt(S / 12.0) * sat / 50.0;

				H = M_PI * (270 + getPhase(wave) + hue) / 180.0;

				I = S * sin(H);
				Q = S * cos(H);

				R = Y + 0.956 * I + 0.621 * Q;
				G = Y - 0.272 * I - 0.647 * Q;
				B = Y - 1.107 * I + 1.705 * Q;

				R *= 256;
				G *= 256;
				B *= 256;

				palettes[x][(y << 4) | z].r = (unsigned char)CLIP(R,0,255);
				palettes[x][(y << 4) | z].g = (unsigned char)CLIP(G,0,255);
				palettes[x][(y << 4) | z].b = (unsigned char)CLIP(B,0,255);
			}
		}
	}
}

void palette_generate(int hue,int sat)       //generate nes palette
{
	GenerateNTSC(hue,sat);
	palette = palettes[0];
}

void palette_setactive(int n)
{
	int i;

	palette = palettes[n];
	for(i=0;i<0x20;i++)
		video_setpalentry(i,palette[nes->pal[i]].r,palette[nes->pal[i]].g,palette[nes->pal[i]].b);
//	log_message("active palette set to palette %d\n",n);
}