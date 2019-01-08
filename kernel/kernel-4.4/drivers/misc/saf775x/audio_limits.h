/*
 * audio_limits.h -- SAF775X Soc Audio driver Audio Controls defination
 *
 * Copyright (c) 2014-2016 NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

static int saf775x_vol_level[24][3] = {
/* vol_main1, vol_main2, mute
    min = 0, max = 23 */
{0x07ff, 0x0080, 0x07ff},
{0x07ff, 0x008F, 0x07ff},
{0x07ff, 0x00A1, 0x07ff},
{0x07ff, 0x00B4, 0x07ff},
{0x07ff, 0x00E3, 0x07ff},
{0x07ff, 0x00FF, 0x07ff},
{0x07ff, 0x011E, 0x07ff},
{0x07ff, 0x0141, 0x07ff},
{0x07ff, 0x0168, 0x07ff},
{0x07ff, 0x0194, 0x07ff},
{0x07ff, 0x01C6, 0x07ff},
{0x07ff, 0x01FD, 0x07ff},
{0x07ff, 0x023B, 0x07ff},
{0x07ff, 0x0281, 0x07ff},
{0x07ff, 0x02CF, 0x07ff},
{0x07ff, 0x0327, 0x07ff},
{0x07ff, 0x038A, 0x07ff},
{0x07ff, 0x03F9, 0x07ff},
{0x07ff, 0x0475, 0x07ff},
{0x07ff, 0x0500, 0x07ff},
{0x07ff, 0x059C, 0x07ff},
{0x07ff, 0x064B, 0x07ff},
{0x07ff, 0x0710, 0x07ff},
{0x07ff, 0x07ED, 0x07ff},
};

static unsigned int saf775x_bal_level[12][1] = {
/* level, bal_value
   min = 0, max = 11 */
{0x07ff},
{0x0487},
{0x0288},
{0x017C},
{0x00CC},
{0x0090},
{0x0040},
{0x0014},
{0x0014},
{0x0006},
{0x0002},
{0x0000},
};

static int saf775x_bas_level[][1] = {
/* min = -14, max = 24*/
{0x0def},
{0x0e83},
{0x0eec},
{0x0f3f},
{0x0f81},
{0x0fab},
{0x0fdf},
{0x0000},
{0x0021},
{0x004a},
{0x007f},
{0x00c1},
{0x0114},
{0x017d},
{0x0201},
{0x02a8},
{0x0378},
{0x0480},
{0x05cb},
};


static int saf775x_sign_level[][1] = {
/* min = 0, max = 1*/
{0x07ff},
{0x0800},
};

static int saf775x_mute_level[][1] = {
/* min = 0, max = 1*/
{0x07ff},
{0x0000},
};

static int saf775x_mute_att_level[][1] = {
/* min = 10, max = 100*/
{0x6d60},
{0x36b0},
{0x2475},
{0x1b58},
{0x15e0},
{0x123a},
{0x0fa0},
{0x0dac},
{0x0c27},
{0x0af0},
};

static int saf775x_mid_level[][1] = {
/* min = -14, max = 14*/
{0x0fb1},
{0x0fc1},
{0x0fc8},
{0x0fd1},
{0x0fdb},
{0x0fe6},
{0x0ff3},
{0x0000},
{0x0021},
{0x004a},
{0x007f},
{0x00c1},
{0x0114},
{0x017d},
{0x0201},
};

