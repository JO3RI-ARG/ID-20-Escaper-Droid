#ifndef LEVELS_H
#define LEVELS_H

#define MAX_AMOUNT_OF_ROOMS                       32
#define MAX_AMOUNT_OF_INFLUENCING_OBJECTS         16
#define MAX_AMOUNT_OF_TRANSPORTERS                16
#define AMOUNT_OF_ROOMS_AT_BYTE                   0
#define AMOUNT_OF_TRANSPORTERS_AT_BYTE            1
#define AMOUNT_OF_INFLUENCING_OBJECTS_AT_BYTE     2
#define LEVEL_DOOR_DATA_START_AT_BYTE             3
#define LEVEL_ROOM_DATA_START_AT_BYTE             4
#define ROOMS_DATA_START_AT_BYTE                  5
#define DOORS_DATA_START_AT_BYTE                  ROOMS_DATA_START_AT_BYTE + 1
#define ELEMENTS_DATA_START_AT_BYTE               ROOMS_DATA_START_AT_BYTE + 5            
#define BYTES_USED_FOR_EVERY_ROOM                 13

// ROOM ORDER OF TILES
//                 /\
//                /  \
//               / 00 \
//              /\    /\
//             /  \  /  \
//            / 01 \/ 05 \
//            \    /\    /
// NORTH       \  /  \  /         EAST
//           02 \/ 06 \/ 10  
//        /\    /\    /\    /\
//       /  \  /  \  /  \  /  \
//      / 03 \/ 07 \/ 11 \/ 15 \
//     /\    /\    /\    /\    /\
//    /  \  /  \  /  \  /  \  /  \
//   / 04 \/ 08 \/ 12 \/ 16 \/ 20 \
//   \    /\    /\    /\    /\    /
//    \  /  \  /  \  /  \  /  \  /
//     \/ 09 \/ 13 \/ 17 \/ 21 \/
//      \    /\    /\    /\    /
//       \  /  \  /  \  /  \  /
//        \/ 14 \/ 18 \/ 22 \/
//              /\    /\    
// WEST        /  \  /  \         SOUTH
//            / 19 \/ 23 \
//            \    /\    /
//             \  /  \  /
//              \/ 24 \/
//               \    /
//                \  /
//                 \/


//const unsigned char PROGMEM centerOfTiles[][2] =
//{
//  { 58, 18}, {46, 24}, {34, 30}, {22, 36}, {10, 42}, // Tile  0  1  2  3  4
//  { 70, 24}, {58, 30}, {46, 36}, {34, 42}, {22, 48}, // Tile  5  6  7  8  9
//  { 82, 30}, {70, 36}, {58, 42}, {46, 48}, {34, 54}, // Tile 10 11 12 13 14
//  { 94, 36}, {82, 42}, {70, 48}, {58, 54}, {46, 60}, // Tile 15 16 17 18 19
//  {106, 42}, {94, 48}, {82, 56}, {70, 60}, {58, 66}, // Tile 20 21 22 23 24
//};

// NEXT LEVEL DOOR
//0b00000001,
//  |||||||└->  \  these 2 bits are used to determine what door is the next level door, make sure it exists
//  ||||||└-->  /. NORTH = 0B00000000; EAST = 0B00000001; SOUTH = 0B00000010; WEST : 0B00000011
//  |||||└--->  \ 
//  ||||└---->   |
//  |||└----->   | these 6 bits are used to set in which room the next level door is
//  ||└------>   |
//  |└------->   |
//  └-------->  / 

// NEXT LEVEL ROOM
//0b00000001,
//  |||||||└--->  \ 
//  ||||||└---->   |
//  |||||└----->   | these 6 bits are used to set in which room the next level TILE is
//  ||||└------>   |
//  |||└------->   |
//  ||└-------->  / 
//  |└--------->  NOT USED
//  └---------->  NOT USED

