#pragma once

#include "Common.h"
#include "DeathlordHacks.h"

// Program Counter locations to hijack when processing the game
// to change the behavior on the fly without patching the original code
// Research by rikkles and qkumba

// Pre-game (main menu, boot disk)
#define PC_PROMPT_USE_2_DRIVES		0x871C		// Prompts for 2 drives. Skip PC to $8738 to bypass the prompt.
#define PC_PROMPT_INSERT_SCENARIOS	0x840B		// Prompts to insert scenarii. NOP to avoid it. Before $842B, swap images for verification to pass.
#define PC_SCENARIOS_ARE_IN_DRIVES	0x8430		// At this PC, the scenarii are in the drives and validated
#define PC_CHECK_KEYPRESS_TITLE		0x1D47		// Check for keypress on title screen to go to menu ("Play a Game...")
#define PC_CHECK_KEYPRESS_MENU		0x1C19		// Checks for 'U', 'C', 'P'

// In-game
#define PC_RNG						0x4AE0		// Random number generator
#define PC_WAIT_FOR_KEYPRESS		0x540B		// Waits for a keypress
#define PC_DECREMENT_TIMER			0x621F		// routine to decrement a timer before it makes the player "wait" and pass a turn
#define PC_TRANSIT_OUT_OVERLAND		0xE8F4		// routine that gets the player into a dungeon or town
#define PC_TRANSIT_IN_OVERLAND		0xEF0A		// routine that gets the player back in the overland
#define PC_BEGIN_DRAWING_TILES		0x5931		// Beginning of the tiles drawing routine on the main screen. $300-350 contains the "current" map, $351-3A0 contains the "new" map
#define PC_END_DRAWING_TILES		0x586A		// After all the drawing is done and the LOS is updated. $300-350 contains the map
#define PC_CHECK_REAR_ATTACK		0xA7EF		// BCS after CMP that checks for the rear rank not allowed to attack
#define PC_CHECK_SEARCH_SUCCESS		0x9428		// BCS after CMP that checks success of search
#define PC_ENEMY_ACTION_DRAIN		0xAB19		// BCS: Always branch to avoid level drain. This option makes the enemy skip the turn without feedback
#define PC_MAGIC_WATER_EFFECT		0xB707		// Set X register to 0 to always branch to stat +1
#define PC_STAT_INCREASE_CEILING	0xB7A3		// BCS: Don't branch to remove the max 18 of the stat increase from magic water
#define PC_CHAR_HP_LOSS				0x54B8		// It sets A to 01 (lo byte) and Y to 00 (hi byte), which is the amount we'll drop HP by because of starvation, toxicity, etc... when calling subroutine 0x605D. X has the char position.
#define PC_CHAR_TILE_DAMAGE			0x6038		// JSR to the tile damage subroutine at 0x6063. Bypass it to avoid tile damage to char at index in X.
#define PC_SAVE_AFTER_DEATH			0x5BC5		// BNE should not branch in order to stop saving after a char dies in combat
#define PC_NINJA_MONK_AC_RESET		0xA952		// AND with 0F that resets the Ninja and Monk A/C to 0 every 32 levels. Bypass this bug.
#define PC_CHECK_READY_WEAPON		0x6B98		// Skip this JSR to bypass the test if the weapon is usable. A has weapon id, X has char index, Y has Melee/Range (0/1)
#define PC_INCREMENT_LEVEL			0xF5BE		// Increments the level by 1 when training
#define PC_RESET_XP_ON_LEVELUP		0xF5D4		// STA 0 on both low and high bytes
#define PC_LEVELUP_CHECK			0xF563		// At academy, when "buying" level, if A is 0, does not level up.
#define PC_PICKLOCK_CHECK			0x938C		// Beginning of the lockpick routine. NOP the instruction at 93CD and set 93D0 to 0x66 to succeed 100% of the time
#define PC_ALL_CHARS_DEAD			0x880C		// "ALL PLYRS OUT". Everyone's dead, time to reload from a backup!

#define PC_MAP_KEY_PRESS			0x5893		// A key has been pressed when on the map, value is in A (could be movement, spell choice...)
#define SP_MAP_KEY_PRESS_1			0xCB		// Last 2 bytes in stack pointer when waiting for keystroke in menu list only
#define SP_MAP_KEY_PRESS_2			0x57		// Last 2 bytes in stack pointer when waiting for keystroke in menu list only
#define SP_MAP_KEY_PRESS_SPELL_1	0x1E		// Last 2 bytes in stack pointer when typing spell
#define SP_MAP_KEY_PRESS_SPELL_2	0x5F		// Last 2 bytes in stack pointer when typing spell

