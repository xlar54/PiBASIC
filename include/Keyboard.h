// https://github.com/syin2/openthread/blob/a9f42768ec221380f42bfd311bc68e784b2163a6/third_party/silabs/gecko_sdk_suite/v1.0/protocol/flex_1.1/connect/plugins/usb/usb-hidkbd/usbkbdscancodes.c

#ifndef Keyboard_H
#define Keyboard_H
#include "Singleton.h"
#include <stdio.h>

extern "C"
{
#include <uspi/usbkeyboard.h>
}

#define MAX_KEYS 0x7f

#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA 0x80

#define KEY_A 			0x04 // Keyboard a and A
#define KEY_B 			0x05 // Keyboard b and B
#define KEY_C 			0x06 // Keyboard c and C
#define KEY_D 			0x07 // Keyboard d and D
#define KEY_E 			0x08 // Keyboard e and E
#define KEY_F 			0x09 // Keyboard f and F
#define KEY_G 			0x0a // Keyboard g and G
#define KEY_H 			0x0b // Keyboard h and H
#define KEY_I 			0x0c // Keyboard i and I
#define KEY_J 			0x0d // Keyboard j and J
#define KEY_K 			0x0e // Keyboard k and K
#define KEY_L 			0x0f // Keyboard l and L
#define KEY_M 			0x10 // Keyboard m and M
#define KEY_N 			0x11 // Keyboard n and N
#define KEY_O 			0x12 // Keyboard o and O
#define KEY_P 			0x13 // Keyboard p and P
#define KEY_Q 			0x14 // Keyboard q and Q
#define KEY_R 			0x15 // Keyboard r and R
#define KEY_S 			0x16 // Keyboard s and S
#define KEY_T 			0x17 // Keyboard t and T
#define KEY_U 			0x18 // Keyboard u and U
#define KEY_V 			0x19 // Keyboard v and V
#define KEY_W 			0x1a // Keyboard w and W
#define KEY_X 			0x1b // Keyboard x and X
#define KEY_Y 			0x1c // Keyboard y and Y
#define KEY_Z 			0x1d // Keyboard z and Z
#define KEY_1 			0x1e // Keyboard 1 and !
#define KEY_2 			0x1f // Keyboard 2 and @
#define KEY_3 			0x20 // Keyboard 3 and #
#define KEY_4 			0x21 // Keyboard 4 and $
#define KEY_5 			0x22 // Keyboard 5 and %
#define KEY_6 			0x23 // Keyboard 6 and ^
#define KEY_7 			0x24 // Keyboard 7 and &
#define KEY_8 			0x25 // Keyboard 8 and *
#define KEY_9 			0x26 // Keyboard 9 and (
#define KEY_0 			0x27 // Keyboard 0 and )
#define KEY_ENTER 		0x28 // Keyboard Return (ENTER)
#define KEY_ESC 		0x29 // Keyboard ESCAPE
#define KEY_BACKSPACE 	0x2a // Keyboard DELETE (Backspace)
#define KEY_TAB 		0x2b // Keyboard Tab
#define KEY_SPACE 		0x2c // Keyboard Spacebar
#define KEY_MINUS 		0x2d // Keyboard - and _
#define KEY_EQUAL 		0x2e // Keyboard = and +
#define KEY_LEFTBRACE 	0x2f // Keyboard [ and {
#define KEY_RIGHTBRACE 	0x30 // Keyboard ] and }
#define KEY_BACKSLASH 	0x31 // Keyboard \ and |
#define KEY_HASHTILDE 	0x32 // Keyboard Non-US # and ~
#define KEY_SEMICOLON 	0x33 // Keyboard ; and :
#define KEY_APOSTROPHE 	0x34 // Keyboard ' and "
#define KEY_GRAVE 		0x35 // Keyboard ` and ~
#define KEY_COMMA 		0x36 // Keyboard , and <
#define KEY_DOT 		0x37 // Keyboard . and >
#define KEY_SLASH 		0x38 // Keyboard / and ?
#define KEY_CAPSLOCK 	0x39 // Keyboard Caps Lock
#define KEY_F1 			0x3a // Keyboard F1
#define KEY_F2 			0x3b // Keyboard F2
#define KEY_F3 			0x3c // Keyboard F3
#define KEY_F4 			0x3d // Keyboard F4
#define KEY_F5 			0x3e // Keyboard F5
#define KEY_F6 			0x3f // Keyboard F6
#define KEY_F7 			0x40 // Keyboard F7
#define KEY_F8 			0x41 // Keyboard F8
#define KEY_F9 			0x42 // Keyboard F9
#define KEY_F10 		0x43 // Keyboard F10
#define KEY_F11 		0x44 // Keyboard F11
#define KEY_F12 		0x45 // Keyboard F12
#define KEY_SYSRQ 		0x46 // Keyboard Print Screen
#define KEY_SCROLLLOCK 	0x47 // Keyboard Scroll Lock
#define KEY_PAUSE 		0x48 // Keyboard Pause
#define KEY_INSERT 		0x49 // Keyboard Insert
#define KEY_HOME 		0x4a // Keyboard Home
#define KEY_PAGEUP 		0x4b // Keyboard Page Up
#define KEY_DELETE 		0x4c // Keyboard Delete Forward
#define KEY_END 		0x4d // Keyboard End
#define KEY_PAGEDOWN 	0x4e // Keyboard Page Down
#define KEY_RIGHT 		0x4f // Keyboard Right Arrow
#define KEY_LEFT 		0x50 // Keyboard Left Arrow
#define KEY_DOWN 		0x51 // Keyboard Down Arrow
#define KEY_UP 			0x52 // Keyboard Up Arrow
#define KEY_NUMLOCK 	0x53 // Keyboard Num Lock and Clear
#define KEY_KPSLASH 	0x54 // Keypad /
#define KEY_KPASTERISK 	0x55 // Keypad *
#define KEY_KPMINUS 	0x56 // Keypad -
#define KEY_KPPLUS 		0x57 // Keypad +
#define KEY_KPENTER 	0x58 // Keypad ENTER
#define KEY_KP1 		0x59 // Keypad 1 and End
#define KEY_KP2 		0x5a // Keypad 2 and Down Arrow
#define KEY_KP3 		0x5b // Keypad 3 and PageDn
#define KEY_KP4 		0x5c // Keypad 4 and Left Arrow
#define KEY_KP5 		0x5d // Keypad 5
#define KEY_KP6 		0x5e // Keypad 6 and Right Arrow
#define KEY_KP7 		0x5f // Keypad 7 and Home
#define KEY_KP8 		0x60 // Keypad 8 and Up Arrow
#define KEY_KP9 		0x61 // Keypad 9 and Page Up
#define KEY_KP0 		0x62 // Keypad 0 and Insert
#define KEY_KPDOT 		0x63 // Keypad . and Delete
#define KEY_102ND 		0x64 // Keyboard Non-US \ and |
#define KEY_COMPOSE 	0x65 // Keyboard Application
#define KEY_POWER 		0x66 // Keyboard Power
#define KEY_KPEQUAL 	0x67 // Keypad =
#define KEY_F13 		0x68 // Keyboard F13
#define KEY_F14 		0x69 // Keyboard F14
#define KEY_F15 		0x6a // Keyboard F15
#define KEY_F16 		0x6b // Keyboard F16
#define KEY_F17 		0x6c // Keyboard F17
#define KEY_F18 		0x6d // Keyboard F18
#define KEY_F19 		0x6e // Keyboard F19
#define KEY_F20 		0x6f // Keyboard F20
#define KEY_F21 		0x70 // Keyboard F21
#define KEY_F22 		0x71 // Keyboard F22
#define KEY_F23 		0x72 // Keyboard F23
#define KEY_F24 		0x73 // Keyboard F24
#define KEY_OPEN 		0x74 // Keyboard Execute
#define KEY_HELP 		0x75 // Keyboard Help
#define KEY_PROPS 		0x76 // Keyboard Menu
#define KEY_FRONT 		0x77 // Keyboard Select
#define KEY_STOP 		0x78 // Keyboard Stop
#define KEY_AGAIN 		0x79 // Keyboard Again
#define KEY_UNDO 		0x7a // Keyboard Undo
#define KEY_CUT 		0x7b // Keyboard Cut
#define KEY_COPY 		0x7c // Keyboard Copy
#define KEY_PASTE 		0x7d // Keyboard Paste
#define KEY_FIND 		0x7e // Keyboard Find
#define KEY_MUTE 		0x7f // Keyboard Mute
#define KEY_VOLUMEUP 	0x80 // Keyboard Volume Up
#define KEY_VOLUMEDOWN 	0x81 // Keyboard Volume Down
#define KEY_LCKCAPSLOCK	0x82 // Keyboard Locking Caps Lock
#define KEY_LCKNUMLOCK 	0x83 // Keyboard Locking Num Lock
#define KEY_LCKSCRLLLCK	0x84 // Keyboard Locking Scroll Lock
#define KEY_KPCOMMA 	0x85 // Keypad Comma
#define KEY_KPEQUALSGN 	0x86 // Keypad Equal Sign
#define KEY_INTL1 		0x87 // Keyboard International1
#define KEY_INTL2 		0x88 // Keyboard International2
#define KEY_INTL3 		0x89 // Keyboard International3
#define KEY_INTL4 		0x8a // Keyboard International4
#define KEY_INTL5 		0x8b // Keyboard International5
#define KEY_INTL6 		0x8c // Keyboard International6
#define KEY_INTL7 		0x8d // Keyboard International7
#define KEY_INTL8 		0x8e // Keyboard International8
#define KEY_INTL9 		0x8f // Keyboard International9
#define KEY_LANG1 		0x90 // Keyboard LANG1
#define KEY_LANG2 		0x91 // Keyboard LANG2
#define KEY_LANG3 		0x92 // Keyboard LANG3
#define KEY_LANG4 		0x93 // Keyboard LANG4
#define KEY_LANG5 		0x94 // Keyboard LANG5
#define KEY_LANG6 		0x95 // Keyboard LANG6
#define KEY_LANG7 		0x96 // Keyboard LANG7
#define KEY_LANG8 		0x97 // Keyboard LANG8
#define KEY_LANG9 		0x98 // Keyboard LANG9
#define KEY_ALTERASE 	0x99 // Keyboard Alternate Erase
#define KEY_SYSREQ 		0x9a // Keyboard SysReq/Attention
#define KEY_CANCEL 		0x9b // Keyboard Cancel
#define KEY_CLEAR 		0x9c // Keyboard Clear
#define KEY_PRIOR 		0x9d // Keyboard Prior
#define KEY_ERTN 		0x9e // Keyboard Return
#define KEY_SEP 		0x9f // Keyboard Separator
#define KEY_OUT 		0xa0 // Keyboard Out
#define KEY_OPER 		0xa1 // Keyboard Oper
#define KEY_CLRAGAIN	0xa2 // Keyboard Clear/Again
#define KEY_CRSEL 		0xa3 // Keyboard CrSel/Props
#define KEY_EXCEL 		0xa4 // Keyboard ExSel
// 0xa5 - 0xaf = reserved
#define KEY_KP00 		0xb0 // Keypad 00
#define KEY_KP000 		0xb1 // Keypad 000
#define KEY_THOUSEP 	0xb2 // Thousands Separator
#define KEY_DECSEP 		0xb3 // Decimal Separator
#define KEY_CUNIT 		0xb4 // Currency Unit
#define KEY_CSUBUNIT 	0xb5 // Currency Sub-unit
#define KEY_KPLPAREN	0xb6 // Keypad (
#define KEY_KPRPAREN 	0xb7 // Keypad )
#define KEY_KPLBRC 		0xb8 // Keypad {
#define KEY_KPRBRC 		0xb9 // Keypad }
#define KEY_KPTAB 		0xba // Keypad Tab
#define KEY_KPBKSPC 	0xbb // Keypad Backspace
#define KEY_KPA 		0xbc // Keypad A
#define KEY_KPB 		0xbd // Keypad B
#define KEY_KPC 		0xbe // Keypad C
#define KEY_KPD 		0xbf // Keypad D
#define KEY_KPE 		0xc0 // Keypad E
#define KEY_KPF 		0xc1 // Keypad F
#define KEY_KPXOR 		0xc2 // Keypad XOR
#define KEY_KPCARAT 	0xc3 // Keypad ^
#define KEY_KPPCT 		0xc4 // Keypad %
#define KEY_KPLESS 		0xc5 // Keypad <
#define KEY_KPGRTR 		0xc6 // Keypad >
#define KEY_KPBITAND 	0xc7 // Keypad &
#define KEY_KPBOOLAND 	0xc8 // Keypad &&
#define KEY_KPBITOR 	0xc9 // Keypad |
#define KEY_KPOR 		0xca // Keypad ||
#define KEY_KPCOLON 	0xcb // Keypad :
#define KEY_KPPOUND 	0xcc // Keypad #
#define KEY_KPSPACE 	0xcd // Keypad Space
#define KEY_KPAT 		0xce // Keypad @
#define KEY_KPEXCLAM 	0xcf // Keypad !
#define KEY_KPMEMSTORE 	0xd0 // Keypad Memory Store
#define KEY_KPMEMRECALL 0xd1 // Keypad Memory Recall
#define KEY_KPMEMCLR 	0xd2 // Keypad Memory Clear
#define KEY_KPMEMADD 	0xd3 // Keypad Memory Add
#define KEY_KPMEMSUB 	0xd4 // Keypad Memory Subtract
#define KEY_KPMEMMULT 	0xd5 // Keypad Memory Multiply
#define KEY_KPMEMDIV 	0xd6 // Keypad Memory Divide
#define KEY_KPPLSMIN 	0xd7 // Keypad +/-
#define KEY_KPCLR 		0xd8 // Keypad Clear
#define KEY_KPCLRENTRY 	0xd9 // Keypad Clear Entry
#define KEY_KPBINARY 	0xda // Keypad Binary
#define KEY_KPOCTAL 	0xdb // Keypad Octal
#define KEY_KPDECIMAL 	0xdc // Keypad Decimal
#define KEY_KPHEX 		0xdd // Keypad Hexadecimal
// 0xde - 0xdf = reserved
#define KEY_LEFTCTRL 	0xe0 // Keyboard Left Control
#define KEY_LEFTSHIFT 	0xe1 // Keyboard Left Shift
#define KEY_LEFTALT 	0xe2 // Keyboard Left Alt
#define KEY_LEFTMETA 	0xe3 // Keyboard Left GUI
#define KEY_RIGHTCTRL 	0xe4 // Keyboard Right Control
#define KEY_RIGHTSHIFT 	0xe5 // Keyboard Right Shift
#define KEY_RIGHTALT 	0xe6 // Keyboard Right Alt
#define KEY_RIGHTMETA 	0xe7 // Keyboard Right GUI
#define KEY_MEDIA_PLAYPAUSE 	0xe8
#define KEY_MEDIA_STOPCD 		0xe9
#define KEY_MEDIA_PREVIOUSSONG 	0xea
#define KEY_MEDIA_NEXTSONG 		0xeb
#define KEY_MEDIA_EJECTCD 		0xec
#define KEY_MEDIA_VOLUMEUP 		0xed
#define KEY_MEDIA_VOLUMEDOWN 	0xee
#define KEY_MEDIA_MUTE 			0xef
#define KEY_MEDIA_WWW 			0xf0
#define KEY_MEDIA_BACK 			0xf1
#define KEY_MEDIA_FORWARD 		0xf2
#define KEY_MEDIA_STOP 			0xf3
#define KEY_MEDIA_FIND 			0xf4
#define KEY_MEDIA_SCROLLUP 		0xf5
#define KEY_MEDIA_SCROLLDOWN 	0xf6
#define KEY_MEDIA_EDIT 			0xf7
#define KEY_MEDIA_SLEEP 		0xf8
#define KEY_MEDIA_COFFEE 		0xf9
#define KEY_MEDIA_REFRESH 		0xfa
#define KEY_MEDIA_CALC 			0xfb