// ALL THE DATA FOR EACH ROOM AND EACH ROOM HAS 13 BYTES
// DOORS         NORTH        EAST       SOUTH        WEST       ENEMY1      ENEMY2     OBJECT3     FLOOR1      FLOOR2      FLOOR3      FLOOR4      FLOOR5
//0b11001110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//  ||||||||    ||||||||                                        ||||||||                ||||||||    ||||||||
//  ||||||||    ||||||||                                        ||||||||                ||||||||    |||||||└->0  \  these 3 bits are used to determine kind of sprite used for the floor
//  ||||||||    ||||||||                                        ||||||||                ||||||||    ||||||└-->1   | 0 = none; 1 = box; 2 = spike; 3 = piramide; 4 = pit; 5 = ; 6 =; 7= LEVEL UP
//  ||||||||    ||||||||                                        ||||||||                ||||||||    |||||└--->2  /
//  ||||||||    ||||||||                                        ||||||||                ||||||||    ||||└---->3  \
//  ||||||||    ||||||||                                        ||||||||                ||||||||    |||└----->4   | if the floor is 
//  ||||||||    ||||||||                                        ||||||||                ||||||||    ||└------>5   | these 5 bits are used to determine on what floor tile the element is
//  ||||||||    ||||||||                                        ||||||||                ||||||||    |└------->6   |
//  ||||||||    ||||||||                                        ||||||||                ||||||||    └-------->7  /  if all 8 bits == 0 => no floor element
//  ||||||||    ||||||||                                        ||||||||                ||||||||
//  ||||||||    ||||||||                                        ||||||||                |||||||└->0  \   these 3 bits are used to determine kind of sprite used for the object
//  ||||||||    ||||||||                                        ||||||||                ||||||└-->1   |  0 = black card; 1 = white card; 2 = battery; 3 = bullet; 4 = chip; 5 = teleport, 6 = switch OFF, 7 switch = ON,
//  ||||||||    ||||||||                                        ||||||||                |||||└--->2  /
//  ||||||||    ||||||||                                        ||||||||                ||||└---->3  \
//  ||||||||    ||||||||                                        ||||||||                |||└----->4   |
//  ||||||||    ||||||||                                        ||||||||                ||└------>5   |  these 5 bits are used to determine on what floor tile the object is
//  ||||||||    ||||||||                                        ||||||||                |└------->6   |
//  ||||||||    ||||||||                                        ||||||||                └-------->7  /   if all 8 bits == 0 => no object
//  ||||||||    ||||||||                                        |||||||| 
//  ||||||||    ||||||||                                        |||||||└->0  \   hese 3 bits are used to determine kind of sprite used for the enemy
//  ||||||||    ||||||||                                        ||||||└-->1   |  0 = BOX; 1 = FLYER; 2 = MOVER; 3 = SHOOTER;
//  ||||||||    ||||||||                                        |||||└--->2  /
//  ||||||||    ||||||||                                        ||||└---->3  \
//  ||||||||    ||||||||                                        |||└----->4   |
//  ||||||||    ||||||||                                        ||└------>5   |  these 5 bits are used to determine on what floor tile the enemy is
//  ||||||||    ||||||||                                        |└------->6   |
//  ||||||||    ||||||||                                        └-------->7  /   if all 8 bits == 0 => no enemy
//  ||||||||    ||||||||
//  ||||||||    |||||||└->0  \  these 2 bits are used to determine what door you'll go to
//  ||||||||    ||||||└-->1  /
//  ||||||||    |||||└--->2  \
//  ||||||||    ||||└---->3   |
//  ||||||||    |||└----->4   | these 6 bits are used for the roomnumber you'll go to
//  ||||||||    ||└------>5   |
//  ||||||||    |└------->6   |
//  ||||||||    └-------->7  /
//  ||||||||
//  |||||||└->0  DOOR NORTH  is closed (0 = false / 1 = true)
//  ||||||└-->1  DOOR EAST   is closed (0 = false / 1 = true)
//  |||||└--->2  DOOR SOUTH  is closed (0 = false / 1 = true)
//  ||||└---->3  DOOR WEST   is closed (0 = false / 1 = true)
//  |||└----->4  DOOR NORTH  exists    (0 = false / 1 = true)
//  ||└------>5  DOOR EAST   exists    (0 = false / 1 = true)
//  |└------->6  DOOR SOUTH  exists    (0 = false / 1 = true)
//  └-------->7  DOOR WEST   exists    (0 = false / 1 = true)
//
//
//
// transporters data, the order of the data is by ascending numbers (ROOM X, ROOM Y, ROOM Z , ...)
// GOTO ROOM
//0b00000001,
//  |||||||└->0  \
//  ||||||└-->1   |
//  |||||└--->2   | these 6 bits are used for the roomnumber you'll go to
//  ||||└---->3   |
//  |||└----->4   |
//  ||└------>5  /
//  |└------->6 NOT USED
//  └-------->7 NOT USED
//
//
//
// influence record is 2 bytes (old unused middle "from room" byte removed)
// ELEMENTS      WHAT
//  IN ROOM    ELEMENTS
//0b00000011, 0b00011111,
//  ||||||||    ||||||||
//  ||||||||    |||||||└->0 FLOOR  5 INFLUENCED (0 = false / 1 = true)
//  ||||||||    ||||||└-->1 FLOOR  4 INFLUENCED (0 = false / 1 = true)
//  ||||||||    |||||└--->2 FLOOR  3 INFLUENCED (0 = false / 1 = true)
//  ||||||||    ||||└---->3 FLOOR  2 INFLUENCED (0 = false / 1 = true)
//  ||||||||    |||└----->4 FLOOR  1 INFLUENCED (0 = false / 1 = true)
//  ||||||||    ||└------>5 OBJECT 3 INFLUENCED (0 = false / 1 = true)
//  ||||||||    |└------->6 ENEMY  2 INFLUENCED (0 = false / 1 = true)
//  ||||||||    └-------->7 ENEMY  1 INFLUENCED (0 = false / 1 = true)
//  ||||||||
//  |||||||└->0  \
//  ||||||└-->1   |
//  |||||└--->2   | these 6 bits are the room whose elements get toggled
//  ||||└---->3   |
//  |||└----->4   |
//  ||└------>5  /
//  |└------->6 NOT USED
//  └-------->7 NOT USED
//
//