// TODO: Recalc armorclass on inventory change in inventory manager!
#define PC_RECALC_ARMORCLASS		0xA93F		// Jumping to this routine recalculates the armor classes of all characters

#define PC_SCROLL_WINDOW			0x5395		// This scrolls the active window
												// if A is $01, it's the log window
												// if A is $0B, it's the last line of the billboard that scrolls (in battle)
												// It scrolls many times on the billboard, probably for each line
#define PC_PRINT_CHAR				0x532D		// Prints a char on screen using: AF is active Y, AE is active X, AB is width, AC is height, AD is original X (for line-feed), AA is original Y
#define PC_INVERSE_CHAR				0x53FF		// Inverses an existing char (or un-inverses it). Used in some menu routines.
#define PC_PRINT_CLEAR_AREA			0x52BC		// Clears a print area. Coordinates are in 0xAA-0xAD

// Battle module
// In general, anything that impacts a CHAR below will have PARTY_CURRENT_CHAR_POS (See DeathlordHacks.h) filled in
#define PC_BATTLE_AMBUSH			0xEB43		// Pre-entry point if ambushed
#define PC_BATTLE_ENTER				0xEC30		// Entry point for battle module (also called after ambush)
#define PC_BATTLE_EXIT				0xA37F		// Exits the battle module (before clearing the screen areas and handling loot)
#define PC_BATTLE_ENEMY_HP_SET		0xA2D1		// Point where the MEM_ENEMY_HP_START is filled in at the start of the battle
#define MEM_BATTLE_GOLD_LO			0x009A		// Gold at end of battle Low Byte
#define MEM_BATTLE_GOLD_HI			0x009B		// Gold at end of battle High Byte
#define PC_BATTLE_HAS_GOLD			0x8EAA		// The previous instruction at 0x8EA7 branches away if A > mem(0x8F55+X). It's a % chance of loot based on enemy type
#define PC_BATTLE_DISPLAY_GOLD		0x8EDF		// Start of the gold display routine 0x8EDF prints the number, 0x8EE2-0x8EF1 prints " gold pieces!"
#define PC_BATTLE_CHAR_BEGIN_TURN	0x5C0B		// Char start turn. Char # is in X (before it's stored in PARTY_CURRENT_CHAR_POS)
#define PC_BATTLE_CHAR_BEGIN_ATK	0xA85E		// Char starts attack
#define PC_BATTLE_CHAR_HAS_HIT		0xA88E		// player hits monster, X = monster index, A/MEM_DAMAGE_AMOUNT = damage
#define PC_BATTLE_CHAR_DID_DMG		0xA9FA		// Character inflicted some damage. Don't know amount yet.
#define PC_BATTLE_CHAR_HAS_KILLED	0xA8C2		// Character has killed enemy, Enemy is in X, damage in MEM_DAMAGE_AMOUNT
#define PC_BATTLE_CHAR_HAS_BANISHED	0x7B53		// Enemy was banished (hard kill, no check for HP). Enemy is in X, but 1-based!
#define PC_BATTLE_CHAR_HAS_HEALED	0x785B		// Char in PARTY_CURRENT_CHAR_POS will heal char in X amount of A in high byte, and Y in low byte
#define PC_BATTLE_CHAR_END_ATK		0xAB49		// End of a single char attack. The damage is in A and also in MEM_DAMAGE_AMOUNT
#define PC_BATTLE_CHAR_END_TURN		0xA886		// Char has exhausted all attack. Move on to next char
#define PC_BATTLE_ENEMY_BEGIN_TURN	0xA50C		// Enemy start turn. Enemy # is in X (before it's stored in MEM_BATTLE_ENEMY_INDEX)
#define PC_BATTLE_ENEMY_BEGIN_ATK	0xAA42		// Enemy starts attack
#define PC_BATTLE_ENEMY_MISSED		0xAB43		// Enemy missed, after branch from 0xAADF which decides on hit/miss
#define PC_BATTLE_ENEMY_HAS_HIT		0xADC7		// Enemy hits player, PARTY_CURRENT_CHAR_POS = player index, MEM_DAMAGE_AMOUNT = damage
#define PC_BATTLE_ENEMY_HAS_KILLED	0xADF0		// Enemy has killed the player, PARTY_CURRENT_CHAR_POS = player index, MEM_DAMAGE_AMOUNT = damage
#define PC_BATTLE_ENEMY_END_ATK		0xAB49		// End of an enemy hit cycle (for example, 3 hit attempts will trigger 3 times)
#define PC_BATTLE_TURN_END			0xAF00		// End of any turn for any player. 6 chars and 3 enemies will trigger at least 9 times per round
#define PC_BATTLE_BEGIN_XP_ALLOC	0xA30A		// LDX #$00: starts with the first char. Randomize to start with any char the xp allocation routine
#define MEM_BATTLE_GETXP_START		0xAFE8		// Array for each char that, if > 0, gives XP to the char after the battle.
#define MEM_CHAR_ATK_COUNT			0xA882		// Current char remaining attack count. Set at PC_BATTLE_CHAR_BEGIN_ATK
#define MEM_DAMAGE_AMOUNT			0x67		// Damage amount of the last attack
#define MEM_ENEMY_DMG_AMOUNT		0x6068		// Enemy's damage to player at X
#define MEM_ENEMY_COUNT				0x52		// Number of enemies
#define MEM_ENEMY_HP_START			0xAFAA		// Array of HPs for all enemies. Use MEM_ENEMY_COUNT to know the length
#define MEM_BATTLE_MONSTER_INDEX	0xFC23		// Index of the monster we're fighting, index into GAMEMAP_ARRAY_MONSTER_ID. FF is empty
#define MEM_BATTLE_ENEMY_INDEX		0xA574		// Index of the monster instance who is attacking
#define PC_BATTLE_CALC_NUM_ENEMY1	0x5200		// JSR (jump table) to calculate # of enemies. Returns at 0xA2AC with enemy ct in X.
#define PC_BATTLE_CALC_NUM_ENEMY2	0x6188		// JMP from the above JSR
#define PC_BATTLE_BEGIN_ENEMY_HP	0xA423		// RTS after having calculated HP of enemy in position X, using sum(rand(0-7)), MONSTER_CURR_HPMULT times.

