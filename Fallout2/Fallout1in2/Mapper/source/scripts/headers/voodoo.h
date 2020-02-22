#ifndef VOODOO_H
#define VOODOO_H

#include "sfall/sfall.rotators.h"

/**************************************************
 *                                                *
 *       All kinds of unsafe scripting macros     *
 *  Requires AllowUnsafeScripting=1 in ddraw.ini  *
 *                                                *
 **************************************************/

// Should be replaced with something like write_byte(address, value, length) when/if sfall adds support for it
// Which is very unlikely to ever happen. https://github.com/phobos2077/sfall/issues/285
procedure VOODOO_CAVE(variable address, variable length)
begin
    // Assumes that SafeMemSet8 is written before to 0x480f75
    call_offset_v3(0x480f75, address, 0x90, length);
end

// This will disable the "You encounter: ..." message:
#define VOODOO_disable_YouEncounter_message      \
               write_int( 0x4C1011, 0x90909090); \
               write_byte(0x4C1015, 0x90);       \
               write_int( 0x4C1042, 0x90909090); \
               write_byte(0x4C1046, 0x90)

// This will change the rest timer "wait until 08:00" to 06:00 like in Fallout 1.
#define VOODOO_rest_till0600 \
               write_int(0x4995F3, 6)

// This will replace RoboDog PID with Dogmeat PID in hardcoded list of dogs PIDs; required for woofs and arfs in combat control
#define VOODOO_dogmeat_pm_dialog \
               write_byte(0x444d10, 0x7a)

// Removes the text under green circles on the worldmap
// Used by Classic Worldmap mod
#define VOODOO_remove_circle_name \
               /* fallout2.wmInterfaceDrawCircleOverlay+0xD2 */            \
               write_byte (0x4c407a, 0x90);       /* nop */                \
               /* fallout2.wmInterfaceDrawCircleOverlay+0xEC */            \
               write_int  (0x4c4094, 0x90909090); /* nop; nop; nop; nop */ \
               write_short(0x4c4098, 0x9090)      /* nop; nop */           \

// No radius when a location is revealed on the worldmap
// Modifies sfall code
#define VOODOO_location_discover_radius \
              begin                                                                          \
               write_byte (0x480ee4, 0x60);       /* pushad */                               \
               write_int  (0x480ee5, 0x4670a12e); /* mov eax,cs:[4C4670] */                  \
               write_short(0x480ee9, 0x004c);                                                \
               /* Calculate where ddraw.sfall::wmAreaMarkVisitedState_hack+0x51 is */        \
               write_int  (0x480eeb, 0x4c46c5ba); /* mov edx,fallout2.4C46C5 */              \
               write_byte (0x480eef, 0x00);                                                  \
               write_short(0x480ef0, 0xd001);     /* add eax,edx */                          \
               write_short(0x480ef2, 0xc689);     /* mov esi,eax */                          \
               /* esi is now the address where the radius byte is */                         \
               /* since the memory of the code address is memory protected, */               \
               /* we need to use VirtualProtect to change this */                            \
               write_short(0x480ef4, 0xec83);     /* sub esp,4 */                            \
               write_byte (0x480ef6, 0x04);                                                  \
               write_byte (0x480ef7, 0x54);       /* push esp */                             \
               write_short(0x480ef8, 0x406a);     /* push 40 */                              \
               write_short(0x480efa, 0x016a);     /* push 1 */                               \
               write_byte (0x480efc, 0x50);       /* push eax */                             \
               write_int  (0x480efd, 0x1815ff2e); /* call cs:[<&fallout2.VirtualProtect>] */ \
               write_short(0x480f01, 0x6c02);                                                \
               write_byte (0x480f03, 0x00);                                                  \
               write_short(0x480f04, 0x06c6);     /* mov ds:[esi],0 */                       \
               write_byte (0x480f06, 0x00);                                                  \
               write_short(0x480f07, 0xc483);     /* add esp,4 */                            \
               write_byte (0x480f09, 0x04);                                                  \
               write_byte (0x480f0a, 0x61);       /* popad */                                \
               write_byte (0x480f0b, 0xc3);       /* ret */                                  \
               call_offset_v0(0x480ee4);                                                     \
              end                                                                            \
              noop