const unsigned char PROGMEM level00[] =
{
  6,          // amount of rooms
  2,          // amount of transporters
  1,          // amount of rooms with influenceable objects

  0b00001000,  // NEXT LEVEL DOOR (room<<2 | door N=0 E=1 S=2 W=3)
  0b00000010,  // NEXT LEVEL ROOM

  // DOORS         NORTH       EAST        SOUTH       WEST         ENEMY1      ENEMY2        OBJECT3       FLOOR1      FLOOR2      FLOOR3      FLOOR4      FLOOR5
  0b01100000, 0b00000000, 0b00000111, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room0
  0b11000100, 0b00000000, 0b00000000, 0b00001000, 0b00000001, 0b00010000, 0b01011000, 0b10101000, 0b10011100, 0b10010100, 0b10001100, 0b10000100, 0b01111100, // room1
  0b00010000, 0b00000110, 0b00000000, 0b00000000, 0b00000000, 0b11000010, 0b10100010, 0b10001010, 0b01100111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room2
  0b01000000, 0b00000000, 0b00000000, 0b00010100, 0b00000000, 0b00000000, 0b00000000, 0b01100101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room3
  0b00010000, 0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01100101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room4
  0b00010000, 0b00001110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01100110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room5

  // transporters
  0b00000000, // T0
  0b00000011, // T1

  // influence: 2 bytes — target room, mask
  0b00000001, 0b00011111,
};


const unsigned char PROGMEM level01[] =
{
  8, 0, 0,
  (6 << 2) | 2,   // black-card door: room 6 SOUTH
  7,              // exit tile in vault room 7
  // DOORS          N dest      E dest      S dest      W dest       EN1        EN2         OBJ          F1         F2         F3         F4         F5
  96, 0, 11, 4, 0, 0, 0, 129, 33, 65, 161, 0, 0,  // r0
  80, 2, 0, 12, 0, 121, 0, 171, 59, 139, 0, 0, 0,  // r1
  192, 0, 0, 16, 1, 80, 0, 160, 51, 91, 131, 0, 0,  // r2
  48, 6, 23, 0, 0, 114, 0, 33, 66, 98, 146, 0, 0,  // r3
  144, 10, 0, 0, 25, 0, 0, 100, 60, 92, 108, 140, 0,  // r4
  128, 0, 0, 0, 13, 163, 0, 18, 66, 130, 0, 0, 0,  // r5
  100, 0, 19, 28, 0, 41, 0, 171, 73, 153, 0, 0, 0,  // r6  SOUTH locked (black card)
  16, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // r7 vault, only NORTH
};