/* the JSRs right after 0xA37F are:
0xA382: JSR 0x51EB	Redraws center right area
0xA385: JSR 0x5162	Clears enemy sprite from map
0xA388: JSR 0x5212	Clears the billboard area top right
0xA38B: JSR 0x5203	Sets co-ords of active window
0xA38E: JSR 0x5194	Clear the log area bottom left
*/

/*
At 0x702B the game checks the type of magic user for the spell level requirement
If it's 4 and above, it's a weak magic user and they divide the max spell level by 4
*/

struct regsrec
{
  BYTE a;   // accumulator
  BYTE x;   // index X
  BYTE y;   // index Y
  BYTE ps;  // processor status
  WORD pc;  // program counter
  WORD sp;  // stack pointer
  BYTE bJammed; // CPU has crashed (NMOS 6502 only)
};
extern regsrec    regs;

extern unsigned __int64 g_nCumulativeCycles;

void    CpuDestroy ();
void    CpuCalcCycles(ULONG nExecutedCycles);
DWORD   CpuExecute(const DWORD uCycles, const bool bVideoUpdate);
ULONG   CpuGetCyclesThisVideoFrame(ULONG nExecutedCycles);
void    CpuInitialize ();
void    CpuSetupBenchmark ();
void	CpuIrqReset();
void	CpuIrqAssert(eIRQSRC Device);
void	CpuIrqDeassert(eIRQSRC Device);
void	CpuNmiReset();
void	CpuNmiAssert(eIRQSRC Device);
void	CpuNmiDeassert(eIRQSRC Device);
void    CpuReset ();

BYTE	CpuRead(USHORT addr, ULONG uExecutedCycles);
void	CpuWrite(USHORT addr, BYTE value, ULONG uExecutedCycles);

enum eCpuType {CPU_UNKNOWN=0, CPU_6502=1, CPU_65C02, CPU_Z80};	// Don't change! Persisted to Registry

eCpuType GetMainCpu(void);
void     SetMainCpu(eCpuType cpu);
eCpuType ProbeMainCpuDefault(eApple2Type apple2Type);
void     SetMainCpuDefault(eApple2Type apple2Type);
eCpuType GetActiveCpu(void);
void     SetActiveCpu(eCpuType cpu);

bool Is6502InterruptEnabled(void);
void ResetCyclesExecutedForDebugger(void);