// Fill_W that works like in Fallout 1
// https://github.com/rotators/Fo1in2/issues/16
#define VOODOO_fill_w \
              begin                                                                         \
               /* wmMarkSubTileRadiusVisited_ */                                            \
               write_int  (0x4c3735, 0xfbd7d2e9); /* jmp [patch] */                         \
               write_byte (0x4c3739, 0xff);                                                 \
               /* fill_w implementation */                                                  \
               write_short(0x480f0c, 0x4d75);     /* jne _ret */                            \
               write_short(0x480f0e, 0xec83);     /* sub esp,4 */                           \
               write_byte (0x480f10, 0x04);                                                 \
               write_int  (0x480f11, 0x002404c6); /* mov ss:[esp+4],0 */                    \
               /* _loop_begin */                                                            \
               write_int  (0x480f15, 0x0c244c8b); /* mov ecx,ss:[esp+C] */                  \
               /* move to the next tile to the left */                                      \
               write_byte (0x480f19, 0x49);       /* dec ecx */                             \
               /* the comparison checks are to see if the tile we are currently on is */    \
               /* one of the tiles on the right side of the wm (3, 7, 11 or 15) */          \
               /* if it is, it means we've wrapped around */                                \
               write_short(0x480f1a, 0xf983);     /* cmp ecx,3 */                           \
               write_byte (0x480f1c, 0x03);                                                 \
               write_short(0x480f1d, 0x3974);     /* je _end */                             \
               write_short(0x480f1f, 0xf983);     /* cmp ecx,7 */                           \
               write_byte (0x480f21, 0x07);                                                 \
               write_short(0x480f22, 0x3474);     /* je _end */                             \
               write_short(0x480f24, 0xf983);     /* cmp ecx,B */                           \
               write_byte (0x480f26, 0x0b);                                                 \
               write_short(0x480f27, 0x2f74);     /* je _end */                             \
               write_short(0x480f29, 0xf983);     /* cmp ecx,F */                           \
               write_byte (0x480f2b, 0x0f);                                                 \
               write_short(0x480f2c, 0x2a74);     /* je _end */                             \
               write_short(0x480f2e, 0xed31);     /* xor ebp,ebp */                         \
               write_int  (0x480f30, 0x0c244c89); /* mov ss:[esp+C],ecx */                  \
               /* _reveal_subtile */                                                        \
               write_short(0x480f34, 0x026a);     /* push 2 */                              \
               write_int  (0x480f36, 0x1024448b); /* mov eax,ss:[esp+10] */                 \
               write_short(0x480f3a, 0xf189);     /* mov ecx,esi */                         \
               write_short(0x480f3c, 0xfb89);     /* mov ebx,edi */                         \
               write_byte (0x480f3e, 0x56);       /* push esi */                            \
               write_short(0x480f3f, 0xea89);     /* mov edx,ebp */                         \
               write_byte (0x480f41, 0x45);       /* inc ebp */                             \
               write_int  (0x480f42, 0x0424ede8); /* call wmMarkSubTileOffsetVisitedFunc */ \
               write_byte (0x480f46, 0x00);                                                 \
               /* did we uncover all the subtiles in the tile? */                           \
               /* if not, go to _reveal_subtile and uncover another one */                  \
               write_short(0x480f47, 0xfd83);     /* cmp ebp,7 */                           \
               write_byte (0x480f49, 0x07);                                                 \
               write_short(0x480f4a, 0xe87c);     /* jl _reveal_subtile */                  \
               write_short(0x480f4c, 0x048b);     /* mov eax,ss:[esp+4] */                  \
               write_byte (0x480f4e, 0x24);                                                 \
               write_byte (0x480f4f, 0x40);       /* inc eax */                             \
               write_short(0x480f50, 0xf883);     /* cmp eax,2 */                           \
               write_byte (0x480f52, 0x02);                                                 \
               write_short(0x480f53, 0x0489);     /* mov ss:[esp+4],eax */                  \
               write_byte (0x480f55, 0x24);                                                 \
               write_short(0x480f56, 0xbd7c);     /* jl _loop_begin */                      \
               /* _end */                                                                   \
               write_short(0x480f58, 0xc483);     /* add esp,4 */                           \
               write_byte (0x480f5a, 0x04);                                                 \
               /* _ret */                                                                   \
               write_short(0x480f5b, 0xc483);     /* add esp,C */                           \
               write_byte (0x480f5d, 0x0c);                                                 \
               write_int  (0x480f5e, 0x0427d7e9); /* jmp fallout2.4C373A */                 \
               write_byte (0x480f62, 0x00);                                                 \
              end                                                                           \
              noop

// Used to refresh the game window, including HRP black edges
#define VOODOO_display_win_redraw \
               call_offset_v1(0x4d6f5c,read_int(0x631e4c)) // win_draw_(_display_win)