const unsigned char PROGMEM level02[] =
{
  8, 2, 1,
  (6 << 2) | 1,   // black-card door: room 6 EAST
  7,
  64, 0, 0, 4, 0, 0, 0, 161, 65, 129, 33, 0, 0,  // r0
  176, 2, 11, 0, 17, 122, 0, 102, 59, 139, 0, 0, 0,  // r1 switch
  128, 0, 0, 0, 5, 81, 0, 101, 50, 146, 0, 0, 0,  // r2 teleport
  80, 18, 0, 24, 0, 0, 0, 35, 68, 100, 132, 164, 0,  // r3 pits
  96, 0, 7, 12, 0, 169, 41, 16, 91, 107, 0, 0, 0,  // r4 black card
  0, 0, 0, 0, 0, 115, 0, 101, 74, 154, 0, 0, 0,  // r5 teleport only
  50, 14, 31, 0, 0, 64, 0, 162, 33, 193, 0, 0, 0,  // r6 EAST locked
  128, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0,  // r7 vault, only WEST
  5,
  2,
  3, 0b00011110,
};

const unsigned char PROGMEM level03[] =
{
  10, 2, 1,
  (8 << 2) | 2,   // black-card door: room 8 SOUTH
  9,
  96, 0, 7, 8, 0, 0, 0, 129, 25, 73, 0, 0, 0,  // r0
  192, 0, 0, 12, 1, 98, 0, 163, 58, 138, 0, 0, 0,  // r1
  48, 2, 15, 0, 0, 121, 0, 102, 51, 147, 0, 0, 0,  // r2 switch
  240, 6, 23, 16, 9, 83, 176, 18, 67, 131, 0, 0, 0,  // r3 hub
  16, 14, 0, 0, 0, 0, 0, 165, 60, 92, 108, 140, 0,  // r4 pits + teleport
  192, 0, 0, 24, 13, 41, 169, 96, 66, 130, 91, 0, 0,  // r5 black card
  48, 22, 35, 0, 0, 114, 0, 101, 33, 164, 0, 0, 0,  // r6 teleport
  64, 0, 0, 32, 0, 0, 0, 99, 49, 145, 0, 0, 0,  // r7
  212, 30, 0, 36, 25, 123, 0, 33, 74, 154, 0, 0, 0,  // r8 SOUTH locked
  16, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // r9 vault, only NORTH
  6,
  4,
  4, 0b00011110,
};

const unsigned char PROGMEM level04[] =
{
  12, 2, 1,
  (10 << 2) | 2,  // black-card door: room 10 SOUTH
  11,
  96, 0, 11, 4, 0, 0, 0, 129, 33, 161, 0, 0, 0,  // r0
  48, 2, 15, 0, 0, 0, 0, 171, 49, 65, 89, 105, 129,  // r1 boxes
  192, 0, 0, 16, 1, 81, 0, 102, 59, 139, 0, 0, 0,  // r2 switch
  192, 0, 0, 24, 5, 123, 43, 19, 66, 98, 130, 0, 0,  // r3 shooters
  48, 10, 23, 0, 0, 114, 0, 101, 36, 164, 0, 0, 0,  // r4 teleport
  128, 0, 0, 0, 17, 169, 0, 96, 60, 92, 108, 140, 0,  // r5 black card + pits
  48, 14, 31, 0, 0, 80, 0, 161, 65, 129, 0, 0, 0,  // r6
  192, 0, 0, 32, 25, 121, 0, 101, 50, 146, 0, 0, 0,  // r7 teleport
  48, 30, 43, 0, 0, 114, 34, 20, 75, 155, 0, 0, 0,  // r8
  64, 0, 0, 40, 0, 0, 0, 99, 65, 129, 0, 0, 0,  // r9
  212, 38, 0, 44, 33, 163, 0, 33, 90, 106, 0, 0, 0,  // r10 SOUTH locked
  16, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // r11 vault, only NORTH
  7,
  4,
  5, 0b00011110,
};

const unsigned char *levels[] =
{
  level00, level01, level02, level03, level04
};

#define AMOUNT_OF_LEVELS  (sizeof(levels) / sizeof(levels[0]))

#endif