class Keyboard : public Singleton<Keyboard>
{
protected:
	friend Singleton<Keyboard>;

	u8 modifier;
	volatile uint64_t keyStatus[2];
	volatile uint64_t keyStatusPrev[2];
	u32 keyRepeatCount[MAX_KEYS];
	u32 timer;
	//volatile bool dirty;
	volatile u32 updateCount;
	u32 updateCountLastRead;

uint8_t Scancode2Ascii[ 256 ] =
{
  /*  Dec  Hex Usage                                    */
  /*  ---------------------------------------           */
  0,    /*  0    00  Reserved (no event indicated)            */
  0,    /*  1    01  Keyboard ErrorRollOver                   */
  0,    /*  2    02  Keyboard POSTFail                        */
  0,    /*  3    03  Keyboard ErrorUndefined                  */
  'a',  /*  4    04  Keyboard a and A                         */
  'b',  /*  5    05  Keyboard b and B                         */
  'c',  /*  6    06  Keyboard c and C                         */
  'd',  /*  7    07  Keyboard d and D                         */
  'e',  /*  8    08  Keyboard e and E                         */
  'f',  /*  9    09  Keyboard f and F                         */
  'g',  /*  10   0A  Keyboard g and G                         */
  'h',  /*  11   0B  Keyboard h and H                         */
  'i',  /*  12   0C  Keyboard i and I                         */
  'j',  /*  13   0D  Keyboard j and J                         */
  'k',  /*  14   0E  Keyboard k and K                         */
  'l',  /*  15   0F  Keyboard l and L                         */
  'm',  /*  16   10  Keyboard m and M                         */
  'n',  /*  17   11  Keyboard n and N                         */
  'o',  /*  18   12  Keyboard o and O                         */
  'p',  /*  19   13  Keyboard p and P                         */
  'q',  /*  20   14  Keyboard q and Q                         */
  'r',  /*  21   15  Keyboard r and R                         */
  's',  /*  22   16  Keyboard s and S                         */
  't',  /*  23   17  Keyboard t and T                         */
  'u',  /*  24   18  Keyboard u and U                         */
  'v',  /*  25   19  Keyboard v and V                         */
  'w',  /*  26   1A  Keyboard w and W                         */
  'x',  /*  27   1B  Keyboard x and X                         */
  'y',  /*  28   1C  Keyboard y and Y                         */
  'z',  /*  29   1D  Keyboard z and Z                         */
  '1',  /*  30   1E  Keyboard 1 and !                         */
  '2',  /*  31   1F  Keyboard 2 and @                         */
  '3',  /*  32   20  Keyboard 3 and #                         */
  '4',  /*  33   21  Keyboard 4 and $                         */
  '5',  /*  34   22  Keyboard 5 and %                         */
  '6',  /*  35   23  Keyboard 6 and ^                         */
  '7',  /*  36   24  Keyboard 7 and &                         */
  '8',  /*  37   25  Keyboard 8 and *                         */
  '9',  /*  38   26  Keyboard 9 and (                         */
  '0',  /*  39   27  Keyboard 0 and )                         */
  '\n', /*  40   28  Keyboard Return(ENTER)                   */
  '\x1b', /*  41   29  Keyboard ESCAPE                          */
  '\b', /*  42   2A  Keyboard DELETE(Backspace)               */
  '\t', /*  43   2B  Keyboard Tab                             */
  ' ',  /*  44   2C  Keyboard Spacebar                        */
  '-',  /*  45   2D  Keyboard - and (underscore)              */
  '=',  /*  46   2E  Keyboard = and +                         */
  '[',  /*  47   2F  Keyboard [ and {                         */
  ']',  /*  48   30  Keyboard ] and }                         */
  '\\', /*  49   31  Keyboard \ and |                         */
  '#',  /*  50   32  Keyboard Non-US# and ~                   */
  ';',  /*  51   33  Keyboard ; and :                         */
  '\'', /*  52   34  Keyboard "english writing left single and double quotation mark" */
  '`',  /*  53   35  Keyboard Grave Accent ` and Tilde ~      */
  ',',  /*  54   36  Keyboard , and <                         */
  '.',  /*  55   37  Keyboard . and >                         */
  '/',  /*  56   38  Keyboard / and ?                         */
  0,    /*  57   39  Keyboard CapsLock                        */
  0,    /*  58   3A  Keyboard F1                              */
  0,    /*  59   3B  Keyboard F2                              */
  0,    /*  60   3C  Keyboard F3                              */
  0,    /*  61   3D  Keyboard F4                              */
  0,    /*  62   3E  Keyboard F5                              */
  0,    /*  63   3F  Keyboard F6                              */
  0,    /*  64   40  Keyboard F7                              */
  0,    /*  65   41  Keyboard F8                              */
  0,    /*  66   42  Keyboard F9                              */
  0,    /*  67   43  Keyboard F10                             */
  0,    /*  68   44  Keyboard F11                             */
  0,    /*  69   45  Keyboard F12                             */
  0,    /*  70   46  Keyboard PrintScreen                     */
  0,    /*  71   47  Keyboard ScrollLock                      */
  0,    /*  72   48  Keyboard Pause                           */
  0,    /*  73   49  Keyboard Insert                          */
  132,    /*  74   4A  Keyboard Home                            */
  0,    /*  75   4B  Keyboard PageUp                          */
  0,    /*  76   4C  Keyboard Delete Forward                  */
  0,    /*  77   4D  Keyboard End                             */
  0,    /*  78   4E  Keyboard PageDown                        */
  130,    /*  79   4F  Keyboard RightArrow                      */
  131,    /*  80   50  Keyboard LeftArrow                       */
  129,    /*  81   51  Keyboard DownArrow                       */
  128,    /*  82   52  Keyboard UpArrow                         */
  0,    /*  83   53  Keypad NumLock and Clear                 */
  '/',  /*  84   54  Keypad /                                 */
  '*',  /*  85   55  Keypad *                                 */
  '-',  /*  86   56  Keypad -                                 */
  '+',  /*  87   57  Keypad +                                 */
  '\r', /*  88   58  Keypad ENTER                             */
  '1',  /*  89   59  Keypad 1 and End                         */
  '2',  /*  90   5A  Keypad 2 and Down Arrow                  */
  '3',  /*  91   5B  Keypad 3 and PageDn                      */
  '4',  /*  92   5C  Keypad 4 and Left Arrow                  */
  '5',  /*  93   5D  Keypad 5                                 */
  '6',  /*  94   5E  Keypad 6 and Right Arrow                 */
  '7',  /*  95   5F  Keypad 7 and Home                        */
  '8',  /*  96   60  Keypad 8 and Up Arrow                    */
  '9',  /*  97   61  Keypad 9 and PageUp                      */
  '0',  /*  98   62  Keypad 0 and Insert                      */
  '.',  /*  99   63  Keypad . and Delete                      */
  '\\', /*  100  64  Keyboard Non-US\ and |                   */
  0,    /*  101  65  Keyboard Application                     */
  0,    /*  102  66  Keyboard Power                           */
  '=',  /*  103  67  Keypad =                                 */
  0,    /*  104  68  Keyboard F13                             */
  0,    /*  105  69  Keyboard F14                             */
  0,    /*  106  6A  Keyboard F15                             */
  0,    /*  107  6B  Keyboard F16                             */
  0,    /*  108  6C  Keyboard F17                             */
  0,    /*  109  6D  Keyboard F18                             */
  0,    /*  110  6E  Keyboard F19                             */
  0,    /*  111  6F  Keyboard F20                             */
  0,    /*  112  70  Keyboard F21                             */
  0,    /*  113  71  Keyboard F22                             */
  0,    /*  114  72  Keyboard F23                             */
  0,    /*  115  73  Keyboard F24                             */
  0,    /*  116  74  Keyboard Execute                         */
  0,    /*  117  75  Keyboard Help                            */
  0,    /*  118  76  Keyboard Menu                            */
  0,    /*  119  77  Keyboard Select                          */
  0,    /*  120  78  Keyboard Stop                            */
  0,    /*  121  79  Keyboard Again                           */
  0,    /*  122  7A  Keyboard Undo                            */
  0,    /*  123  7B  Keyboard Cut                             */
  0,    /*  124  7C  Keyboard Copy                            */
  0,    /*  125  7D  Keyboard Paste                           */
  0,    /*  126  7E  Keyboard Find                            */
  0,    /*  127  7F  Keyboard Mute                            */
  0,    /*  128  80  Keyboard Volume Up                       */
  0,    /*  129  81  Keyboard Volume Down                     */
  0,    /*  130  82  Keyboard Locking Caps Lock               */
  0,    /*  131  83  Keyboard Locking Num Lock                */
  0,    /*  132  84  Keyboard Locking Scroll Lock             */
  ',',  /*  133  85  Keypad Comma                             */
  '=',  /*  134  86  Keypad Equal Sign                        */
  0,    /*  135  87  Keyboard International 1                 */
  0,    /*  136  88  Keyboard International 2                 */
  0,    /*  137  89  Keyboard International 3                 */
  0,    /*  138  8A  Keyboard International 4                 */
  0,    /*  139  8B  Keyboard International 5                 */
  0,    /*  140  8C  Keyboard International 6                 */
  0,    /*  141  8D  Keyboard International 7                 */
  0,    /*  142  8E  Keyboard International 8                 */
  0,    /*  143  8F  Keyboard International 9                 */
  0,    /*  144  90  Keyboard LANG1                           */
  0,    /*  145  91  Keyboard LANG2                           */
  0,    /*  146  92  Keyboard LANG3                           */
  0,    /*  147  93  Keyboard LANG4                           */
  0,    /*  148  94  Keyboard LANG5                           */
  0,    /*  149  95  Keyboard LANG6                           */
  0,    /*  150  96  Keyboard LANG7                           */
  0,    /*  151  97  Keyboard LANG8                           */
  0,    /*  152  98  Keyboard LANG9                           */
  0,    /*  153  99  Keyboard AlternateErase                  */
  0,    /*  154  9A  Keyboard SysReq/Attention                */
  0,    /*  155  9B  Keyboard Cancel                          */
  0,    /*  156  9C  Keyboard Clear                           */
  0,    /*  157  9D  Keyboard Prior                           */
  '\n', /*  158  9E  Keyboard Return                          */
  0,    /*  159  9F  Keyboard Separator                       */
  0,    /*  160  A0  Keyboard Out                             */
  0,    /*  161  A1  Keyboard Oper                            */
  0,    /*  162  A2  Keyboard Clear/Again                     */
  0,    /*  163  A3  Keyboard CrSel/Props                     */
  0,    /*  164  A4  Keyboard ExSel                           */
  0,    /*  165-175  A5-AF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0,    /*  176  B0  Keypad 00                                */
  0,    /*  177  B1  Keypad 000                               */
  ',',  /*  178  B2  Thousands Separator                      */
  '.',  /*  179  B3  Decimal Separator                        */
  0,    /*  180  B4  Currency Unit                            */
  0,    /*  181  B5  Currency Sub-unit                        */
  '(',  /*  182  B6  Keypad (                                 */
  ')',  /*  183  B7  Keypad )                                 */
  '{',  /*  184  B8  Keypad {                                 */
  '}',  /*  185  B9  Keypad }                                 */
  0,    /*  186  BA  Keypad Tab                               */
  0,    /*  187  BB  Keypad Backspace                         */
  'A',  /*  188  BC  Keypad A                                 */
  'B',  /*  189  BD  Keypad B                                 */
  'C',  /*  190  BE  Keypad C                                 */
  'D',  /*  191  BF  Keypad D                                 */
  'E',  /*  192  C0  Keypad E                                 */
  'F',  /*  193  C1  Keypad F                                 */
  '^',  /*  194  C2  Keypad XOR                               */
  '^',  /*  195  C3  Keypad ^                                 */
  '%',  /*  196  C4  Keypad %                                 */
  '<',  /*  197  C5  Keypad <                                 */
  '>',  /*  198  C6  Keypad >                                 */
  '&',  /*  199  C7  Keypad &                                 */
  0,    /*  200  C8  Keypad &&                                */
  '|',  /*  201  C9  Keypad |                                 */
  0,    /*  202  CA  Keypad ||                                */
  ':',  /*  203  CB  Keypad :                                 */
  '#',  /*  204  CC  Keypad #                                 */
  ' ',  /*  205  CD  Keypad Space                             */
  '@',  /*  206  CE  Keypad @                                 */
  '!',  /*  207  CF  Keypad !                                 */
  0,    /*  208  D0  Keypad Memory Store                      */
  0,    /*  209  D1  Keypad Memory Recall                     */
  0,    /*  210  D2  Keypad Memory Clear                      */
  0,    /*  211  D3  Keypad Memory Add                        */
  0,    /*  212  D4  Keypad Memory Subtract                   */
  0,    /*  213  D5  Keypad Memory Multiply                   */
  0,    /*  214  D6  Keypad Memory Divide                     */
  0,    /*  215  D7  Keypad +/-                               */
  0,    /*  216  D8  Keypad Clear                             */
  0,    /*  217  D9  Keypad Clear Entry                       */
  0,    /*  218  DA  Keypad Binary                            */
  0,    /*  219  DB  Keypad Octal                             */
  0,    /*  220  DC  Keypad Decimal                           */
  0,    /*  221  DD  Keypad Hexadecimal                       */
  0, 0, /*  222-223  DE-DF Reserved                           */
  0,    /*  224  E0  Keyboard LeftControl                     */
  0,    /*  225  E1  Keyboard LeftShift                       */
  0,    /*  226  E2  Keyboard LeftAlt                         */
  0,    /*  227  E3  Keyboard Left GUI                        */
  0,    /*  228  E4  Keyboard RightControl                    */
  0,    /*  229  E5  Keyboard RightShift                      */
  0,    /*  230  E6  Keyboard RightAlt                        */
  0,    /*  231  E7  Keyboard Right GUI                       */
  0,    /*  232-255  E8-FF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t ShiftScancode2Ascii[ 256 ] =
{
  /*  Dec  Hex Usage                                    */
  /*  ---------------------------------------           */
  0,    /*  0    00  Reserved (no event indicated)            */
  0,    /*  1    01  Keyboard ErrorRollOver                   */
  0,    /*  2    02  Keyboard POSTFail                        */
  0,    /*  3    03  Keyboard ErrorUndefined                  */
  'A',  /*  4    04  Keyboard a and A                         */
  'B',  /*  5    05  Keyboard b and B                         */
  'C',  /*  6    06  Keyboard c and C                         */
  'D',  /*  7    07  Keyboard d and D                         */
  'E',  /*  8    08  Keyboard e and E                         */
  'F',  /*  9    09  Keyboard f and F                         */
  'G',  /*  10   0A  Keyboard g and G                         */
  'H',  /*  11   0B  Keyboard h and H                         */
  'I',  /*  12   0C  Keyboard i and I                         */
  'J',  /*  13   0D  Keyboard j and J                         */
  'K',  /*  14   0E  Keyboard k and K                         */
  'L',  /*  15   0F  Keyboard l and L                         */
  'M',  /*  16   10  Keyboard m and M                         */
  'N',  /*  17   11  Keyboard n and N                         */
  'O',  /*  18   12  Keyboard o and O                         */
  'P',  /*  19   13  Keyboard p and P                         */
  'Q',  /*  20   14  Keyboard q and Q                         */
  'R',  /*  21   15  Keyboard r and R                         */
  'S',  /*  22   16  Keyboard s and S                         */
  'T',  /*  23   17  Keyboard t and T                         */
  'U',  /*  24   18  Keyboard u and U                         */
  'V',  /*  25   19  Keyboard v and V                         */
  'W',  /*  26   1A  Keyboard w and W                         */
  'X',  /*  27   1B  Keyboard x and X                         */
  'Y',  /*  28   1C  Keyboard y and Y                         */
  'Z',  /*  29   1D  Keyboard z and Z                         */
  '!',  /*  30   1E  Keyboard 1 and !                         */
  '@',  /*  31   1F  Keyboard 2 and @                         */
  '#',  /*  32   20  Keyboard 3 and #                         */
  '$',  /*  33   21  Keyboard 4 and $                         */
  '%',  /*  34   22  Keyboard 5 and %                         */
  '^',  /*  35   23  Keyboard 6 and ^                         */
  '&',  /*  36   24  Keyboard 7 and &                         */
  '*',  /*  37   25  Keyboard 8 and *                         */
  '(',  /*  38   26  Keyboard 9 and (                         */
  ')',  /*  39   27  Keyboard 0 and )                         */
  '\n', /*  40   28  Keyboard Return(ENTER)                   */
  '\x1b', /*  41   29  Keyboard ESCAPE                          */
  '\b', /*  42   2A  Keyboard DELETE(Backspace)               */
  '\t', /*  43   2B  Keyboard Tab                             */
  ' ',  /*  44   2C  Keyboard Spacebar                        */
  '_',  /*  45   2D  Keyboard - and (underscore)              */
  '+',  /*  46   2E  Keyboard = and +                         */
  '{',  /*  47   2F  Keyboard [ and {                         */
  '}',  /*  48   30  Keyboard ] and }                         */
  '|',  /*  49   31  Keyboard \ and |                         */
  '~',  /*  50   32  Keyboard Non-US# and ~                   */
  ':',  /*  51   33  Keyboard ; and :                         */
  '"',  /*  52   34  Keyboard "english writing left single and double quotation mark" */
  '~',  /*  53   35  Keyboard Grave Accent ` and Tilde ~      */
  '<',  /*  54   36  Keyboard , and <                         */
  '>',  /*  55   37  Keyboard . and >                         */
  '?',  /*  56   38  Keyboard / and ?                         */
  0,    /*  57   39  Keyboard CapsLock                        */
  0,    /*  58   3A  Keyboard F1                              */
  0,    /*  59   3B  Keyboard F2                              */
  0,    /*  60   3C  Keyboard F3                              */
  0,    /*  61   3D  Keyboard F4                              */
  0,    /*  62   3E  Keyboard F5                              */
  0,    /*  63   3F  Keyboard F6                              */
  0,    /*  64   40  Keyboard F7                              */
  0,    /*  65   41  Keyboard F8                              */
  0,    /*  66   42  Keyboard F9                              */
  0,    /*  67   43  Keyboard F10                             */
  0,    /*  68   44  Keyboard F11                             */
  0,    /*  69   45  Keyboard F12                             */
  0,    /*  70   46  Keyboard PrintScreen                     */
  0,    /*  71   47  Keyboard ScrollLock                      */
  0,    /*  72   48  Keyboard Pause                           */
  0,    /*  73   49  Keyboard Insert                          */
  0,    /*  74   4A  Keyboard Home                            */
  0,    /*  75   4B  Keyboard PageUp                          */
  0,    /*  76   4C  Keyboard Delete Forward                  */
  0,    /*  77   4D  Keyboard End                             */
  0,    /*  78   4E  Keyboard PageDown                        */
  0,    /*  79   4F  Keyboard RightArrow                      */
  0,    /*  80   50  Keyboard LeftArrow                       */
  0,    /*  81   51  Keyboard DownArrow                       */
  0,    /*  82   52  Keyboard UpArrow                         */
  0,    /*  83   53  Keypad NumLock and Clear                 */
  '/',  /*  84   54  Keypad /                                 */
  '*',  /*  85   55  Keypad *                                 */
  '-',  /*  86   56  Keypad -                                 */
  '+',  /*  87   57  Keypad +                                 */
  '\n', /*  88   58  Keypad ENTER                             */
  0,    /*  89   59  Keypad 1 and End                         */
  0,    /*  90   5A  Keypad 2 and Down Arrow                  */
  0,    /*  91   5B  Keypad 3 and PageDn                      */
  0,    /*  92   5C  Keypad 4 and Left Arrow                  */
  0,    /*  93   5D  Keypad 5                                 */
  0,    /*  94   5E  Keypad 6 and Right Arrow                 */
  0,    /*  95   5F  Keypad 7 and Home                        */
  0,    /*  96   60  Keypad 8 and Up Arrow                    */
  0,    /*  97   61  Keypad 9 and PageUp                      */
  0,    /*  98   62  Keypad 0 and Insert                      */
  0,    /*  99   63  Keypad . and Delete                      */
  '|',  /*  100  64  Keyboard Non-US\ and |                   */
  0,    /*  101  65  Keyboard Application                     */
  0,    /*  102  66  Keyboard Power                           */
  '=',  /*  103  67  Keypad =                                 */
  0,    /*  104  68  Keyboard F13                             */
  0,    /*  105  69  Keyboard F14                             */
  0,    /*  106  6A  Keyboard F15                             */
  0,    /*  107  6B  Keyboard F16                             */
  0,    /*  108  6C  Keyboard F17                             */
  0,    /*  109  6D  Keyboard F18                             */
  0,    /*  110  6E  Keyboard F19                             */
  0,    /*  111  6F  Keyboard F20                             */
  0,    /*  112  70  Keyboard F21                             */
  0,    /*  113  71  Keyboard F22                             */
  0,    /*  114  72  Keyboard F23                             */
  0,    /*  115  73  Keyboard F24                             */
  0,    /*  116  74  Keyboard Execute                         */
  0,    /*  117  75  Keyboard Help                            */
  0,    /*  118  76  Keyboard Menu                            */
  0,    /*  119  77  Keyboard Select                          */
  0,    /*  120  78  Keyboard Stop                            */
  0,    /*  121  79  Keyboard Again                           */
  0,    /*  122  7A  Keyboard Undo                            */
  0,    /*  123  7B  Keyboard Cut                             */
  0,    /*  124  7C  Keyboard Copy                            */
  0,    /*  125  7D  Keyboard Paste                           */
  0,    /*  126  7E  Keyboard Find                            */
  0,    /*  127  7F  Keyboard Mute                            */
  0,    /*  128  80  Keyboard Volume Up                       */
  0,    /*  129  81  Keyboard Volume Down                     */
  0,    /*  130  82  Keyboard Locking Caps Lock               */
  0,    /*  131  83  Keyboard Locking Num Lock                */
  0,    /*  132  84  Keyboard Locking Scroll Lock             */
  ',',  /*  133  85  Keypad Comma                             */
  '=',  /*  134  86  Keypad Equal Sign                        */
  0,    /*  135  87  Keyboard International 1                 */
  0,    /*  136  88  Keyboard International 2                 */
  0,    /*  137  89  Keyboard International 3                 */
  0,    /*  138  8A  Keyboard International 4                 */
  0,    /*  139  8B  Keyboard International 5                 */
  0,    /*  140  8C  Keyboard International 6                 */
  0,    /*  141  8D  Keyboard International 7                 */
  0,    /*  142  8E  Keyboard International 8                 */
  0,    /*  143  8F  Keyboard International 9                 */
  0,    /*  144  90  Keyboard LANG1                           */
  0,    /*  145  91  Keyboard LANG2                           */
  0,    /*  146  92  Keyboard LANG3                           */
  0,    /*  147  93  Keyboard LANG4                           */
  0,    /*  148  94  Keyboard LANG5                           */
  0,    /*  149  95  Keyboard LANG6                           */
  0,    /*  150  96  Keyboard LANG7                           */
  0,    /*  151  97  Keyboard LANG8                           */
  0,    /*  152  98  Keyboard LANG9                           */
  0,    /*  153  99  Keyboard AlternateErase                  */
  0,    /*  154  9A  Keyboard SysReq/Attention                */
  0,    /*  155  9B  Keyboard Cancel                          */
  0,    /*  156  9C  Keyboard Clear                           */
  0,    /*  157  9D  Keyboard Prior                           */
  '\n', /*  158  9E  Keyboard Return                          */
  0,    /*  159  9F  Keyboard Separator                       */
  0,    /*  160  A0  Keyboard Out                             */
  0,    /*  161  A1  Keyboard Oper                            */
  0,    /*  162  A2  Keyboard Clear/Again                     */
  0,    /*  163  A3  Keyboard CrSel/Props                     */
  0,    /*  164  A4  Keyboard ExSel                           */
  0,    /*  165-175  A5-AF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0,    /*  176  B0  Keypad 00                                */
  0,    /*  177  B1  Keypad 000                               */
  ',',  /*  178  B2  Thousands Separator                      */
  '.',  /*  179  B3  Decimal Separator                        */
  0,    /*  180  B4  Currency Unit                            */
  0,    /*  181  B5  Currency Sub-unit                        */
  '(',  /*  182  B6  Keypad (                                 */
  ')',  /*  183  B7  Keypad )                                 */
  '{',  /*  184  B8  Keypad {                                 */
  '}',  /*  185  B9  Keypad }                                 */
  0,    /*  186  BA  Keypad Tab                               */
  0,    /*  187  BB  Keypad Backspace                         */
  'A',  /*  188  BC  Keypad A                                 */
  'B',  /*  189  BD  Keypad B                                 */
  'C',  /*  190  BE  Keypad C                                 */
  'D',  /*  191  BF  Keypad D                                 */
  'E',  /*  192  C0  Keypad E                                 */
  'F',  /*  193  C1  Keypad F                                 */
  '^',  /*  194  C2  Keypad XOR                               */
  '^',  /*  195  C3  Keypad ^                                 */
  '%',  /*  196  C4  Keypad %                                 */
  '<',  /*  197  C5  Keypad <                                 */
  '>',  /*  198  C6  Keypad >                                 */
  '&',  /*  199  C7  Keypad &                                 */
  0,    /*  200  C8  Keypad &&                                */
  '|',  /*  201  C9  Keypad |                                 */
  0,    /*  202  CA  Keypad ||                                */
  ':',  /*  203  CB  Keypad :                                 */
  '#',  /*  204  CC  Keypad #                                 */
  ' ',  /*  205  CD  Keypad Space                             */
  '@',  /*  206  CE  Keypad @                                 */
  '!',  /*  207  CF  Keypad !                                 */
  0,    /*  208  D0  Keypad Memory Store                      */
  0,    /*  209  D1  Keypad Memory Recall                     */
  0,    /*  210  D2  Keypad Memory Clear                      */
  0,    /*  211  D3  Keypad Memory Add                        */
  0,    /*  212  D4  Keypad Memory Subtract                   */
  0,    /*  213  D5  Keypad Memory Multiply                   */
  0,    /*  214  D6  Keypad Memory Divide                     */
  0,    /*  215  D7  Keypad +/-                               */
  0,    /*  216  D8  Keypad Clear                             */
  0,    /*  217  D9  Keypad Clear Entry                       */
  0,    /*  218  DA  Keypad Binary                            */
  0,    /*  219  DB  Keypad Octal                             */
  0,    /*  220  DC  Keypad Decimal                           */
  0,    /*  221  DD  Keypad Hexadecimal                       */
  0, 0, /*  222-223  DE-DF Reserved                           */
  0,    /*  224  E0  Keyboard LeftControl                     */
  0,    /*  225  E1  Keyboard LeftShift                       */
  0,    /*  226  E2  Keyboard LeftAlt                         */
  0,    /*  227  E3  Keyboard Left GUI                        */
  0,    /*  228  E4  Keyboard RightControl                    */
  0,    /*  229  E5  Keyboard RightShift                      */
  0,    /*  230  E6  Keyboard RightAlt                        */
  0,    /*  231  E7  Keyboard Right GUI                       */
  0,    /*  232-255  E8-FF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

	static void KeyPressedHandlerRaw(TUSBKeyboardDevice* device, unsigned char modifiers, const unsigned char RawKeys[6]);
	static void USBKeyboardDeviceTimerHandler(unsigned hTimer, void *pParam, void *pContext);

public:
	Keyboard();

	//inline u32 UpdateCount() const { return updateCount; }

	inline bool CheckChanged()
	{
		if (updateCountLastRead == updateCount)
		{
			return false;
		}
		else
		{
			updateCountLastRead = updateCount;
			return true;
		}
	//	bool dirtyOld = dirty;
	//	dirty = false;
	//	return dirtyOld;
	}
	
	unsigned char GetChar();

	inline bool KeyPressed(u32 rawKey)
	{
		int keyStatusIndex = rawKey >= 64 ? 1 : 0;
		uint64_t mask = 1ULL << (rawKey & 0x3f);
		return ((keyStatus[keyStatusIndex] & mask) && !(keyStatusPrev[keyStatusIndex] & mask));
	}
	inline bool KeyReleased(u32 rawKey)
	{
		int keyStatusIndex = rawKey >= 64 ? 1 : 0;
		uint64_t mask = 1ULL << (rawKey & 0x3f);
		return (!(keyStatus[keyStatusIndex] & mask) && (keyStatusPrev[keyStatusIndex] & mask));
	}
	inline bool KeyHeld(u32 rawKey)
	{
		int keyStatusIndex = rawKey >= 64 ? 1 : 0;
		uint64_t mask = 1ULL << (rawKey & 0x3f);
		return (keyStatus[keyStatusIndex] & mask);
	}

	inline bool KeyAnyHeld()
	{ return (keyStatus[0] | keyStatus[1]); }

	inline bool KeyEitherAlt() { return (modifier & (KEY_MOD_LALT | KEY_MOD_RALT) ); }

	inline bool KeyNoModifiers() { return (!modifier ); }

	inline bool KeyLCtrlAlt() { return (modifier == (KEY_MOD_LALT | KEY_MOD_LCTRL) ); }
	
	inline bool KeyEitherShift() { return (modifier & (KEY_MOD_LSHIFT | KEY_MOD_RSHIFT) ); }
};
#endif