#define VOODOO_SafeMemSet8 \
              begin                                                                            \
               write_byte (0x480f75, 0x52);       /* push edx   - int num (bytes) */           \
               write_byte (0x480f76, 0x53);       /* push ebx   - uint8 value */               \
               write_byte (0x480f77, 0x50);       /* push eax   - void* ptr */                 \
               write_byte (0x480f78, 0x57);       /* push edi */                               \
               write_short(0x480f79, 0xec83);     /* sub esp,4  - DWORD oldProtect; */         \
               write_byte (0x480f7b, 0x04);                                                    \
               write_byte (0x480f7c, 0x54);       /* push esp   - &oldProtect */               \
               write_short(0x480f7d, 0x406a);     /* push 40    - PAGE_EXECUTE_READWRITE */    \
               write_byte (0x480f7f, 0x53);       /* push ebx   - int num (bytes) */           \
               write_byte (0x480f80, 0x50);       /* push eax   - void* ptr */                 \
               write_int  (0x480f81, 0x1815ff2e); /* call cs:[<&fallout2.VirtualProtect>] */   \
               write_short(0x480f85, 0x6c02);                                                  \
               write_byte (0x480f87, 0x00);                                                    \
               write_short(0x480f88, 0xc483);     /* add esp,4 */                              \
               write_byte (0x480f8a, 0x04);                                                    \
               write_int  (0x480f8b, 0x08244c8b); /* mov ecx,ss:[esp+0x8] - int num (bytes) */ \
               write_int  (0x480f8f, 0x0c24448a); /* mov al, ss:[esp+0xC] - uint8 value */     \
               write_int  (0x480f93, 0x04247c8b); /* mov edi,ss:[esp+0x4] - void* ptr */       \
               write_short(0x480f97, 0xaaf3);     /* rep stosb */                              \
               write_byte (0x480f99, 0x5f);       /* pop edi */                                \
               write_byte (0x480f9a, 0x58);       /* pop eax */                                \
               write_byte (0x480f9b, 0x5b);       /* pop ebx */                                \
               write_byte (0x480f9c, 0x5a);       /* pop edx */                                \
               write_byte (0x480f9d, 0xc3);       /* ret */                                    \
              end                                                                              \
              noop

// This will create codecave out of few selfrun functions related to .vcr recording
// As creation and hotkey blocking is done by scripts, recording is available *only* before first start/load game
#define VOODOO_codecave_selfrun \
              begin                                                                     \
               /* ignore CTRL+R on main screen */                                       \
               write_short(0x480c90, 0x6666); /* nop */                                 \
               write_byte (0x480c92, 0x90);                                             \
               write_short(0x480c93, 0xff33); /* xor edi,edi */                         \
               /* ignore main_selfrun_exit_ call on game exit */                        \
               write_int  (0x480ca2, 0x66666666); /* nop */                             \
               write_byte (0x480ca6, 0x90);                                             \
               /* clear main_selfrun_init_, main_selfrun_exit_, main_selfrun_record_ */ \
               call VOODOO_CAVE(0x480ee4, 438);                                         \
              end                                                                       \
              noop


////////////////////////////////////////////////////// TEST ZONE //////////////////////////////////////////////////////


// This will make screeenshot without interface and cursor
// recommended: f2_res.ini->[IFACE]->IFACE_BAR_MODE=1,
//              f2_res.ini->[IFACE]->IFACE_BAR_SIDE_ART=0
// ---
// hidemouse/showmouse only ... DON'T! hidemouse doesn't work until cursor is over interface or player is on worldmap
// tap_key(DIK_F12) ... DON'T! doesn't work with compact keyboards (Fn + other key required to "press" F12)
#define VOODOO_clean_screenshot \
               if(get_game_mode bwand 0x1) then                  \
                  hidemouse;                                     \
               else                                              \
               begin                                             \
                  intface_hide;                                  \
                  call_offset_v0(0x44ce34); /* gmouse_3d_off_ */ \
               end                                               \
               call_offset_v0(0x4c8f4c);      /* dump_screen_ */ \
               if(get_game_mode bwand 0x1) then                  \
                  showmouse;                                     \
               else                                              \
               begin                                             \
                  call_offset_v0(0x44cd2c);  /* gmouse_3d_on_ */ \
                  intface_show;                                  \
               end                                               \
               noop

// This will make screenshots saved in "screen" directory
// HRP required
// sfall-rotators required (sfall cannot patch HRP from scripts)
// ---
// strings are arguments for sprintf(), results are passed to hrp_fopen(); see 0x1001a8e0
// strings cannot be longer than 16b (including trailing "\0")
// directory must exist, HRP/engine won't create it on their own
// --
// hrp_fopen    = 0x1002966a
// hrp_scrnameR = 0x100397bc used when selecting screenshot name
// hrp_scrnameW = 0x100397d0 used when creating screenshot file
#define VOODOO_subdirectory_screenshots \
               r_write_string(r_hrp_offset(0x100397bc), "screen/%.5d.bmp"); \
               r_write_string(r_hrp_offset(0x100397d0), "screen/%.5d.bmp")


////////////////////////////////////////////////////// DANGER ZONE //////////////////////////////////////////////////////


#define VOODOO_ERROR_READ   r_write_byte(read_byte(-1),0)
#define VOODOO_ERROR_WRITE  r_write_byte(-1,0)
#define VOODOO_ERROR_RING0  r_call_offset(0x410003)

#endif // VOODOO_H //
