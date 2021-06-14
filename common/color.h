/*************************************************************************
	> File Name: ../common/color.h
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 20时52分14秒
 ************************************************************************/

#ifndef _COLOR_H
#define _COLOR_H

#define COLOR(a, b) "\033[0;" #b "m" a "\033[0m"
#define COLOR_HL(a, b) "\033[1;" #b "m" a "\033[0m"

#define BLACK(a) COLOR(a, 30)
#define RED(a) COLOR(a, 31)
#define GREEN(a) COLOR(a, 32)
#define YELLOW(a) COLOR(a, 33)
#define BLUE(a) COLOR(a, 34)
#define PINK(a) COLOR(a, 35)

#define BLACK_HL(a) COLOR_HL(a, 30)
#define RED_HL(a) COLOR_HL(a, 31)
#define GREEN_HL(a) COLOR_HL(a, 32)
#define YELLOW_HL(a) COLOR_HL(a, 33)
#define BLUE_HL(a) COLOR_HL(a, 34)
#define PINK_HL(a) COLOR_HL(a, 35)


#endif
